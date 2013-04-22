#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>

class Logger
{
private:
    static Logger* _logger;

protected:
    int _level;
    Logger* _next;

    virtual Logger* writeMessage (std::string msg) = 0;

public:
    enum { INFO, DEBUG, ERROR };

    Logger (int level);
    Logger* setNext (Logger* _next);
    Logger* message (std::string msg, int priority);

    static Logger* getLogger ();
};

class DebugLogger : public Logger {
public:
    DebugLogger () : Logger(Logger::DEBUG) {}
    Logger* writeMessage (std::string msg);
};

class ErrorLogger : public Logger {
public:
    ErrorLogger () : Logger(Logger::ERROR) {}
    Logger* writeMessage (std::string msg);
};

class InfoLogger : public Logger {
public:
    InfoLogger () : Logger(Logger::INFO) {}
    Logger* writeMessage (std::string msg);
};

#endif // LOGGER_H
