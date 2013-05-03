#include "modulethread.h"

#include "packet/packet.h"
#include "imodule.h"
#include "modulehandler.h"

ModuleThread::ModuleThread(IModule *module, ModuleHandler *handler)
	: _module(module), _parent(handler) {}

ModuleThread::~ModuleThread() {
	if (_packet) delete _packet;
}

const std::string ModuleThread::getName() const {
	std::ostringstream stream;
	stream << "Module Thread(" << _module->getName() << ")" ;
	return stream.str();
}

void ModuleThread::run() {
	try {
		_module->onNewPacket(_packet);
	} catch (std::exception &e) {
		*this << ERROR << "Error while running thread : " << e.what() << std::endl;
	}
	_parent->onThreadFinished();
}

ModuleThread* ModuleThread::start(Packet *p) {
	_packet = p;
	Thread::start();

	return this;
}
