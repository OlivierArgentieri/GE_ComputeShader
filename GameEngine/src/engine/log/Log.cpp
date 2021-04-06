#include "Log.h"
#include <ctime>
#include <iostream>

// static 
std::ofstream Log::file;

Log::Log()
{
    file.open(GL_LOG_FILE, std::fstream::app);

}

Log::~Log()
{
    os << std::endl;
    file << os.str();
    printf(os.str().c_str());
    os.clear();
    file.close();
}


void Log::restart()
{

    file.open(GL_LOG_FILE, std::fstream::trunc);
    file.close();
}

std::ostringstream& Log::get(LogLevel _level)
{

    if (!file) return os;

    // Log
    time_t _now;
    struct tm* _timeInfo;
    char _date[19];

    time(&_now);
    _timeInfo = localtime(&_now);
    strftime(_date, 19, "%y-%m-%d %H:%M:%S", _timeInfo);

    // Log
    os << _date << " " << getLabel(_level) << ": \t";

    return os;
}

std::string Log::getLabel(LogLevel _type)
{
    std::string _label;
    switch (_type)
    {
    case Debug: _label = "DEBUG"; break;
    case Info: _label = "INFO"; break;
    case Warning: _label = "WARN"; break;
    case Error: _label = "ERROR"; break;
    }

    return _label;
}