#ifndef IMODULE_H
#define IMODULE_H

#include <string>
#include <vector>

#include "typedef.h"
#include "packet/packet.h"
#include "thread.h"

class IModule : public Loggable {
public:
	IModule () {}
	virtual ~IModule () {}
	virtual void onNewPacket (Packet* p) = 0;
};

#endif // IMODULE_H
