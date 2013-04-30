#ifndef AUDIOMODULE_H
#define AUDIOMODULE_H

#include <algorithm>

#include "imodule.h"
#include "packet/trainpacket.h"

#include "audiosample.h"

class AudioModule : public IModule
{
public:
	AudioModule() : IModule() {}

	void onNewPacket(Packet *p);
	void onTrainRequest(Packet* p);
};

#endif // AUDIOMODULE_H
