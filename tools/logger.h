#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <sstream>

#include "singleton.h"

using namespace std;

enum LoggerPriority {
	UNDEFINED,
	DEBUG,
	INFO,
	WARNING,
	ERROR
};

class LoggerBuffer : public std::stringbuf
{
public:
	LoggerBuffer(const string prefix, const LoggerPriority priority,
							 ostream& stream = cout, LoggerBuffer* next = NULL);

	LoggerBuffer* setNext(LoggerBuffer* next = NULL);
	LoggerBuffer* setCurrentPriority(const LoggerPriority priority);
	virtual int sync ();

protected:
	LoggerBuffer* onNewMessage(const string msg, const LoggerPriority priority);
	LoggerBuffer* displayMessage(const string msg);

private:
	const string _prefix;
	const LoggerPriority _priority;
	ostream& _stream;
	LoggerPriority _currentPriority;
	LoggerBuffer* _next;
};

class Logger : public std::ostream, public Singleton<Logger>
{
	friend class Singleton<Logger>;
public :
	virtual Logger& operator<< (const LoggerPriority priority);
	Logger& setBuffer(LoggerBuffer* buffer);
//private :
	Logger();

	LoggerBuffer* _buffer;
};

class Loggable
{
public:
	virtual const string getName () const = 0;
	virtual Logger& operator<< (const LoggerPriority priority);
};

#endif // LOGGER_H
