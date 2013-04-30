#ifndef TRAINREQUESTPACKET_H
#define TRAINREQUESTPACKET_H

#include "packet/packet.h"
#include "packet/person.h"

class TrainRequestPacket : public Packet
{
public:
	TrainRequestPacket(Packet* p);
	Person* getPerson ();
	uint8* getTrainData ();
	int getTrainDataSize ();
private:
	TrainRequestPacket* doReadPerson ();
	TrainRequestPacket* doReadData ();

	Person* _person;
	uint8* _pTrainData;
	int _trainDataSize;
};

class TrainResultPacket : public Packet
{
public:
	TrainResultPacket (Packet* p);
};

#endif // TRAINREQUESTPACKET_H
