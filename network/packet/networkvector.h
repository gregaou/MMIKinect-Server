#ifndef NETWORKVECTOR_H
#define NETWORKVECTOR_H

#include <vector>

#include "inetworkmessage.h"

template <class T>
class NetworkVector : public std::vector<T>, public INetworkMessage
{
public:
	NetworkVector() : std::vector<T>(), INetworkMessage() {}

	uint8* toNetworkMessage () {
		if (!_data) {
			_data = new uint8[getNetworkMessageSize()];
			uint8* index = _data;
			uint8 size = (uint8) this->size();
			memcpy(index, &size, sizeof(uint8)); index += sizeof(uint8);
			for (uint32 i = 0; i < this->size(); i++) {
				memcpy(index, this->at(i).toNetworkMessage(), this->at(i).getNetworkMessageSize());
				index += this->at(i).getNetworkMessageSize();
			}
		}
		return _data;
	}

	int getNetworkMessageSize () {
		if (!_size) {
			_size = sizeof(uint8);
			for (uint32 i = 0; i < this->size(); i++) {
				_size += this->at(i).getNetworkMessageSize();
			}
		}
		return _size;
	}
};

#endif // NETWORKVECTOR_H
