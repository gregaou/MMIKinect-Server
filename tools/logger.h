#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
#include <sstream>

#define ERROR_STREAM   std::cerr
#define DEBUG_STREAM   std::cout
#define INFO_STREAM    std::clog
#define WARNING_STREAM std::clog
#define DEFAULT_STREAM INFO_STREAM

#define ERROR_PREFIX   "[ERROR]: "
#define DEBUG_PREFIX   "[DEBUG]: "
#define INFO_PREFIX    "[INFO] : "
#define WARNING_PREFIX "[WARN] : "
#define DEFAULT_PREFIX INFO_PREFIX

namespace io {

extern char endl;

class Logger : public std::ostream {
public :
	Logger (std::string prefix = DEFAULT_PREFIX,
					std::ostream& stream = DEFAULT_STREAM);

	virtual std::ostream& operator<< (const char* str);

private:
	std::string __prefix;
};

class DebugLogger : public Logger {
public:
	DebugLogger () : Logger(DEBUG_PREFIX,DEBUG_STREAM) {}
};

class InfoLogger : public Logger {
public:
	InfoLogger () : Logger(INFO_PREFIX,INFO_STREAM) {}
};

class WarningLogger : public Logger {
public:
	WarningLogger () : Logger(WARNING_PREFIX,WARNING_STREAM) {}
};

class ErrorLogger : public Logger {
public:
	ErrorLogger () : Logger(ERROR_PREFIX,ERROR_STREAM) {}
};

extern ErrorLogger err;
extern InfoLogger info;
extern WarningLogger warn;
extern DebugLogger dbg;

}

#endif // LOGGER_H
