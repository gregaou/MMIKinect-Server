#ifndef MODULETHREAD_H
#define MODULETHREAD_H

#include "tools/thread.h"

class IModule;
class Packet;
class ModuleHandler;

class ModuleThread : public Thread
{
public:
	ModuleThread(IModule* module, ModuleHandler* handler);
	~ModuleThread();

	const std::string getName() const;

	ModuleThread* start(Packet* p);
	void run ();
private:
	IModule* _module;
	Packet* _packet;

	ModuleHandler* _parent;
};

#endif // MODULETHREAD_H
