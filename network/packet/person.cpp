#include "person.h"

Person::Person(std::string id)
	: INetworkMessage(), _id(id) {}
Person::Person(char* data, int size)
	: INetworkMessage(), _id((char*)data, size) {}
Person::Person(const Person& p)
	: INetworkMessage(p), _id(p._id) {}

Person* Person::setId(std::string id) {
	_id = id;
	return this;
}
std::string Person::getId () {
	return _id;
}

Person* Person::fromNetworkMessage(uint8 *data) {
	uint8* index = data;
	uint16 size;
	memcpy(&size, index, sizeof(uint16)); index += sizeof(uint16);

	return new Person((char*)index,size);
}

uint8* Person::toNetworkMessage () {
	uint8* data = new uint8[getNetworkMessageSize()];
	uint8* index = data;
	uint16 size = (uint16) _id.size() * sizeof(uint8);
	memcpy(index, &size, sizeof(uint16)); index += sizeof(uint16);
	memcpy(index, _id.c_str(), size);

	return data;
}

int Person::getNetworkMessageSize () {
	return sizeof(uint16) + _id.size();
}
