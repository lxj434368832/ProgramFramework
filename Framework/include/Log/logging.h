#ifndef ZX_LOGGING_H_
#define ZX_LOGGING_H_
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <mutex>
#include <list>
#include <map>
#include <condition_variable>
#include <atomic>
#include <memory>

#ifdef _WINDLL
#ifdef ZX_LOGGING_EXPORTS
#define ZX_LOGGING_API __declspec(dllexport)
#else
#define ZX_LOGGING_API __declspec(dllimport)
#endif
#else
#define ZX_LOGGING_API
#endif

static const char *default_log_name = "default.log";

namespace zxl
{
    enum class level;
    enum class config_flag;
    class zx_logger;
    class writer;
    class configure;
    class container;
    class helpers;
    class format_parser;
    
    namespace base
    {
        class noncopyable;
        class nonmovable;
        class static_class;

        namespace type
        {
            typedef int enum_type;
        }

        namespace utils
        {
            class files;
            class strings;
            class date_time;
        }
    }
}

namespace zxl
{
    enum class level : base::type::enum_type
    {
        global,
        info,
        debug,
        trace,
        fatal,
        error,
        warning,
        verbose,
        unknown,
		level_num
    };

    // @bref flags need to be valued by bit
    enum class config_flag : base::type::enum_type
    {
        rolling_by_day = 1,
        rolling_by_file_size = 2,
        current_module_path = 4,        // 如果是相对路径，该参数说明是否记录日志在本模块地址，如果没有设置则记录在调用模块路径下
        type_err,
    };

	struct str_cfg
	{
		std::string format;
		std::string dst_file_format;
		bool        is_enable;
		bool        is_tofile;
		bool        is_tostdoutput;
		unsigned    millisec_width;
		bool        is_performance;
		long long   max_file_size;
		unsigned    flush_threshold;

		std::string dst_file_name;
		int         day_of_year;

		str_cfg(){
			format = "";
			dst_file_format = default_log_name;
			dst_file_name = default_log_name;
			is_enable = true;
			is_tofile = false;
			is_tostdoutput = true;
			millisec_width = 50;
			is_performance = true;
			max_file_size = 2097152;
			flush_threshold = 1;
			day_of_year = 0;
		}
	};

    class base::noncopyable
    {
    public:
        noncopyable(const noncopyable&) = delete;
        noncopyable& operator=(const noncopyable&) = delete;
        noncopyable(){};
    };

    class base::nonmovable
    {
    public:
        nonmovable(nonmovable&&) = delete;
        nonmovable& operator=(nonmovable&&) = delete;
        nonmovable(){};
    };

    class base::static_class
    {
    private:
        static_class();
        static_class(const static_class &);
        static_class& operator=(const static_class&);
        static_class& operator=(static_class&&);
    };

    class writer : public base::noncopyable
    {
        friend zx_logger;
    public:
        writer(level lv);
        ~writer();

    public:
        template <typename T>
        inline writer& operator<<(const T& log) 
        {
            string_t_ << log;
            return *this;
        }
        inline writer& operator<<(std::ostream& (*log)(std::ostream&)) 
        {
            string_t_ << log;
            return *this;
        }

	public:
		void flush_dispatch(level lv,std::string &&log);

    private:
        level lv_;
        std::stringstream string_t_;
    };

    class configure
    {
        friend zx_logger;
    public:
        configure(void );
        configure(const configure &);
        configure(std::string cfg_file);
        ~configure();
    private:
        static zxl::base::type::enum_type flags_;
        int max_log_num_;

        std::map<std::string, std::map<std::string, std::string>> cfg_section_list_;
        std::string                     cfg_cur_section_;
        std::recursive_mutex            cfg_list_lock_;

        std::recursive_mutex            block_mutex_;
        std::map<level, str_cfg>        block_;
    public:
        // @bref set log-writing rule flag
        static void add_flag(config_flag flag);
        // @bref get log-writing rule flag
        static config_flag get_flag();
        // @bref set log max number
        void set_max_log_num(int i);
        // @bref if error occured,return false
        bool check_and_reset_file_name(level lv);
        // @bref open a reading configuration file
        bool open(std::string cfg_file);
        // @bref check whether log of level is exist
        bool is_level(level lv);
        // @bref check whether log of level is enable
        bool is_enable_level(level lv);
        // @bref rebuild this configure object
        void rebuild(configure &cfg);
    private:
        void _parse_cfg_line(std::string &line);
        void _resort_cfg();
    };

    class container : public base::noncopyable, base::nonmovable
    {
        friend zx_logger;
    public:
        container(level lv, std::string format = "");
        ~container() = default;
    private:
        level lv_;
        std::string log_format_;
        std::recursive_mutex mutex_;
        std::list<std::string> log_list_;
        std::fstream log_file_;

    public:
        // @bref add log string to log_list of this container
        unsigned add_log(std::string &&log);
        // @bref write log string in log_list to file(log_name file)
        void write_log(std::string log_name);
    };

    class helpers
    {
    public:
        // @bref convert level to string
        static std::string lv_convert_to_str(level lv);
        // @bref convert string to level
        static level str_convert_to_lv(std::string str);

        // @bref remove @param(rid_s) in the string
        static std::string &trim_extend(std::string &line, const std::string rid_s);
        // @bref change the lower characters to upper characters
        static std::string &to_upper(std::string &str);

        // @bref if $file_fullpath is null,this function gets self module handle
        static std::string get_module_dir(const std::string& file_fullpath = "");
        // @bref get the calling-app's name
        static std::string get_app_name();
        // @bref check whether the @param(path) is correct 
        static bool is_absolute_path(std::string path);
    };

    enum class format_flag
    {
        ff_datatime = 1,
        ff_level = 2,
        ff_line = 4,
        ff_app = 8,
        ff_location = 16,
        ff_message = 32,
    };

    class format_parser
    {
    private:
        int flags_;
        level lv_;
        std::string user_format_;
        std::string log_format_;
        std::string date_format_;
    public:
        format_parser(level lv, std::string user_format);
		// @bref if is_parse_all=false,just parse some parts necessary.
		static std::string parse(format_parser &&format, bool is_parse_all = false);

    private:
        void _parse_format();
        inline bool _has_flag(format_flag f);
		inline void _add_flag(format_flag f);
    };

    class zx_logger : public base::noncopyable
    {
    public:
        static zx_logger* instance();
		static void delete_instance();
        ~zx_logger();
    private:
        zx_logger();
        static zx_logger* s_instance_;
    public:
        // @bref add the @param(log) of @param(level) to the corresponding container
        void add_message(level lv, std::string &&log);
        // @bref the same as the function above
        //void add_message(level lv, std::string &log);
        // @bref flush the logs of @param(lv)
        void flush_log_level(level lv);

        // @bref ####most important,reload configuration{@param(cfg)}
        void reconfiguration(configure &cfg);
        // @bref get current config
        inline configure get_crt_cfg();
    protected:
        // @bref write-log thread function
        void _write_to_file_thrd();

	private:
		void _load_container_from_config(const configure &cfg);

    private:
        bool is_running_;
        std::thread write_thrd_;
        std::condition_variable write_cv_;              // notify the thread to write

        configure cfg_;
        std::recursive_mutex mutex_;
        std::map<level, std::unique_ptr<container>> contain_m_;
        std::atomic<level> writing_lv_ = level::unknown;
        int thrd_sleep_time_ = 50;
    };
}//end zxl

#include <ctime>
#include <algorithm>
#include <cctype>
#ifdef WIN32
#include <Windows.h>
#endif
namespace zxl{
    namespace base{
        namespace consts{
            static const char format_specifier_char = '%';
            static const char *level_specifier = "%level";
            static const char *date_specifier = "%datetime";
            static const char *line_specifier = "%line";
            static const char *msg_specifier = "%msg";
            static const char *app_specifier = "%app";              // 当前调用模块
            static const char *location_specifier = "%location/";
            static const char *file_path_seperator = "\\";
            static const std::size_t source_filename_max_len = 100;

            const struct {
                double value;
                const char* unit;
            } time_formats[] = {
                { 1000.0f, "mis" },
                { 1000.0f, "ms"},
                { 60.0f, "seconds" },
                { 60.0f, "minutes" },
                { 24.0f, "hours" },
                { 7.0f, "days" }
            };
            static const int time_formats_count = sizeof(time_formats) / sizeof(time_formats[0]);

            static const char* days[7] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
            static const char* days_abbrev[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
            static const char* months[12] = { "January", "February", "March", "Apri", "May", "June", "July", "August",
                "September", "October", "November", "December" };
            static const char* months_abbrev[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
            static const char* default_datetime_format = "%Y-%M-%d %H:%m:%s,%g";
            static const char* default_datetime_format_in_filename = "%Y-%M-%d_%H-%m";
            static const int year_base = 1900;
            static const char* kam = "AM";
            static const char* kpm = "PM";
        }
        namespace utils{
            enum class timestamp_unit : base::type::enum_type 
            {
                Microsecond = 0,
                Millisecond = 1, 
                Second = 2, 
                Minute = 3, 
                Hour = 4, 
                Day = 5
			};

			template <typename T>
			static inline
				typename std::enable_if<std::is_pointer<T*>::value, void>::type
				safe_delete(T*& pointer) {
				if (pointer == nullptr)
					return;
				delete pointer;
				pointer = nullptr;
			}

			class files : public static_class
            {
            public:
                static std::fstream* new_file_stream(const std::string& filename);
                static std::size_t get_size_of_file(std::fstream* fs);
                static inline bool path_exists(const char* path, bool consider_file = false);
                static bool create_path(const std::string& path);
                static std::string extract_path_from_filename(const std::string& full_path, const char* seperator = base::consts::file_path_seperator);
                static void build_stripped_filename(const char* filename, char buff[],
                    std::size_t limit = base::consts::source_filename_max_len);
                static void build_base_filename(const std::string& full_path, char buff[],
                    std::size_t limit = base::consts::source_filename_max_len,
                    const char* seperator = base::consts::file_path_seperator);
            };

            class strings : public static_class
            {
            public:
                static inline bool is_digit(char c);
                static bool wild_card_match(const char* str, const char* pattern);
				// @bref remove the left spaces in the string
                static inline std::string& ltrim(std::string& str);
				// @bref remove the right spaces in the string
                static inline std::string& rtrim(std::string& str);
				// @bref remove the left-right spaces in the string
                static inline std::string& trim(std::string& str);
                static inline bool starts_with(const std::string& str, const std::string& start);
                static inline bool ends_with(const std::string& str, const std::string& end);
                static inline std::string& replace_all(std::string& str, char replace_what, char replace_with);
                static inline std::string& replace_all(std::string& str, const std::string& replace_what, // NOLINT
                    const std::string& replace_with);
                static void replace_first_with_escape(std::string& str, const std::string& replace_what, // NOLINT
                    const std::string& replace_with);
                static inline std::string& to_upper(std::string& str);
                static inline bool cstring_eq(const char* s1, const char* s2);
                static bool cstring_case_eq(const char* s1, const char* s2);
                static inline bool contains(const char* str, char c);
                static inline char* convert_and_add_to_buff(std::size_t n, int len, char* buf, const char* bufLim, bool zeroPadded = true);
                static inline char* add_to_buff(const char* str, char* buf, const char* bufLim);
                static inline char* clear_buff(char buff[], std::size_t lim);
                static char* wcharptr_to_charptr(const wchar_t* line);
            };

            class date_time : public static_class
            {
            public:
                static void get_time_of_day(struct timeval* tv);
                static inline std::string get_datetime(const char* format);
                static std::string format_time(unsigned long long time, timestamp_unit timestamp);
                static inline unsigned long long get_time_diff(const struct timeval& end, const struct timeval& start, timestamp_unit timestamp);
            private:
                static inline struct ::tm* _build_timeinfo(struct timeval* cur_time, struct ::tm* timeinfo);
                static char* _parse_format(char* buf, std::size_t len, const char* format, const struct tm* info, std::size_t msec);
            };
        }
    }
}

#define Log(LEVEL) (zxl::writer(LEVEL))
#define LogInfo()   Log(zxl::level::info)
#define LogDebug()  Log(zxl::level::debug)
#define LogTrace()  Log(zxl::level::trace)
#define LogFatal()  Log(zxl::level::fatal)
#define LogErr()    Log(zxl::level::error)
#define LogWarn()   Log(zxl::level::warning)

#define logm() LogInfo()<<"["<<__FUNCTION__<<"]|<message> "
#define logw() LogInfo()<<"["<<__FUNCTION__<<"]|<warn> "
#define loge() LogInfo()<<"["<<__FUNCTION__<<"]|<error> "
#define logd() LogInfo()<<"["<<__FUNCTION__<<"]|<debug> "
#define logt() LogInfo()<<"["<<__FUNCTION__<<"]|<trace> "

extern zxl::zx_logger *g_logger;
#endif