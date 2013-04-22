#include "logger.h"

Logger* Logger::_logger(0);

Logger* Logger::getLogger() {
    if (!_logger)
        (_logger = new DebugLogger())
            ->setNext(new InfoLogger())
//          ->setNext(new WarningLogger())
            ->setNext(new ErrorLogger());
    return _logger;
}

Logger::Logger (int level) {
    this->_level = level;
    this->_next = NULL;
}

Logger* Logger::setNext (Logger* next) {
    _next = next;
    return _next;
}

Logger* Logger::message (std::string msg, int priority) {
    if (priority == _level) {
        writeMessage(msg);
    } else if (_next != NULL) {
        _next->message(msg, priority);
    }
    return this;
}

Logger* DebugLogger::writeMessage (std::string msg) {
    std::cout << "[DEBUG] " << msg;
    return this;
}

Logger* ErrorLogger::writeMessage (std::string msg) {
    std::cerr << "[ERROR] " << msg;
    return this;
}

Logger* InfoLogger::writeMessage (std::string msg) {
    std::clog << "[INFO]  " << msg;
    return this;
}
