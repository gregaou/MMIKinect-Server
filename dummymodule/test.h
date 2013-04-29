#ifndef TEST_H
#define TEST_H

#include "logger.h"
#include "imodule.h"
#include "packet/packet.h"

class Test : public IModule
{
public:
	Test() : IModule () {}

	void onNewPacket (const Packet* p);
};

#endif // TEST_H
