#include "logger.h"

io::DebugLogger   io::dbg;
io::InfoLogger    io::info;
io::WarningLogger io::warn;
io::ErrorLogger   io::err;

char io::endl = '\n';

io::Logger::Logger (std::string prefix, std::ostream& stream)
	: std::ostream(stream.rdbuf()), __prefix(prefix)
{}

std::ostream& io::Logger::operator<< (const char* str) {
	std::ostream& __out = *this;
	std::stringstream msg;
	msg << __prefix << str;
	__out << msg.str();
	return __out;
}
