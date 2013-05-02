#ifndef SCOREPACKET_H
#define SCOREPACKET_H

#include "packet/packet.h"
#include "packet/score.h"

class ScoreRequestPacket : public Packet
{
public:
	ScoreRequestPacket (Packet* p);
};

class ScoreResultPacket : public Packet
{
public:
	ScoreResultPacket(Packet* p);
	~ScoreResultPacket();
	ScoringVector* getScoringVector ();
	ScoreResultPacket* setScoringVector (ScoringVector* vector);

private:
	ScoringVector* _vector;
};

#endif // SCOREPACKET_H
