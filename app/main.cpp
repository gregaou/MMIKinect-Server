#include "server.h"

int main()
{
//	Server s;
//	s.doWork();
//	ModuleServer::kill();

	Packet* p = new Packet(0);
	p->setVersion(PACKET_VERSION)
	 ->setType(BROADCAST_TYPE | LISTING_REQUEST)
	 ->setId(0x0001);
	ModuleServer::getInstance()->onPacketReceived(p);
	ModuleServer::kill();
	delete p;

	return 0;
}

