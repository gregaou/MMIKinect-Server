#ifndef TRAINREQUESTPACKET_H
#define TRAINREQUESTPACKET_H

#include "network/packet/packet.h"
#include "network/packet/person.h"

class TrainRequestPacket : public Packet
{
public:
	TrainRequestPacket(Packet* p);
	TrainRequestPacket(int socket);
	~TrainRequestPacket();
	Person* getPerson ();
	uint8* getTrainData ();
	int getTrainDataSize ();

	TrainRequestPacket* setPerson (Person* p);
	TrainRequestPacket* setTrainData (uint8* data, int size);
	TrainRequestPacket* setTrainData (std::vector<uint8>* data);

private:
	TrainRequestPacket* doUpdateData ();

	Person* _person;
	uint8* _trainData;
	int _trainDataSize;
};

class TrainResultPacket : public Packet
{
public:
	TrainResultPacket (Packet* p);
	TrainResultPacket(int socket);
};

#endif // TRAINREQUESTPACKET_H
