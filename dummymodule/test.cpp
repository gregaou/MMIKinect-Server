#include "test.h"

extern "C" IModule* create () {
	return new Test();
}

extern "C" void destroy (IModule* module) {
	delete module;
}

const std::string Test::getName() const { return "Dummy Module"; }

void Test::onNewPacket(Packet *p)  {
	throw ModuleException("False alarm!!", 0);
}
