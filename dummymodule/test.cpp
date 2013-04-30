#include "test.h"

extern "C" IModule* create () {
	return new Test();
}

extern "C" void destroy (IModule* module) {
	delete module;
}

void Test::onNewPacket(Packet *p)  {
	if (!p) { io::warn << "Empty packet!" << io::endl; }
	else {
		io::info << "Packet received!" << io::endl;
	}
}
