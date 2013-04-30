#ifndef PERSON_H
#define PERSON_H

#include <string>

#include "typedef.h"
#include "inetworkmessage.h"
#include "networkvector.h"

class Person : public INetworkMessage
{
public:
	Person(std::string id) : _id(id) {}
	Person(uint8* data, int size) {
		std::string id((char*)data, size);
		_id = id;
	}

	std::string getId () { return _id; }

	uint8* toNetworkMessage () {
		if (!_data) {
			_data = new uint8[getNetworkMessageSize()];
			uint8* index = _data;
			uint8 size = (uint8) _id.size();
			memcpy(index, &size, sizeof(uint8)); index += sizeof(uint8);
			memcpy(index, _id.c_str(), size);
		}
		return _data;
	}

	int getNetworkMessageSize () {
		if (!_size) {
			_size = sizeof(uint16) + _id.size();
		}
		return _size;
	}

private :
	std::string _id;
};

typedef NetworkVector<Person> PeopleVector;

#endif // PERSON_H
