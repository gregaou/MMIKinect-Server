#ifndef AUDIOMODULE_H
#define AUDIOMODULE_H

#include "imodule.h"

class AudioModule : public IModule
{
public:
	AudioModule() : IModule() {}

	void onNewPacket(Packet *p);
};

#endif // AUDIOMODULE_H
