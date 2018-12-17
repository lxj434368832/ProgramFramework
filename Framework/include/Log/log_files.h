#pragma once

#include <windows.h>
#include <mutex>

#define MAX_LOG_BUF_LEN 0x2000

class log_files
{
private:
	std::recursive_mutex m_file_lock;
	FILE *	m_fp;
	long	m_file_length;
	char *	m_file_buffer;
	char	m_mode[0x10];
	char	m_file_name[MAX_PATH];
	char	m_printf_buf[MAX_LOG_BUF_LEN];		// printf缓冲
	int		m_buf_len;							// printf缓冲中数据长度

public:
	log_files(const char * file_name, const char * mode, int name_type = 0);
	~log_files();

	log_files(log_files const&) = delete;
	log_files& operator = (log_files const&) = delete;

public:
	void  dbg_print_to_file(char * ctext, ...);
	void  read_file_buff_len();
	char *get_file_buff();
	long get_file_size();                                           // 获取文件大小
	void  dump_data(BYTE *buffer, int length, char *type);
	void  log_printf(char *ctext, ...);							    // 当写缓冲区满时再写入文件
	long file_read_string(char *file_name, std::string &out_buf);   // 读取文件到out_buf
	char *file_read(char *file_name, char *out_buf, int &len);      // 读取文件到out_buf
	static bool file_save(char *file_name, char *fbuf, int len);           // 保存数据到文件
	static bool save_mem_log(char * f_name, char * sstr, char * src, int len); // 保存数据到日志
	static bool save_log(char * f_name, char * ctext, ...);                    // 保存文本到日志
};
