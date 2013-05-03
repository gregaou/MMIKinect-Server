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
	LoggerBuffer* setMutex(pthread_mutex_t* mutex);
	virtual int sync ();

protected:
	LoggerBuffer* onNewMessage(const string msg, const LoggerPriority priority);
	LoggerBuffer* displayMessage(const string msg);

private:
	const string _prefix;
	const LoggerPriority _priority;
	ostream& _stream;
	LoggerPriority _currentPriority;
	pthread_mutex_t* _mutex;
	LoggerBuffer* _next;
};

class Logger : public std::ostream, public Singleton<Logger>
{
	friend class Singleton<Logger>;
<<<<<<< HEAD
	friend class LoggerBuffer;
public :
	virtual Logger& operator<< (const LoggerPriority priority);
	Logger& setBuffer(LoggerBuffer* buffer);
private :
	Logger();

	LoggerBuffer* _buffer;
	pthread_mutex_t _mutex;
=======
public :
	virtual Logger& operator<< (const LoggerPriority priority);
	Logger& setBuffer(LoggerBuffer* buffer);
//private :
	Logger();

	LoggerBuffer* _buffer;
>>>>>>> e4b72eb91728a7e7d866e9f8cfef7e3ade6cd849
};

class Loggable
{
public:
	virtual const string getName () const = 0;
	virtual Logger& operator<< (const LoggerPriority priority);
};

#endif // LOGGER_H
