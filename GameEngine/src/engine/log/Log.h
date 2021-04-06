#pragma once

#include <fstream>
#include <sstream>

#define GL_LOG_FILE "gl.log" // log file name

enum LogLevel { Error, Warning, Info, Debug };

struct LogConfig
{
    LogLevel reporting_level = Info;
    bool restart = false;
};

extern LogConfig LOG_CONFIG;

class Log
{
public:
    Log();
    virtual ~Log();
    std::ostringstream& get(LogLevel _level = Info);
    static void restart();

private:
    std::ostringstream os;
    static std::ofstream file;

    std::string getLabel(LogLevel _type);

    Log(const Log&) = delete;
    Log& operator=(const Log&) = delete;

};

#define LOG(level) \
if (level > LOG_CONFIG.reporting_level); \
else Log().get(level)
