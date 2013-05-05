#ifndef AUDIOMODULE_H
#define AUDIOMODULE_H

#include <algorithm>

#include "module/imodule.h"
#include "network/packet/trainpacket.h"
#include "network/packet/scorepacket.h"
#include "network/packet/listpacket.h"

#include "audiosample.h"

class AudioModule : public IModule
{
public:
	AudioModule() : IModule() {}

	const std::string getName() const;

	void onNewPacket(Packet *p);
private:
	void onTrainRequest(Packet* p);
	void onScoreRequest(Packet* p);
	void onListRequest(Packet* p);
};

#endif // AUDIOMODULE_H
