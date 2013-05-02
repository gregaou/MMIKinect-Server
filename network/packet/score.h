#ifndef SCORE_H
#define SCORE_H

#include <string>
#include <cstring>

#include "typedef.h"
#include "inetworkmessage.h"
#include "networkvector.h"
#include "person.h"

class Score : public INetworkMessage
{
public:
	Score(std::string id, double score);
	Score(Person* p, double score);
	Score(const Score& s);
	virtual ~Score();

	Person* getPerson ();
	double getScore ();

	Score* setPerson (Person* person);
	Score* setScore (double score);

	static Score* fromNetworkMessage(uint8 *data);

	uint8* toNetworkMessage ();
	int getNetworkMessageSize ();

private:
	Person* _person;
	double _score;
};

typedef NetworkVector<Score> ScoringVector;

#endif // SCORE_H
