#include "test.h"

extern "C" IModule* create () {
	return new Test();
}

extern "C" void destroy (IModule* module) {
	delete module;
}

void Test::onNewPacket(Packet *p)  {
	if (!p) { io::warn << "Dummy Module : Empty packet!" << io::endl; }
	else {
		sleep(1);
		io::dbg << "Dummy Module : Packet received!" << io::endl;
	}
}
