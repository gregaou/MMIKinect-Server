#include "listpacket.h"

ListRequestPacket::ListRequestPacket(Packet* p)
	: Packet(*p) {}

ListResultPacket::ListResultPacket(Packet* p)
	: Packet(*p), _vector(0) { setType(getType() ^ 0x01); }

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
