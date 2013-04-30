#include "audiomodule.h"

extern "C" IModule* create () {
	return new AudioModule();
}

extern "C" void destroy (IModule* module) {
	delete module;
}

void AudioModule::onNewPacket(Packet *p)  {

	if (!p) { io::warn << "Empty packet!" << io::endl; }
	else {
		io::info << "Packet received by Audio module!" << io::endl;
	}
	p->getType();
}
