#ifndef TEST_H
#define TEST_H

#include "logger.h"
#include "imodule.h"
#include "packet/packet.h"

#include "moduleexception.h"

class Test : public IModule
{
public:
	Test() : IModule () {}

	const std::string getName() const;
	void onNewPacket (Packet* p);
};

#endif // TEST_H
