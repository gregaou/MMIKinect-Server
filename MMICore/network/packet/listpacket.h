#ifndef LISTPACKET_H
#define LISTPACKET_H

#include "network/packet/person.h"
#include "network/packet/packet.h"

class ListRequestPacket : public Packet
{
public:
	ListRequestPacket(Packet* p);
	ListRequestPacket(int socket);
};

class ListResultPacket : public Packet
{
public:
	ListResultPacket(Packet* p);
	~ListResultPacket();
	PeopleVector* getPeopleVector ();
	ListResultPacket* setPeopleVector (PeopleVector* vector);

private:
	PeopleVector* _vector;
};

#endif // LISTPACKET_H
