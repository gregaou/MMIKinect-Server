#include "test.h"

extern "C" IModule* create () {
	return new Test();
}

extern "C" void destroy (IModule* module) {
	delete module;
}

void Test::onNewPacket(Packet *p)  {
	throw ModuleException("False alarm!!", 0);
}
