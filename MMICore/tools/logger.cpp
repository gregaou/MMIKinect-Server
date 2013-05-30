#include "logger.h"

#include <fstream>

#include "tools/config.h"

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

void Logger::configure() {
	Config* c = Config::getInstance();
	string prefix;
	LoggerBuffer* buf = NULL;

	if (c->paramExists("DEBUG.prefix"))
		prefix = c->getParamValue("DEBUG.prefix");
	if (c->paramExists("DEBUG.file")) {
		ofstream* stream = new ofstream(c->getParamValue("DEBUG.file").c_str());
		if (buf == NULL) {
			buf = new LoggerBuffer(prefix, DEBUG, *stream);
		} else {
			buf->setNext(new LoggerBuffer(prefix, DEBUG, *stream));
		}
	}
	if (c->paramExists("DEBUG.stream")) {
		string name = c->getParamValue("DEBUG.stream");
		ostream* stream = NULL;
		if (name == "std::cout") {
			stream = &std::cout;
		} else if (name == "std::clog") {
			stream = &std::clog;
		} else if (name == "std::cerr") {
			stream = &std::cerr;
		}

		if (stream != NULL) {
			if (buf == NULL) {
				buf = new LoggerBuffer(prefix, DEBUG, *stream);
			} else {
				buf->setNext(new LoggerBuffer(prefix, DEBUG, *stream));
			}
		}
	}

	if (c->paramExists("INFO.prefix"))
		prefix = c->getParamValue("INFO.prefix");
	if (c->paramExists("INFO.file")) {
		ofstream* stream = new ofstream(c->getParamValue("INFO.file").c_str());
		if (buf == NULL) {
			buf = new LoggerBuffer(prefix, INFO, *stream);
		} else {
			buf->setNext(new LoggerBuffer(prefix, INFO, *stream));
		}
	}
	if (c->paramExists("INFO.stream")) {
		string name = c->getParamValue("INFO.stream");
		ostream* stream = NULL;
		if (name == "std::cout") {
			stream = &std::cout;
		} else if (name == "std::clog") {
			stream = &std::clog;
		} else if (name == "std::cerr") {
			stream = &std::cerr;
		}

		if (stream != NULL) {
			if (buf == NULL) {
				buf = new LoggerBuffer(prefix, INFO, *stream);
			} else {
				buf->setNext(new LoggerBuffer(prefix, INFO, *stream));
			}
		}
	}

	if (c->paramExists("WARN.prefix"))
		prefix = c->getParamValue("WARN.prefix");
	if (c->paramExists("WARN.file")) {
		ofstream* stream = new ofstream(c->getParamValue("WARN.file").c_str());
		if (buf == NULL) {
			buf = new LoggerBuffer(prefix, WARNING, *stream);
		} else {
			buf->setNext(new LoggerBuffer(prefix, WARNING, *stream));
		}
	}
	if (c->paramExists("WARN.stream")) {
		string name = c->getParamValue("WARN.stream");
		ostream* stream = NULL;
		if (name == "std::cout") {
			stream = &std::cout;
		} else if (name == "std::clog") {
			stream = &std::clog;
		} else if (name == "std::cerr") {
			stream = &std::cerr;
		}

		if (stream != NULL) {
			if (buf == NULL) {
				buf = new LoggerBuffer(prefix, WARNING, *stream);
			} else {
				buf->setNext(new LoggerBuffer(prefix, WARNING, *stream));
			}
		}
	}

	if (c->paramExists("ERROR.prefix"))
		prefix = c->getParamValue("ERROR.prefix");
	if (c->paramExists("ERROR.file")) {
		ofstream* stream = new ofstream(c->getParamValue("ERROR.file").c_str());
		if (buf == NULL) {
			buf = new LoggerBuffer(prefix, ERROR, *stream);
		} else {
			buf->setNext(new LoggerBuffer(prefix, ERROR, *stream));
		}
	}
	if (c->paramExists("ERROR.stream")) {
		string name = c->getParamValue("ERROR.stream");
		ostream* stream = NULL;
		if (name == "std::cout") {
			stream = &std::cout;
		} else if (name == "std::clog") {
			stream = &std::clog;
		} else if (name == "std::cerr") {
			stream = &std::cerr;
		}

		if (stream != NULL) {
			if (buf == NULL) {
				buf = new LoggerBuffer(prefix, ERROR, *stream);
			} else {
				buf->setNext(new LoggerBuffer(prefix, ERROR, *stream));
			}
		}
	}

	if (buf != NULL)
		Logger::getInstance()->setBuffer(buf);
}


LoggerBuffer::LoggerBuffer(const string prefix, const LoggerPriority priority,
													 ostream& stream, LoggerBuffer* next)
	: _prefix(prefix), _priority(priority), _stream(stream), _next(next) {}

LoggerBuffer* LoggerBuffer::setNext(LoggerBuffer* next) {
	if (_next != NULL)
		next->setNext(_next);

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
