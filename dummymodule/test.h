#ifndef TEST_H
#define TEST_H

#include "tools/logger.h"
#include "network/packet/packet.h"
#include "module/imodule.h"
#include "module/moduleexception.h"

class Test : public IModule
{
public:
	Test() : IModule () {}

	const std::string getName() const;
	void onNewPacket (Packet* p);
};

#endif // TEST_H
