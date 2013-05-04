#ifndef HISTOGRAMMODULE_H
#define HISTOGRAMMODULE_H

#include "imodule.h"
#include "packet/trainpacket.h"
#include "packet/scorepacket.h"
#include "packet/listpacket.h"

#include "histogramrecognizer.h"

class HistogramModule : public IModule
{
public:
	HistogramModule() : IModule() {}

	const std::string getName() const;

	void onNewPacket(Packet *p);
private:
	void onTrainRequest(Packet* p);
	void onScoreRequest(Packet* p);
	void onListRequest(Packet* p);
};

#endif // HISTOGRAMMODULE_H
