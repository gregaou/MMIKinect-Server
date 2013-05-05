#ifndef SCOREPACKET_H
#define SCOREPACKET_H

#include "network/packet/packet.h"
#include "network/packet/score.h"

class ScoreRequestPacket : public Packet
{
public:
	ScoreRequestPacket(int socket);
	ScoreRequestPacket (Packet* p);
};

class ScoreResultPacket : public Packet
{
public:
	ScoreResultPacket(int socket);
	ScoreResultPacket(Packet* p);
	~ScoreResultPacket();
	ScoringVector* getScoringVector ();
	ScoreResultPacket* setScoringVector (ScoringVector* vector);

private:
	ScoringVector* _vector;
};

#endif // SCOREPACKET_H
