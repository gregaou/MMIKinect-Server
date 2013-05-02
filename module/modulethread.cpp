#include "modulethread.h"

#include "packet/packet.h"
#include "imodule.h"
#include "modulehandler.h"

ModuleThread::ModuleThread(IModule *module, ModuleHandler *handler)
	: _module(module), _parent(handler) {}

ModuleThread::~ModuleThread() {
	if (_packet) delete _packet;
}

void ModuleThread::run() {
	try {
		_module->onNewPacket(_packet);
	} catch (std::exception &e) {
		io::err << "Error while running thread : " << e.what() << io::endl;
	}
	_parent->onThreadFinished();
}

ModuleThread* ModuleThread::start(Packet *p) {
	_packet = p;
	Thread::start();

	return this;
}
