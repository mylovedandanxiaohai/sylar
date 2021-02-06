#ifndef _SYLAR_LOG_H
#define _SYLAR_LOG_H

#include <string>
#include <stdint.h>
#include <memory>
#include <stringstreams>
#include <fstream>
namespace Mylove
{
    //日志事件
    class LogEvent
    {
        public: 
            typedef std::shared_ptr<LogEvent> ptr;//添加只能指针,便于内存管理
            LogEvent();

            const char* getFile() const {return m_file;}
            int32_t getline() const {return m_line;}
            uint32_t getElapse() const {return m_elapse;}
            uint32_t getThreadId() const {return m_threadId;}
            uint32_t getFiberId() const {return m_fibrerId;}
            uint64_t getTime() const {return m_time;}
            const std::string& getCount() const {return m_count;}`
        private:
            const char* m_file=nullptr; //文件名
            uint32_t m_line=0;          //行号
            uint32_t m_elapse=0;        //从启动开始到现在的毫秒数
            uint32_t m_threadId=0;      //线程ID
            uint32_t m_fiberId=0;        //协程ID
            uint64_t m_time;            //时间戳
            std::string m_count;
    };
    //日志等级
    class LogLevel
    {
        public:
            enum Level
            {
                UNKONW=0,
                DEBUG=1,
                INFO=2,
                WARN=3,
                ERROR=4,
                FATAL=5;
            };
            static const char* ToString(LogLevel::Level level);
};
    //日志器
    class Logger//日志器
    {
        public:
            typedef std::shared_ptr<Logger> ptr;

            Logger(const std::string& name="root");
            void log(LogLevel level,const LogEvent& event);
            
            void debug(LogEvent::ptr event);
            void info(LogEvent::ptr event);
            void warn(LogEvent::ptr event);
            void error(LogEvent::ptr event);
            void fatal(LogEvent::ptr event);

            void addAppender(LogAppender::ptr appender);
            void delAppender(LogAppender::ptr appender);
            LogLevel::Level getLevel() const{return m_level;}
            void setLevel(LogLevel::Level val){m_level=val;}
        private:
            std::string m_name;                             //日志名称
            LogLevel::Level m_level;                        //日志级别
            std::list<LogAppender::ptr> m_appender;         //Appender集合
    };
    //日志格式器
    class LogFormatter//日志的格式
    {
        public:
        typedef std::shared_ptr<LogFormatter> ptr;
        LogFormatter(const std::string& pattern);
        //
        std::string format(LogLevel::Level level,LogEvent::ptr event);
        privete:
            class FormatterItem
            {
                public:
                    typedef std::shared_ptr<FormatItem> ptr;
                    virtual ~Formatter(){}
                    virtual void format(std::ostream& os,LogLevel::Level level,LogEvent::ptr event) = 0;
             };

            void init();
        private:
            std::string m_patter;
            std::vector<FormatItem::ptr> m_items;
        
    };
    //日志输出的地方
    class LogAppender//日志输出的地方
    {
        public:
            typedef std::shared_ptr<Appender> pt;
            virtual ~LogFormatter(){}
            
           virtual void log(LogLevel::Level level,LogEvent::ptr event)=0;

           void setFormatter(LogFormatter::ptr val){m_formatter = val;}
           LogFormatter::ptr getFormatter() const {return m_formatetr;}
        protected:
            LogLevel::Level m_level;
            LogFormatter::ptr m_formatter;
    };

    //输出到控制台的Appender
    class StdoutLogAppender : public LogAppender
    {
        public:
            typedef std::shared_ptr<StdoutLogAppender> ptr;
            void log(LogLevel::Level level,LogEvent::ptr event) override;
        private:
    };

    //定义输出到文件的Appender
    class FileLogAppender : public LogAppender
    {
        public:
            typedef std::shared_ptr<FileLogAppender> ptr;
            FileLogAppender(const std::string& filename);
            void log(LogLevel::Level level,LogEvent::ptr event) override;
            //重新打开文件，文件打开成功返回true
            bool reopen();
           
        private:
            std::string m_filename;
            std::ofstream m_file;

    };
}










#endif
