#include "scorepacket.h"

ScoreRequestPacket::ScoreRequestPacket(int socket)
	: Packet(socket) {}

ScoreRequestPacket::ScoreRequestPacket(Packet *p)
	: Packet(*p) {}

ScoreResultPacket::ScoreResultPacket(int socket)
	: Packet(socket), _vector(0)
{ setType((getType() & 0xF0) | SCORING_RESULT ); }

ScoreResultPacket::ScoreResultPacket(Packet *p)
	: Packet(*p), _vector(0)
{ setType((getType() & 0xF0) | SCORING_RESULT ); }

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
