#ifndef IMODULE_H
#define IMODULE_H

#include <string>
#include <vector>

#include "typedef.h"
#include "packet/packet.h"
#include "thread.h"

class IModule {
public:
	IModule () {}
	virtual void onNewPacket (const Packet* p) = 0;
};

class ModuleThread : public Thread {
public :
	ModuleThread (IModule* module) : Thread (), _module(module), _packet(0) {}
	void run () {
		_module->onNewPacket(getPacket());
	}
	ModuleThread* setPacket(Packet* p) { _packet = p; return this; }
	const Packet* getPacket() { return _packet; }
private :
	IModule* _module;
	Packet* _packet;
};

typedef IModule* create_t();
typedef void destroy_t(IModule*);

#endif // IMODULE_H
