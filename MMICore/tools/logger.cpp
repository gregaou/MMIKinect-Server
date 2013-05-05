#include "logger.h"

Logger& Loggable::operator<< (const LoggerPriority priority) {
	Logger& logger = *Logger::getInstance();

	logger << priority << getName() << " : ";

	return logger;
}

Logger::Logger()
	: std::ostream(cout.rdbuf()), Singleton<Logger>(), _buffer(0)
{ pthread_mutex_init(&_mutex, NULL); }

Logger& Logger::operator<< (const LoggerPriority priority) {
	if (_buffer) {
		pthread_mutex_lock(&_mutex);
		_buffer->setCurrentPriority(priority);
		_buffer->setMutex(&_mutex);
	}

	return *this;
}

Logger& Logger::setBuffer(LoggerBuffer* buffer) {
	_buffer = buffer;
	rdbuf(buffer);

	return *this;
}

LoggerBuffer::LoggerBuffer(const string prefix, const LoggerPriority priority,
													 ostream& stream, LoggerBuffer* next)
	: _prefix(prefix), _priority(priority), _stream(stream), _next(next) {}

LoggerBuffer* LoggerBuffer::setNext(LoggerBuffer* next) {
	_next = next;

	return next;
}

LoggerBuffer* LoggerBuffer::setCurrentPriority(const LoggerPriority priority) {
	_currentPriority = priority;

	return this;
}

LoggerBuffer* LoggerBuffer::setMutex(pthread_mutex_t* mutex) {
	_mutex = mutex;

	return this;
}


int LoggerBuffer::sync () {
	onNewMessage(str(), _currentPriority);
	str("");
	pthread_mutex_unlock(_mutex);
	return 0;
}

LoggerBuffer* LoggerBuffer::onNewMessage(const string msg, const LoggerPriority priority) {
	if (_priority == priority) {
		displayMessage(msg);
	}
	if (_next != NULL) {
		_next->onNewMessage(msg, priority);
	}

	return this;
}

LoggerBuffer* LoggerBuffer::displayMessage(const string msg) {
	((ostream&)_stream) << _prefix << msg;
	_stream.flush();

	return this;
}