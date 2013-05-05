#include "test.h"

extern "C" IModule* create () {
	return new Test();
}

extern "C" void destroy (IModule* module) {
	delete module;
}

const std::string Test::getName() const { return "Dummy Module"; }

void Test::onNewPacket(Packet *p)  {
	*this << DEBUG << "This is the module " << getName() << endl;
	*this << DEBUG << "Its associated folder is " << getFolder() << endl;

	throw ModuleException("Throwing exception form within a module", 0);
}
