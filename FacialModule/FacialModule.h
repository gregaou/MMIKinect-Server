#ifndef MMIFRM_H
#define MMIFRM_H

#include "FacialUtils.h"
#include "tools/logger.h"
#include "module/imodule.h"
#include "network/packet/packet.h"

class FacialModule : public IModule {

public:

	FacialModule() : IModule() {}
	const string getName() const;
	void onNewPacket(Packet* p);

private:

	/* Traitement du paquet */
	void onListRequest(Packet *p);
	void onTrainRequest(Packet *p);
	void onScoreRequest(Packet *p);

    Ptr<FaceRecognizer> _faceRecognizer;

};

#endif //MMIFRM_H
