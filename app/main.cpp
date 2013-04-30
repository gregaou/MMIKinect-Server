#include "server.h"

#include "packet/trainpacket.h"

int main()
{
//	Server s;
//	s.doWork();
//	ModuleServer::kill();

	std::vector<uint8>* data = new std::vector<uint8>();
	data->push_back(0x04);
	data->push_back(0x00);
	data->push_back('T');
	data->push_back('h');
	data->push_back('e');
	data->push_back('o');

	Packet* p = new Packet(0);
	p->setVersion(PACKET_VERSION)
	 ->setType(AUDIO_TYPE | TRAINING_REQUEST)
	 ->setId(0x0001)
	 ->setData(data);
	ModuleServer::getInstance()->onPacketReceived(p)->onPacketReceived(NULL);
	ModuleServer::kill();
	delete p;

	return 0;
}

