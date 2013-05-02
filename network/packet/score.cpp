#include "score.h"

Score::Score(std::string id, double score)
	: INetworkMessage(), _person(new Person(id)), _score(score) {}

Score::Score(Person* p, double score)
	: INetworkMessage(), _person(p), _score(score) {}

Score::Score(const Score& s)
	: INetworkMessage(s), _person(new Person(*s._person)), _score(s._score) {}

Score::~Score() {
	if (_person) {
		delete _person;
		_person = 0;
	}
}

Person* Score::getPerson () {
	return _person;
}

double Score::getScore () {
	return _score;
}

Score* Score::setPerson (Person* person) {
	if (_person)
		delete _person;

	_person = person;

	return this;
}

Score* Score::setScore (double score) {
	_score = score;

	return this;
}

Score* Score::fromNetworkMessage(uint8 *data) {
	uint8* index = data;
	double score;
	memcpy(&score, index, sizeof(double)); index += sizeof(double);

	return new Score(Person::fromNetworkMessage(index),score);
}

uint8* Score::toNetworkMessage () {
	if (!_data) {
		_data = new uint8[getNetworkMessageSize()];
		uint8* index = _data;
		memcpy(index, &_score, sizeof(double)); index += sizeof(double);
		memcpy(index, _person->toNetworkMessage(), _person->getNetworkMessageSize());
	}
	return _data;
}

int Score::getNetworkMessageSize () {
	return sizeof(double) + _person->getNetworkMessageSize();
}

