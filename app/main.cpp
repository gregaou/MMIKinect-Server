#include <fstream>

#include "server.h"

int main()
{
	LoggerBuffer* buf = new LoggerBuffer("[DEBUG]   : ", DEBUG, std::clog);
	buf      ->setNext( new LoggerBuffer("[INFO]    : ", INFO))
					 ->setNext( new LoggerBuffer("[WARNING] : ", WARNING))
					 ->setNext( new LoggerBuffer("[ERROR]   : ", ERROR, std::cerr));

	Logger::getInstance()->setBuffer(buf);

	Server s;
	s.doWork();

	return 0;
}

