#include <fstream>

#include "opencv2/opencv.hpp"

#include "server.h"

int main()
{
	LoggerBuffer* buf = new LoggerBuffer("[DEBUG]   : ", DEBUG,   std::clog);
	buf      ->setNext( new LoggerBuffer("[INFO]    : ", INFO,    std::cout))
					 ->setNext( new LoggerBuffer("[WARNING] : ", WARNING, std::clog))
					 ->setNext( new LoggerBuffer("[ERROR]   : ", ERROR,   std::cerr));

	Logger::getInstance()->setBuffer(buf);

	Server s;
	s.doWork();

	return 0;
}

