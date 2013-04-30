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
	Score(std::string id, double score) :
		INetworkMessage(), _person(id), _score(score) {}

	Person getPerson () { return _person; }
	double getScore () { return _score; }

	Score* setId (Person id) { _person = id; return this; }
	Score* setScore (double score) { _score = score; return this; }

	uint8* toNetworkMessage () {
		if (!_data) {
			_data = new uint8[getNetworkMessageSize()];
			uint8* index = _data;
			memcpy(index, &_score, sizeof(double)); index += sizeof(double);
			memcpy(index, _person.toNetworkMessage(), _person.getNetworkMessageSize());
		}
		return _data;
	}

	int getNetworkMessageSize () {
		if (!_size) {
			_size = sizeof(double) + _person.getNetworkMessageSize();
		}
		return _size;
	}

private:
	Person _person;
	double _score;
};

typedef NetworkVector<Score> ScoringVector;

#endif // SCORE_H
