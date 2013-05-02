#ifndef LISTPACKET_H
#define LISTPACKET_H

#include "packet/person.h"
#include "packet/packet.h"

class ListRequestPacket : public Packet
{
public:
	ListRequestPacket(Packet* p);
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
