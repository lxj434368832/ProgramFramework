#include "cfg_reg_reader.h"
#include <sstream>
#include <fstream>

const int MaxKeyLen = 0x100;
bool cfg_reg_reader::read_from_reg(HKEY reg_key_root, std::string item_name)
{
	reg_key_root_ = reg_key_root;
	reg_item_name_ = item_name;
	HKEY tmp_key_handel;

	if (RegOpenKeyExA(reg_key_root, item_name.c_str(), 0, KEY_READ, &tmp_key_handel) != ERROR_SUCCESS)
	{
		return false;
	}
	char    tmp_subkey_buff[MaxKeyLen] = { 0 };			// buffer for subkey name
	DWORD   tmp_name_len;								// size of name string 
	char    tmp_class_buff[MAX_PATH] = "";		        // buffer for class name 
	DWORD   tmp_class_len = MAX_PATH;					// size of class string 
	DWORD   tmp_subkey_count = 0;						// number of subkeys 
	DWORD   tmp_longest_subkey_len;					    // longest subkey size 
	DWORD   tmp_longest_class_len;						// longest class string 
	DWORD   tmp_values_count;							// number of values for key 
	DWORD   tmp_longest_value_name;					    // longest value name 
	DWORD   tmp_longest_value_data;					    // longest value data 
	DWORD   tmp_security_descriptor;					// size of security descriptor 
	FILETIME tmp_last_write_time;						// last write time 

	DWORD tmp_ret_code = 0;

	// Get the class name and the value count. 
	tmp_ret_code = RegQueryInfoKeyA(tmp_key_handel, tmp_class_buff, &tmp_class_len, NULL, &tmp_subkey_count, &tmp_longest_subkey_len,
		&tmp_longest_class_len, &tmp_values_count, &tmp_longest_value_name, &tmp_longest_value_data, &tmp_security_descriptor, &tmp_last_write_time);

	std::lock_guard<std::recursive_mutex> tmp_lock(reg_list_lock_);
	// Enumerate the subkeys, until RegEnumKeyEx fails.
	// 子项暂不枚举
	reg_subkey_list_.clear();
	if (tmp_subkey_count > 0)
	{
		for (DWORD i = 0; i < tmp_subkey_count; i++)
		{
			tmp_name_len = MaxKeyLen;
			tmp_ret_code = RegEnumKeyExA(tmp_key_handel, i, tmp_subkey_buff, &tmp_name_len, NULL, NULL, NULL, &tmp_last_write_time);
			if (tmp_ret_code != ERROR_SUCCESS)
			{
				RegCloseKey(tmp_key_handel);
				return false;
			}
			reg_subkey_list_.push_back(tmp_subkey_buff);
		}
	}

	// Enumerate the key values. 
	char tmp_value_name[MaxKeyLen] = { 0 };
	DWORD tmp_value_len = MaxKeyLen;
	DWORD tmp_data_type, tmp_data_size;
	BYTE tmp_value_data[MaxKeyLen * 5] = { 0 };
	reg_key_value_list_.clear();
	if (tmp_values_count)
	{
		for (DWORD i = 0; i < tmp_values_count; i++)
		{
			tmp_value_len = MaxKeyLen;
			tmp_value_name[0] = '\0';
			tmp_ret_code = RegEnumValueA(tmp_key_handel, i, tmp_value_name, &tmp_value_len, NULL, NULL, NULL, NULL);
			if (tmp_ret_code != ERROR_SUCCESS)
			{
				RegCloseKey(tmp_key_handel);
				return false;
			}

			tmp_value_data[0] = 0x00;
			tmp_data_size = MaxKeyLen * 5;
			tmp_ret_code = RegQueryValueExA(tmp_key_handel, tmp_value_name, NULL, &tmp_data_type, tmp_value_data, &tmp_data_size);
			if (tmp_ret_code != ERROR_SUCCESS)
			{
				RegCloseKey(tmp_key_handel);
				return false;
			}
			if (tmp_data_type == REG_DWORD)
			{
				DWORD tmp_int_value = *(DWORD *)tmp_value_data;
				std::stringstream tmp_ss;
				tmp_ss << tmp_int_value;
				std::string tmp_int_str = tmp_ss.str();
				reg_key_value_list_[tmp_value_name] = tmp_int_str;
				continue;
			}
			reg_key_value_list_[tmp_value_name] = (char *)tmp_value_data;
		}
	}
	RegCloseKey(tmp_key_handel);
	return true;
}

bool cfg_reg_reader::read_from_cfg(std::string file_name)
{
	cfg_file_name_ = file_name;
	std::fstream tmp_file;
	tmp_file.open(file_name, std::ios::in);
	if (!tmp_file.is_open())
		return false;
	if (tmp_file.bad())
		return false;
	std::vector<std::string> tmp_lines;
	std::string tmp_cur_line;
	while (!tmp_file.eof() && tmp_file.good())
	{
		std::getline(tmp_file, tmp_cur_line);
		if (tmp_cur_line.empty())
			continue;
		if (tmp_cur_line.size() >= 1 && (tmp_cur_line[0] == '#' || tmp_cur_line[0] == ';'))
			continue;
		tmp_lines.push_back(tmp_cur_line);
	}
	for (auto tmp_iter : tmp_lines)
	{
		_parse_cfg_line(tmp_iter);
	}
	tmp_file.close();
	return true;
}

std::string cfg_reg_reader::get_value_from_reg(std::string key)
{
	std::lock_guard<std::recursive_mutex> tmp_lock(reg_list_lock_);
	if (reg_key_value_list_.find(key) == reg_key_value_list_.end())
		return "";
	return reg_key_value_list_[key];
}

std::string cfg_reg_reader::get_value_from_cfg(std::string section_name, std::string key_name)
{
	std::lock_guard<std::recursive_mutex> tmp_lock(cfg_list_lock_);
	auto tmp_app_iter = cfg_section_list_.find(section_name);
	if (tmp_app_iter == cfg_section_list_.end())
		return "";
	auto tmp_key_iter = tmp_app_iter->second.find(key_name);
	if (tmp_key_iter == tmp_app_iter->second.end())
		return "";
	return tmp_key_iter->second;
}

std::string cfg_reg_reader::get_value_from_cfg(std::string section_name, std::string key_name, const std::string& defaut_value)
{
	std::lock_guard<std::recursive_mutex> tmp_lock(cfg_list_lock_);
	auto tmp_app_iter = cfg_section_list_.find(section_name);
	if (tmp_app_iter == cfg_section_list_.end())
		return defaut_value;
	auto tmp_key_iter = tmp_app_iter->second.find(key_name);
	if (tmp_key_iter == tmp_app_iter->second.end())
		return defaut_value;
	return tmp_key_iter->second;
}

bool cfg_reg_reader::read_str_from_cfg(std::string file_name, std::string type, std::string key, char *out_buf, std::size_t buff_len)
{
	if (0 >= GetPrivateProfileStringA(type.c_str(), key.c_str(), "", out_buf, buff_len, file_name.c_str()))
		return false;
	return true;
}

bool cfg_reg_reader::read_str_from_cfg(std::string file_name, std::string type, std::string key, std::string &out_str)
{
	out_str.assign(0x100, 0);
	if (0 >= GetPrivateProfileStringA(type.c_str(), key.c_str(), "", (char *)out_str.data(), out_str.size(), file_name.c_str()))
		return false;
	return true;
}

bool cfg_reg_reader::read_int_from_cfg(std::string file_name, std::string type, std::string key, int& out_data)
{
	out_data = GetPrivateProfileIntA(type.c_str(), key.c_str(), -1, file_name.c_str());

	if (out_data == -1)
		return false;

	return true;
}

void cfg_reg_reader::_parse_cfg_line(std::string &line)
{
	if (line.empty())
		return;
	// 去掉左右空格和去掉左右制表符
	line.erase(0, line.find_first_not_of(" \t"));
	line.erase(line.find_last_not_of(" \t") + 1);

	//// 去掉字符串中所有的空格和制表格
	//int j = 0;
	//for (int i = 0; i < line.size(); i++)
	//{
	//    if (std::isspace((unsigned char)line[i]))
	//    {
	//        continue;
	//    }
	//    line[j] = line[i];
	//    j++;
	//}
	//line.erase(j, line.size() - j);

	std::lock_guard<std::recursive_mutex> tmp_lock(cfg_list_lock_);
	auto tmp_pos = line.rfind(']');
	if (tmp_pos != std::string::npos)
	{
		if (tmp_pos != 1 && line.find('[') == 0)
		{
			std::string tmp_section = line.substr(1, tmp_pos - 1);
			tmp_section.erase(0, tmp_section.find_first_not_of(" \t"));
			tmp_section.erase(tmp_section.find_last_not_of(" \t") + 1);
			cfg_cur_section_ = tmp_section;
			cfg_section_list_[tmp_section];
		}
	}
	else
	{
		tmp_pos = line.find('=');
		if (tmp_pos != std::string::npos)
		{
			std::string tmp_key, tmp_value;
			tmp_key = line.substr(0, tmp_pos);
			tmp_value = line.substr(tmp_pos + 1, line.size());
			tmp_key.erase(0, tmp_key.find_first_not_of(" \t"));
			tmp_key.erase(tmp_key.find_last_not_of(" \t") + 1);
			tmp_value.erase(0, tmp_value.find_first_not_of(" \t"));
			tmp_value.erase(tmp_value.find_last_not_of(" \t") + 1);
			cfg_section_list_[cfg_cur_section_].insert(std::map<std::string, std::string>::value_type(tmp_key, tmp_value));
		}
	}
}