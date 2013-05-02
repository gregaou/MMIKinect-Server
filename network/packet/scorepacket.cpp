#include "scorepacket.h"

ScoreRequestPacket::ScoreRequestPacket(Packet *p)
	: Packet(*p) {}

ScoreResultPacket::ScoreResultPacket(Packet *p)
	: Packet(*p), _vector(0) { setType(getType() ^ 0x01); }

ScoreResultPacket::~ScoreResultPacket() {
	if (_vector)
		delete _vector;
}

ScoringVector* ScoreResultPacket::getScoringVector() {
	if (!_vector) {
		_vector = ScoringVector::fromNetworkMessage(getData());
	}
	return _vector;
}

ScoreResultPacket* ScoreResultPacket::setScoringVector(ScoringVector *vector) {
	_vector = vector;
	setData(_vector->toNetworkMessage(), _vector->getNetworkMessageSize());

	return this;
}
