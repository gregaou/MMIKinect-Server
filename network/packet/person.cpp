#include "person.h"

Person::Person(std::string id)
	: INetworkMessage(), _id(id) {}
Person::Person(char* data, int size)
	: INetworkMessage(), _id((char*)data, size) {}
Person::Person(const Person& p)
	: INetworkMessage(p), _id(p._id) {}

Person* Person::setId(std::string id) {
	_id = id;
	_size = 0;
	if (_data) { delete _data; _data = 0; }
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
	if (!_data) {
		_data = new uint8[getNetworkMessageSize()];
		uint8* index = _data;
		uint16 size = (uint16) _id.size() * sizeof(uint8);
		memcpy(index, &size, sizeof(uint16)); index += sizeof(uint16);
		memcpy(index, _id.c_str(), size);
	}
	return _data;
}

int Person::getNetworkMessageSize () {
	if (!_size) {
		_size = sizeof(uint16) + _id.size();
	}
	return _size;
}
