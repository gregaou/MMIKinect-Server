#ifndef IMODULE_H
#define IMODULE_H

#include <string>
#include <vector>

#include "tools/typedef.h"
#include "tools/thread.h"
#include "network/packet/packet.h"

class IModule : public Loggable {
public:
	IModule () : _folder("") {}
	virtual ~IModule () {}
	virtual void onNewPacket (Packet* p) = 0;
	virtual void setFolder (std::string folder) { _folder = folder; }
    virtual std::string getFolder() { return _folder; }

private:
	std::string _folder;
};

#endif // IMODULE_H
