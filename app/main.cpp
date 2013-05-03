#include <fstream>

#include "server.h"

int main()
{
	LoggerBuffer* buf = new LoggerBuffer("[DEBUG]   : ", DEBUG);
	buf      ->setNext( new LoggerBuffer("[INFO]    : ", INFO))
					 ->setNext( new LoggerBuffer("[WARNING] : ", WARNING))
					 ->setNext( new LoggerBuffer("[ERROR]   : ", ERROR));

	Logger::getInstance()->setBuffer(buf);

	Packet p(0);
	p.setVersion(PACKET_VERSION)->setType(BROADCAST_TYPE | LISTING_REQUEST)->setId(0x0001)->setData(new uint8[1],1);

	ModuleServer m;
	m.onNewPacket(&p);

//	Server s;
//	s.doWork();

	return 0;
}

