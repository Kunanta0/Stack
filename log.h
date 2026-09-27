#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

///this enum creates levels for log file
enum LogLevel
{
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
};

///this define accepts name of log file, level, and other arguments, for printing them in log file
#define write_log(file, level, ...)\
{\
    time_t now = time(NULL);\
    char* time_str = ctime(&now);\
    time_str[strcspn(time_str, "\n")] = 0;\
    \
    fprintf(file, "%s ", time_str);\
    fprintf(file, #level": " __VA_ARGS__);\
}

#endif // LOG_H_INCLUDED
