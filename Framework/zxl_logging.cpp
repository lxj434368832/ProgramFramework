#include "zxl_logging.h"
#include <vector>
#include <ctime>
#include <algorithm>
#include <cctype>
#ifdef WIN32
#include <Windows.h>
#endif

using namespace zxl;
zx_logger* zx_logger::s_instance_ = nullptr;
zx_logger* zx_logger::instance()
{
    if (!s_instance_)
        s_instance_ = (new zx_logger);
    return s_instance_;
}

void zx_logger::delete_instance()
{
	delete s_instance_;
	s_instance_ = nullptr;
}

zx_logger::zx_logger() : is_running_(true)
{
	_load_container_from_config(cfg_);
    write_thrd_ = std::move(std::thread(std::bind(&zx_logger::_write_to_file_thrd, this)));
}

zx_logger::~zx_logger()
{
    is_running_ = false;
    writing_lv_ = level::unknown;

    write_cv_.notify_all();

    // 将未写的日志写入相对应的文件中
    mutex_.lock();
    for (auto &iter : contain_m_)
    {
        if (iter.second)
        {
            iter.second->write_log(cfg_.block_[iter.first].dst_file_name);
        }
    }
    mutex_.unlock();
    
    if (write_thrd_.joinable())
    {
        write_thrd_.join();
    }
}

void zx_logger::add_message(level lv, std::string &&log)
{
    std::lock_guard<std::recursive_mutex> lck(mutex_);
    level in_lv = lv;
    if (contain_m_.find(in_lv) == contain_m_.end())
    {
        in_lv = level::global;
        if (contain_m_.find(in_lv) == contain_m_.end())
            return;
    }
    if (!cfg_.is_enable_level(in_lv))
        return;
    if (cfg_.block_[in_lv].is_tofile)
    {
        auto size = contain_m_[in_lv]->add_log(std::move(log));
        // 检测是否达到记录最大条数或者默认一个500条的最大限度，达到这个限制后将日志写进文件中
        if (size >= cfg_.block_[in_lv].flush_threshold || size > 500)
        {
            writing_lv_ = in_lv;
            write_cv_.notify_one();
        }
    }
    if (cfg_.block_[in_lv].is_tostdoutput)
    {
        std::cout << log << std::endl;
#ifdef WIN32
        ::OutputDebugStringA(log.c_str());
		::OutputDebugStringA("\n");
#endif
    }
}

void zx_logger::_write_to_file_thrd()
{
    std::mutex tmp_mtx;
    std::unique_lock<std::mutex> wait_lck(tmp_mtx);
    while (is_running_)
    {
        if (writing_lv_ == level::unknown)
        {
            write_cv_.wait_for(wait_lck, std::chrono::milliseconds(thrd_sleep_time_));
            continue;
        }
        container *tmp_ctner = nullptr;
        {
            std::lock_guard<std::recursive_mutex> lck(mutex_);
            if (contain_m_.find(writing_lv_) == contain_m_.end())
                continue;
            tmp_ctner = contain_m_[writing_lv_].get();
        }
        if (tmp_ctner)
        {
            try{
                if(cfg_.check_and_reset_file_name(writing_lv_))
                    tmp_ctner->write_log(cfg_.block_[writing_lv_.load()].dst_file_name);
            }
            catch (...){
                writing_lv_ = level::unknown;
            }
        }
        writing_lv_ = level::unknown;
    }
}

void zx_logger::_load_container_from_config(const configure &cfg)
{
	std::lock_guard<std::recursive_mutex> lck(mutex_);
	for (auto &iter : cfg.block_)
	{
		contain_m_[iter.first].reset(new container(iter.first, iter.second.format));
        if (iter.first == level::global)
        {
            thrd_sleep_time_ = iter.second.millisec_width;
        }
	}
}

void zx_logger::flush_log_level(level lv)
{
    std::lock_guard<std::recursive_mutex> lck(mutex_);
    if (!cfg_.is_enable_level(lv))
        return;
    level in_lv = lv;
    if (contain_m_.find(in_lv) == contain_m_.end())
    {
        in_lv = level::global;
        if (contain_m_.find(in_lv) == contain_m_.end())
            return;
    }
    if (cfg_.block_[in_lv].is_tofile)
    {
        writing_lv_ = in_lv;
        write_cv_.notify_one();
    }
}

void zx_logger::reconfiguration(configure &cfg)
{
    cfg_.rebuild(cfg);
    // 重新构建各个container
	_load_container_from_config(cfg_);
}

configure zx_logger::get_crt_cfg()
{
    return cfg_;
}

///////////////////////////////////////////////////
zxl::base::type::enum_type configure::flags_ = 0;

// default configuration loaded
configure::configure(void)
{
    block_[level::global] = str_cfg();
}

configure::configure(const configure &other)
{
    block_ = other.block_;
    cfg_cur_section_ = other.cfg_cur_section_;
    cfg_section_list_ = other.cfg_section_list_;
}

configure::configure(std::string cfg_file) :configure()
{
    open(cfg_file);
}

configure::~configure()
{
    cfg_section_list_.clear();
    block_.clear();
}

void configure::add_flag(config_flag type)
{
    flags_ |= static_cast<base::type::enum_type>(type);
}

config_flag configure::get_flag()
{
    return static_cast<config_flag>(flags_);
}

void configure::set_max_log_num(int i)
{
    max_log_num_ = i;
}

bool configure::check_and_reset_file_name(level lv)
{
	std::lock_guard<std::recursive_mutex> lck(block_mutex_);
	if (block_.find(lv) == block_.end())
		return false;
	if (block_[lv].dst_file_format.find(base::consts::format_specifier_char) == std::string::npos)
		return true;

    try{
        if (static_cast<base::type::enum_type>(flags_)& static_cast<base::type::enum_type>(config_flag::rolling_by_day))
        {
			tm now;
			time_t time_now = ::time(nullptr);
			::localtime_s(&now, &time_now);
            if (block_[lv].day_of_year != now.tm_yday)
            {
                // 将container中的日志写入文件中
                zxl::zx_logger::instance()->flush_log_level(lv);
                block_[lv].day_of_year = now.tm_yday;
				block_[lv].dst_file_name = format_parser::parse(format_parser(lv, block_[lv].dst_file_format));
            }
        }
        if (static_cast<base::type::enum_type>(flags_)& static_cast<base::type::enum_type>(config_flag::rolling_by_file_size))
        {
            // 判断文件的大小
            std::fstream f(block_[lv].dst_file_name, std::ios::in);
            if (base::utils::files::get_size_of_file(&f) > block_[lv].max_file_size)
            {
                f.close();
                f.open(block_[lv].dst_file_name, std::ios::out | std::ios::trunc);
            }
            f.close();
        }
        if (static_cast<base::type::enum_type>(flags_)& static_cast<base::type::enum_type>(config_flag::current_module_path))
        {
            if (!helpers::is_absolute_path(block_[lv].dst_file_name))
            {
				block_[lv].dst_file_name = helpers::get_module_dir() + block_[lv].dst_file_name;
            }
        }
        return true;
    }
    catch (...)
    {
        std::cout << block_.size() << std::endl;
        return false;
    }
}

bool configure::open(std::string file_name)
{
    std::fstream tmp_file;
    tmp_file.open(file_name, std::ios::in);
    if (!tmp_file.is_open())
        return false;
    std::vector<std::string> tmp_lines;
    std::string tmp_cur_line;
    while (!tmp_file.eof() && tmp_file.good())
    {
        std::getline(tmp_file, tmp_cur_line);
        if (tmp_cur_line.empty())
            continue;
        tmp_lines.push_back(tmp_cur_line);
    }
    for (auto tmp_iter : tmp_lines)
    {
        _parse_cfg_line(tmp_iter);
    }
    tmp_file.close();
    _resort_cfg();
    for (auto &iter : block_)
    {
		iter.second.dst_file_name = format_parser::parse(format_parser(iter.first, iter.second.dst_file_format));
        check_and_reset_file_name(iter.first);
    }
    return true;
}

bool configure::is_level(level lv)
{
    return block_.find(lv) != block_.end();
}

bool configure::is_enable_level(level lv)
{
    if (is_level(lv))
        return block_[lv].is_enable;
    return false;
}

void configure::rebuild(configure &cfg)
{
    max_log_num_ = cfg.max_log_num_;
    cfg_section_list_ = cfg.cfg_section_list_;
    cfg_cur_section_ = cfg.cfg_cur_section_;
    block_ = cfg.block_;
}

void configure::_parse_cfg_line(std::string &line)
{
    if (line.empty())
        return;
    // 去掉左右空格和去掉左右制表符
    helpers::trim_extend(line, " \t");
    // 注释行去掉
    if (line.size() >= 1 && (line[0] == '#' || line[0] == ';'))
        return;
    // 去掉行中的注释
    auto comment_pos = line.find('#');
    if (comment_pos != std::string::npos)
    {
        line.erase(comment_pos);
    }
    comment_pos = line.find(';');
    if (comment_pos != std::string::npos)
    {
        line.erase(comment_pos);
    }

    std::lock_guard<std::recursive_mutex> tmp_lock(cfg_list_lock_);

    auto start_square = line.find('[');
    auto end_square = line.rfind(']');
    if (start_square != std::string::npos && end_square != std::string::npos && start_square == 0)
    {
        if (end_square - start_square > 1)
        {
            std::string tmp_section = line.substr(1, end_square - 1);
            helpers::trim_extend(tmp_section, " \t");
            cfg_cur_section_ = tmp_section;
            cfg_section_list_[tmp_section];
        }
    }
    else
    {
        auto equal_pos = line.find('=');
        if (equal_pos != std::string::npos)
        {
            std::string tmp_key, tmp_value;
            tmp_key = line.substr(0, equal_pos);
            tmp_value = line.substr(equal_pos + 1, line.size());
            std::transform(tmp_key.begin(), tmp_key.end(), tmp_key.begin(), ::toupper);
            helpers::trim_extend(tmp_key, " \t");
            helpers::trim_extend(tmp_value, " \t");
            cfg_section_list_[cfg_cur_section_].insert(std::map<std::string, std::string>::value_type(tmp_key, tmp_value));
        }
    }
}

void configure::_resort_cfg()
{
    std::lock_guard<std::recursive_mutex> tmp_lock(cfg_list_lock_);

    std::lock_guard<std::recursive_mutex> lck(block_mutex_);
    // 先寻找是否有配置global
    level cur_sec_lv = level::global;
    std::string sec_name = helpers::lv_convert_to_str(cur_sec_lv);
    if (cfg_section_list_.find(sec_name) == cfg_section_list_.end())
    {
        block_[cur_sec_lv] = str_cfg();
    }
    else
    { 
        std::map<std::string, std::string> *cur_sec = &cfg_section_list_[sec_name];
        if (cur_sec->find("FORMAT") != cur_sec->end())
            block_[level::global].format = cur_sec->find("FORMAT")->second;
		if (cur_sec->find("FILENAME") != cur_sec->end())
		{
			block_[level::global].dst_file_format = cur_sec->find("FILENAME")->second;
			base::utils::strs::replace_all(block_[level::global].dst_file_format, base::consts::line_specifier, "");
		}
        if (cur_sec->find("ENABLED") != cur_sec->end())
            block_[level::global].is_enable = cur_sec->find("ENABLED")->second == "true" ? true : false;
        if (cur_sec->find("TO_FILE") != cur_sec->end())
            block_[level::global].is_tofile = cur_sec->find("TO_FILE")->second == "true" ? true : false;
        if (cur_sec->find("TO_STANDARD_OUTPUT") != cur_sec->end())
            block_[level::global].is_tostdoutput = cur_sec->find("TO_STANDARD_OUTPUT")->second == "true" ? true : false;
        if (cur_sec->find("MILLISECONDS_WIDTH") != cur_sec->end())
            block_[level::global].millisec_width = std::stoi(cur_sec->find("MILLISECONDS_WIDTH")->second);
        if (cur_sec->find("PERFORMANCE_TRACKING") != cur_sec->end())
            block_[level::global].is_performance = cur_sec->find("PERFORMANCE_TRACKING")->second == "true" ? true : false;
        if (cur_sec->find("MAX_LOG_FILE_SIZE") != cur_sec->end())
            block_[level::global].max_file_size = std::stoi(cur_sec->find("MAX_LOG_FILE_SIZE")->second);
        if (cur_sec->find("LOG_FLUSH_THRESHOLD") != cur_sec->end())
            block_[level::global].flush_threshold = std::stoi(cur_sec->find("LOG_FLUSH_THRESHOLD")->second);
    }

    for (auto &iter : cfg_section_list_)
    {
        if (iter.first == "GLOBAL")
            continue;
        cur_sec_lv = helpers::str_convert_to_lv(iter.first);
        if (cur_sec_lv == level::unknown)
            continue;

        if (iter.second.find("FORMAT") != iter.second.end())
            block_[cur_sec_lv].format = iter.second.find("FORMAT")->second;
        else
            block_[cur_sec_lv].format = block_[level::global].format;
		if (iter.second.find("FILENAME") != iter.second.end())
		{
			block_[cur_sec_lv].dst_file_format = iter.second.find("FILENAME")->second;
			base::utils::strs::replace_all(block_[cur_sec_lv].dst_file_format, base::consts::line_specifier, "");
		}
        else
            block_[cur_sec_lv].dst_file_format = block_[level::global].dst_file_format;
        if (iter.second.find("ENABLED") != iter.second.end())
            block_[cur_sec_lv].is_enable = iter.second.find("ENABLED")->second == "true" ? true : false;
        else
            block_[cur_sec_lv].is_enable = block_[level::global].is_enable;
        if (iter.second.find("TO_FILE") != iter.second.end())
            block_[cur_sec_lv].is_tofile = iter.second.find("TO_FILE")->second == "true" ? true : false;
        else
            block_[cur_sec_lv].is_tofile = block_[level::global].is_tofile;
        if (iter.second.find("TO_STANDARD_OUTPUT") != iter.second.end())
            block_[cur_sec_lv].is_tostdoutput = iter.second.find("TO_STANDARD_OUTPUT")->second == "true" ? true : false;
        else
            block_[cur_sec_lv].is_tostdoutput = block_[level::global].is_tostdoutput;
        if (iter.second.find("MILLISECONDS_WIDTH") != iter.second.end())
            block_[cur_sec_lv].millisec_width = std::stoi(iter.second.find("MILLISECONDS_WIDTH")->second);
        else
            block_[cur_sec_lv].millisec_width = block_[level::global].millisec_width;
        if (iter.second.find("PERFORMANCE_TRACKING") != iter.second.end())
            block_[cur_sec_lv].is_performance = iter.second.find("PERFORMANCE_TRACKING")->second == "true" ? true : false;
        else
            block_[cur_sec_lv].is_performance = block_[level::global].is_performance;
        if (iter.second.find("MAX_LOG_FILE_SIZE") != iter.second.end())
            block_[cur_sec_lv].max_file_size = std::stoi(iter.second.find("MAX_LOG_FILE_SIZE")->second);
        else
            block_[cur_sec_lv].max_file_size = block_[level::global].max_file_size;
        if (iter.second.find("LOG_FLUSH_THRESHOLD") != iter.second.end())
            block_[cur_sec_lv].flush_threshold = std::stoi(iter.second.find("LOG_FLUSH_THRESHOLD")->second);
        else
            block_[cur_sec_lv].flush_threshold = block_[level::global].flush_threshold;
    }
    return;
}

container::container(level lv, std::string format) :lv_(lv), log_format_(format)
{
}

unsigned container::add_log(std::string &&log)
{
    std::lock_guard<std::recursive_mutex> lck(mutex_);
    log_list_.emplace_back(log);
    return log_list_.size(); 
}

void container::write_log(std::string log_name)
{
    if (log_list_.size() <= 0)
        return;
    std::lock_guard<std::recursive_mutex> lck(mutex_);
    log_file_.open(log_name, std::ios::out | std::ios::app);
    if (!log_file_.is_open())
        return;
    std::string log_format{ "" };
    for (auto iter : log_list_)
    {
        if (log_format_ != "" && log_format_.find(zxl::base::consts::format_specifier_char) != std::string::npos)
        {
			log_format = format_parser::parse(format_parser(lv_, log_format_));
            if (log_format.find(zxl::base::consts::line_specifier) != std::string::npos)
            {
                base::utils::strs::replace_all(log_format, base::consts::line_specifier, helpers::get_app_name());
            }
            if (log_format.find(zxl::base::consts::msg_specifier) != std::string::npos)
            {
                base::utils::strs::replace_all(log_format, base::consts::msg_specifier, iter);
            }
            log_file_ << log_format << std::endl;
        }
        else
        {
            log_file_ << iter << std::endl;
        }
    }
    log_list_.clear();
    log_file_.close();
}

writer::writer(level lv) :lv_(lv)
{
    string_t_.str("");
}

writer::~writer()
{
    flush_dispatch(lv_, string_t_.str());
    string_t_.str("");
}

void writer::flush_dispatch(level lv, std::string &&log)
{
    zx_logger::instance()->add_message(lv, std::move(log));
}

std::string helpers::lv_convert_to_str(level lv)
{
    if (lv == level::global) return "GLOBAL";
    if (lv == level::debug) return "DEBUG";
    if (lv == level::info) return "INFO";
    if (lv == level::warning) return "WARNING";
    if (lv == level::error) return "ERROR";
    if (lv == level::fatal) return "FATAL";
    if (lv == level::verbose) return "VERBOSE";
    if (lv == level::trace) return "TRACE";
    return "UNKNOWN";
}

level helpers::str_convert_to_lv(std::string str)
{
    if (str == "GLOBAL") return level::global;
    if (str == "DEBUG") return level::debug;
    if (str == "INFO") return level::info;
    if (str == "WARNING") return level::warning;
    if (str == "ERROR") return level::error;
    if (str == "FATAL") return level::fatal;
    if (str == "VERBOSE") return level::verbose;
    if (str == "TRACE") return level::trace;
    return level::unknown;
}

std::string &helpers::trim_extend(std::string &line, const std::string rid_s)
{
    if (line.empty())
    {
        return line;
    }

    line.erase(0, line.find_first_not_of(rid_s));
    line.erase(line.find_last_not_of(rid_s) + 1);
    return line;
}

std::string &helpers::to_upper(std::string &str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

std::string helpers::get_module_dir(const std::string& file_fullpath)
{
    std::string out_path;
#ifdef WIN32
    char tmp_path[260] = { 0 };
    char tmp_drive[260] = { 0 };
    char tmp_dir[260] = { 0 };
    char tmp_app_name[260] = { 0 };
    char tmp_ext[260] = { 0 };
    if (file_fullpath == "")
    {
        MEMORY_BASIC_INFORMATION mbi;
		GetModuleFileNameA(((VirtualQuery(get_module_dir, &mbi, sizeof(mbi)) != 0) ? (HMODULE)mbi.AllocationBase : NULL), tmp_path, 260);
        _splitpath_s(tmp_path, tmp_drive, sizeof(tmp_drive), tmp_dir, sizeof(tmp_dir), tmp_app_name, sizeof(tmp_app_name), tmp_ext, sizeof(tmp_ext));
    }
    else
    {
        _splitpath_s(file_fullpath.c_str(), tmp_drive, sizeof(tmp_drive), tmp_dir, sizeof(tmp_dir), tmp_app_name, sizeof(tmp_app_name), tmp_ext, sizeof(tmp_ext));
    }
    out_path.append(tmp_drive);
    out_path.append(tmp_dir);
#else
    char tmp_dir[260] = { 0 };
    if (!getcwd(tmp_dir, 260))
    {
        return false;
    }
    out_path = tmp_dir;
    out_path.append("/");
#endif
    return out_path;
}

std::string helpers::get_app_name()
{
    std::string out_name;
#ifdef WIN32
    char tmp_path[260] = { 0 };
    char tmp_drive[260] = { 0 };
    char tmp_dir[260] = { 0 };
    char tmp_app_name[260] = { 0 };
    char tmp_ext[260] = { 0 };
    GetModuleFileNameA(nullptr, tmp_path, 260);
    _splitpath_s(tmp_path, tmp_drive, sizeof(tmp_drive), tmp_dir, sizeof(tmp_dir), tmp_app_name, sizeof(tmp_app_name), tmp_ext, sizeof(tmp_ext));
    out_name.append(tmp_app_name);
#else
#endif
    return out_name;
}

bool helpers::is_absolute_path(std::string path)
{
    char tmp_drive[260] = { 0 };
    char tmp_dir[260] = { 0 };
    char tmp_app_name[260] = { 0 };
    char tmp_ext[260] = { 0 };
    _splitpath_s(path.data(), tmp_drive, sizeof(tmp_drive), tmp_dir, sizeof(tmp_dir), tmp_app_name, sizeof(tmp_app_name), tmp_ext, sizeof(tmp_ext));
    if (strlen(tmp_drive) > 0)
        return true;
    return false;
}

format_parser::format_parser(level lv, std::string user_format) :lv_(lv), flags_(0)
{
    user_format_ = user_format;
    _parse_format();
}

std::string format_parser::parse(format_parser &&format, bool is_parse_all)
{
    // 是否存在格式化字符%
    if (format.log_format_.empty())
        return std::string();
    if (format.log_format_.find(base::consts::format_specifier_char) == std::string::npos)
        return format.log_format_;
    if (format._has_flag(format_flag::ff_datatime))
    {
        base::utils::strs::replace_all(format.log_format_, base::consts::date_specifier, base::utils::date_time::get_datetime(format.date_format_.c_str()));
    }
	if (format._has_flag(format_flag::ff_level))
	{
		base::utils::strs::replace_all(format.log_format_, base::consts::level_specifier, helpers::lv_convert_to_str(format.lv_));
	}
	if (format._has_flag(format_flag::ff_location))
	{
		base::utils::strs::replace_all(format.log_format_, base::consts::location_specifier, helpers::get_module_dir());
	}
	if (format._has_flag(format_flag::ff_app))
	{
		base::utils::strs::replace_all(format.log_format_, base::consts::app_specifier, helpers::get_app_name());
	}
    return format.log_format_;
}

void format_parser::_parse_format()
{
    if (user_format_.empty())
        return;
    log_format_ = user_format_;
    if (log_format_.find(base::consts::level_specifier) != std::string::npos)
    {
        _add_flag(zxl::format_flag::ff_level);
        base::utils::strs::replace_all(log_format_, base::consts::level_specifier, helpers::lv_convert_to_str(lv_));
    }
    auto found_at = log_format_.find(base::consts::date_specifier);
    if (found_at != std::string::npos)
    {
        _add_flag(zxl::format_flag::ff_datatime);
        auto start = log_format_.find('{', found_at);
        if (start != std::string::npos)
        {
            auto end = log_format_.find('}', start);
            if (end != std::string::npos)
            {
                date_format_ = log_format_.substr(start+1, end - start-1);
                log_format_.erase(start, end - start+1);
            }
        }
    }
    if (log_format_.find(base::consts::line_specifier) != std::string::npos)
    {
        _add_flag(zxl::format_flag::ff_line);
    }
    if (log_format_.find(base::consts::msg_specifier) != std::string::npos)
    {
        _add_flag(zxl::format_flag::ff_message);
    }
    if (log_format_.find(base::consts::app_specifier) != std::string::npos)
    {
        _add_flag(zxl::format_flag::ff_app);
        std::string app_name = helpers::get_app_name();
        base::utils::strs::replace_all(log_format_, base::consts::app_specifier, app_name);
    }
    if (log_format_.find(base::consts::location_specifier) != std::string::npos)
    {
        _add_flag(zxl::format_flag::ff_location);
		std::string location = helpers::get_module_dir();
        base::utils::strs::replace_all(log_format_, base::consts::location_specifier, location);
		location = helpers::get_module_dir(log_format_);
        if (!base::utils::files::path_exists(location.c_str()))
        {
            base::utils::files::create_path(location);
        }
    }
}

bool format_parser::_has_flag(format_flag f)
{
    return (flags_ & static_cast<int>(f));
}

void format_parser::_add_flag(format_flag f)
{
    flags_ |= static_cast<int>(f);
}


#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
std::fstream* base::utils::files::new_file_stream(const std::string& filename)
{
    std::fstream* fs = new std::fstream(filename.c_str(), std::fstream::out | std::fstream::app);
    if (fs->is_open())
        fs->flush();
    else
		base::utils::safe_delete(fs);
    return fs;
}

std::size_t base::utils::files::get_size_of_file(std::fstream* fs)
{
    if (fs == nullptr) {
        return 0;
    }
    std::streampos cur_pos = fs->tellg();
    fs->seekg(0, fs->end);
    std::size_t size = static_cast<std::size_t>(fs->tellg());
    fs->seekg(cur_pos);
    return size;
}

inline bool base::utils::files::path_exists(const char* path, bool consider_file)
{
    if (path == nullptr)
    {
        return false;
    }
    DWORD file_type = GetFileAttributesA(path);
    if (file_type == INVALID_FILE_ATTRIBUTES) {
        return false;
    }
    return consider_file ? true : ((file_type & FILE_ATTRIBUTE_DIRECTORY) == 0 ? false : true);
}

bool base::utils::files::create_path(const std::string& path)
{
    if (path.empty()) {
        return false;
    }
    if (base::utils::files::path_exists(path.c_str())) {
        return true;
    }
    int status = -1;

    char* cur_path = const_cast<char*>(path.c_str());
    std::string built_path = std::string();

    char* next_tok = nullptr;
    cur_path = strtok_s(cur_path, base::consts::file_path_seperator, &next_tok);
    while (cur_path != nullptr) {
        built_path.append(cur_path);
        built_path.append(base::consts::file_path_seperator);
        status = _mkdir(built_path.c_str());
        cur_path = strtok_s(nullptr, base::consts::file_path_seperator, &next_tok);
    }
    if (status == -1) {
        return false;
    }
    return true;
}

std::string base::utils::files::extract_path_from_filename(const std::string& full_path, const char* seperator)
{
    if ((full_path == "") || (full_path.find(seperator) == std::string::npos)) {
        return full_path;
    }
    std::size_t last_slash_at = full_path.find_last_of(seperator);
    if (last_slash_at == 0) {
        return std::string(seperator);
    }
    return full_path.substr(0, last_slash_at + 1);
}

void base::utils::files::build_stripped_filename(const char* filename, char buff[], std::size_t limit)
{
    std::size_t sizeof_filename = strlen(filename);
    if (sizeof_filename >= limit) {
        filename += (sizeof_filename - limit);
        if (filename[0] != '.' && filename[1] != '.') {  // prepend if not already
            filename += 3;  // 3 = '..'
            strcat_s(buff, limit, "..");
        }
    }
    strcat_s(buff, limit, filename);
}

void base::utils::files::build_base_filename(const std::string& full_path, char buff[], std::size_t limit, const char* seperator)
{
    const char *filename = full_path.c_str();
    std::size_t last_slash_at = full_path.find_last_of(seperator);
    filename += last_slash_at ? last_slash_at + 1 : 0;
    std::size_t sizeof_filename = strlen(filename);
    if (sizeof_filename >= limit) {
        filename += (sizeof_filename - limit);
        if (filename[0] != '.' && filename[1] != '.') {  // prepend if not already
            filename += 3;  // 3 = '..'
            strcat_s(buff, limit, "..");
        }
    }
    strcat_s(buff, limit, filename);
}

bool base::utils::strs::is_digit(char c)
{
    return c >= '0' && c <= '9';
}

bool base::utils::strs::wild_card_match(const char* str, const char* pattern) {
    while (*pattern) {
        switch (*pattern) {
        case '?':
            if (!*str)
                return false;
            ++str;
            ++pattern;
            break;
        case '*':
            if (wild_card_match(str, pattern + 1))
                return true;
            if (*str && wild_card_match(str + 1, pattern))
                return true;
            return false;
            break;
        default:
            if (*str++ != *pattern++)
                return false;
            break;
        }
    }
    return !*str && !*pattern;
}

std::string& base::utils::strs::ltrim(std::string& str) {
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), std::not1(std::ptr_fun<int, int>(&std::isspace))));
    return str;
}

std::string& base::utils::strs::rtrim(std::string& str) {
    str.erase(std::find_if(str.rbegin(), str.rend(), std::not1(std::ptr_fun<int, int>(&std::isspace))).base(), str.end());
    return str;
}

std::string& base::utils::strs::trim(std::string& str) {
    return ltrim(rtrim(str));
}

bool base::utils::strs::starts_with(const std::string& str, const std::string& start) {
    return (str.length() >= start.length()) && (str.compare(0, start.length(), start) == 0);
}

bool base::utils::strs::ends_with(const std::string& str, const std::string& end) {
    return (str.length() >= end.length()) && (str.compare(str.length() - end.length(), end.length(), end) == 0);
}

std::string& base::utils::strs::replace_all(std::string& str, char replace_what, char replace_with) {
    std::replace(str.begin(), str.end(), replace_what, replace_with);
    return str;
}

std::string& base::utils::strs::replace_all(std::string& str, const std::string& replace_what, // NOLINT
    const std::string& replace_with) {
    if (replace_what == replace_with)
        return str;
    std::size_t foundAt = std::string::npos;
    while ((foundAt = str.find(replace_what, foundAt + 1)) != std::string::npos) {
        str.replace(foundAt, replace_what.length(), replace_with);
    }
    return str;
}

void base::utils::strs::replace_first_with_escape(std::string& str, const std::string& replace_what, // NOLINT
    const std::string& replace_with) {
    std::size_t foundAt = std::string::npos;
    while ((foundAt = str.find(replace_what, foundAt + 1)) != std::string::npos) {
        if (foundAt > 0 && str[foundAt - 1] == base::consts::format_specifier_char) {
            str.erase(foundAt > 0 ? foundAt - 1 : 0, 1);
            ++foundAt;
        }
        else {
            str.replace(foundAt, replace_what.length(), replace_with);
            return;
        }
    }
}

std::string& base::utils::strs::to_upper(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

bool base::utils::strs::cstring_eq(const char* s1, const char* s2) {
    if (s1 == nullptr && s2 == nullptr) return true;
    if (s1 == nullptr || s2 == nullptr) return false;
    return strcmp(s1, s2) == 0;
}

bool base::utils::strs::cstring_case_eq(const char* s1, const char* s2) {
    if (s1 == nullptr && s2 == nullptr) return true;
    if (s1 == nullptr || s2 == nullptr) return false;
    if (strlen(s1) != strlen(s2)) return false;
    while (*s1 != '\0' && *s2 != '\0') {
        if (::toupper(*s1) != ::toupper(*s2)) return false;
        ++s1;
        ++s2;
    }
    return true;
}

bool base::utils::strs::contains(const char* str, char c) {
    for (; *str; ++str) {
        if (*str == c)
            return true;
    }
    return false;
}

char* base::utils::strs::convert_and_add_to_buff(std::size_t n, int len, char* buf, const char* bufLim, bool zeroPadded) {
    char localBuff[10] = "";
    char* p = localBuff + sizeof(localBuff) - 2;
    if (n > 0) {
        for (; n > 0 && p > localBuff && len > 0; n /= 10, --len)
            *--p = static_cast<char>(n % 10 + '0');
    }
    else {
        *--p = '0';
        --len;
    }
    if (zeroPadded)
        while (p > localBuff && len-- > 0) *--p = static_cast<char>('0');
    return add_to_buff(p, buf, bufLim);
}

char* base::utils::strs::add_to_buff(const char* str, char* buf, const char* bufLim) {
    while ((buf < bufLim) && ((*buf = *str++) != '\0'))
        ++buf;
    return buf;
}

char* base::utils::strs::clear_buff(char buff[], std::size_t lim) {
    memcpy(buff, "", lim);
    return buff;
}

char* base::utils::strs::wcharptr_to_charptr(const wchar_t* line) {
    std::size_t len_ = wcslen(line) + 1;
    char* buff_ = static_cast<char*>(malloc(len_ + 1));
    std::size_t convCount_ = 0;
    mbstate_t mbState_;
    ::memset(static_cast<void*>(&mbState_), 0, sizeof(mbState_));
    wcsrtombs_s(&convCount_, buff_, len_, &line, len_, &mbState_);
    return buff_;
}

void base::utils::date_time::get_time_of_day(struct timeval* tv)
{
    if (tv != nullptr) {
        const unsigned __int64 delta_ = 11644473600000000Ui64;
        const double sec_offset = 0.000001;
        const unsigned long usec_offset = 1000000;
        FILETIME file_time;
        GetSystemTimeAsFileTime(&file_time);
        unsigned __int64 present = 0;
        present |= file_time.dwHighDateTime;
        present = present << 32;
        present |= file_time.dwLowDateTime;
        present /= 10;  // mic-sec
        // Subtract the difference
        present -= delta_;
        tv->tv_sec = static_cast<long>(present * sec_offset);
        tv->tv_usec = static_cast<long>(present % usec_offset);
    }
}

std::string base::utils::date_time::get_datetime(const char* format)
{
    struct timeval cur_time;
    get_time_of_day(&cur_time);
    struct ::tm timeInfo;
    _build_timeinfo(&cur_time, &timeInfo);
    const int buff_size = 30;
    char buff_[buff_size] = "";
    _parse_format(buff_, buff_size, format, &timeInfo, static_cast<std::size_t>(cur_time.tv_usec / 1000));
    return std::string(buff_);
}

std::string base::utils::date_time::format_time(unsigned long long time, timestamp_unit timestamp)
{
    double result = static_cast<double>(time);
    base::type::enum_type start = static_cast<base::type::enum_type>(timestamp);
    const char* unit = base::consts::time_formats[start].unit;
    for (base::type::enum_type i = start; i < base::consts::time_formats_count - 1; ++i) {
        if (result <= base::consts::time_formats[i].value) {
            break;
        }
        result /= base::consts::time_formats[i].value;
        unit = base::consts::time_formats[i + 1].unit;
    }
    std::stringstream ss;
    ss << result << " " << unit;
    return ss.str();
}

unsigned long long base::utils::date_time::get_time_diff(const struct timeval& end, const struct timeval& start, timestamp_unit timestamp)
{
    if (timestamp == timestamp_unit::Microsecond) {
        return static_cast<unsigned long long>(static_cast<unsigned long long>(1000000 * end.tv_sec + end.tv_usec) -
            static_cast<unsigned long long>(1000000 * start.tv_sec + start.tv_usec));
    }
    else {
        return static_cast<unsigned long long>((((end.tv_sec - start.tv_sec) * 1000000) + (end.tv_usec - start.tv_usec)) / 1000);
    }
}

struct ::tm* base::utils::date_time::_build_timeinfo(struct timeval* cur_time, struct ::tm* timeinfo)
{
    time_t t;
    _time64(&t);
    localtime_s(timeinfo, &t);
    return timeinfo;
}

char* base::utils::date_time::_parse_format(char* buf, std::size_t len, const char* format, const struct tm* info, std::size_t msec)
{
    const char* buf_lim = buf + len;
    for (; *format; ++format) 
    {
        if (*format == base::consts::format_specifier_char) 
        {
            switch (*++format) 
            {
            case base::consts::format_specifier_char:  // Escape
                break;
            case '\0':  // End
                --format;
                break;
            case 'd':  // Day
				buf = base::utils::strs::convert_and_add_to_buff(info->tm_mday, 2, buf, buf_lim);
                continue;
            case 'a':  // Day of week (short)
				buf = base::utils::strs::add_to_buff(base::consts::days_abbrev[info->tm_wday], buf, buf_lim);
                continue;
            case 'A':  // Day of week (long)
				buf = base::utils::strs::add_to_buff(base::consts::days[info->tm_wday], buf, buf_lim);
                continue;
            case 'M':  // month
				buf = base::utils::strs::convert_and_add_to_buff(info->tm_mon + 1, 2, buf, buf_lim);
                continue;
            case 'b':  // month (short)
				buf = base::utils::strs::add_to_buff(base::consts::months_abbrev[info->tm_mon], buf, buf_lim);
                continue;
            case 'B':  // month (long)
				buf = base::utils::strs::add_to_buff(base::consts::months[info->tm_mon], buf, buf_lim);
                continue;
            case 'y':  // year (two digits)
				buf = base::utils::strs::convert_and_add_to_buff(info->tm_year + base::consts::year_base, 2, buf, buf_lim);
                continue;
            case 'Y':  // year (four digits)
				buf = base::utils::strs::convert_and_add_to_buff(info->tm_year + base::consts::year_base, 4, buf, buf_lim);
                continue;
            case 'h':  // hour (12-hour)
				buf = base::utils::strs::convert_and_add_to_buff(info->tm_hour % 12, 2, buf, buf_lim);
                continue;
            case 'H':  // hour (24-hour)
				buf = base::utils::strs::convert_and_add_to_buff(info->tm_hour, 2, buf, buf_lim);
                continue;
            case 'm':  // minute
				buf = base::utils::strs::convert_and_add_to_buff(info->tm_min, 2, buf, buf_lim);
                continue;
            case 's':  // second
				buf = base::utils::strs::convert_and_add_to_buff(info->tm_sec, 2, buf, buf_lim);
                continue;
            case 'z':  // milliseconds
            case 'g':
				buf = base::utils::strs::convert_and_add_to_buff(msec, 3, buf, buf_lim);
                continue;
            case 'F':  // AM/PM
				buf = base::utils::strs::add_to_buff((info->tm_hour >= 12) ? base::consts::kpm : base::consts::kam, buf, buf_lim);
                continue;
            default:
                continue;
            }
        }
		if (buf == buf_lim)
            break;
        *buf++ = *format;
    }
    return buf;
}