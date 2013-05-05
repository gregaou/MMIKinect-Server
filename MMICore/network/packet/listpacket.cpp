#include "listpacket.h"

ListRequestPacket::ListRequestPacket(Packet* p)
	: Packet(*p) {}

ListRequestPacket::ListRequestPacket(int socket)
	: Packet(socket) {}

ListResultPacket::ListResultPacket(Packet* p)
	: Packet(*p), _vector(0)
{ setType((getType() & 0xF0) | LISTING_RESULT ); }

ListResultPacket::~ListResultPacket() {
	if (_vector)
		delete _vector;
}

PeopleVector* ListResultPacket::getPeopleVector () {
	if (!_vector) {
		_vector = PeopleVector::fromNetworkMessage(getData());
	}
	return _vector;
}

ListResultPacket* ListResultPacket::setPeopleVector (PeopleVector* vector) {
	_vector = vector;
	setData(_vector->toNetworkMessage(), _vector->getNetworkMessageSize());

	return this;
}
