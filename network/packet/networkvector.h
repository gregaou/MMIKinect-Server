#ifndef NETWORKVECTOR_H
#define NETWORKVECTOR_H

#include <vector>

#include "inetworkmessage.h"

template <class T>
class NetworkVector : public std::vector<T>, public INetworkMessage
{
public:
	NetworkVector() : std::vector<T>(), INetworkMessage() {}

	static NetworkVector<T>* fromNetworkMessage (uint8* data) {
		NetworkVector<T>* vec = new NetworkVector<T>();

		uint8* index = data;
		uint8 size;
		memcpy(&size, index, sizeof(uint8)); index += sizeof(uint8);
		for (int i = 0; i < size; i++) {
			T* t = T::fromNetworkMessage(index);
			index += t->getNetworkMessageSize();
			vec->push_back(*t);
			delete t;
		}
		return vec;
	}

	uint8* toNetworkMessage () {
		if (!_data) {
			_data = new uint8[getNetworkMessageSize()];
			uint8* index = _data;
			uint8 size = (uint8) this->size();
			memcpy(index, &size, sizeof(uint8)); index += sizeof(uint8);
			for (uint8 i = 0; i < size; i++) {
				memcpy(index, this->at(i).toNetworkMessage(), this->at(i).getNetworkMessageSize());
				index += this->at(i).getNetworkMessageSize();
			}
		}
		return _data;
	}

	int getNetworkMessageSize () {
		int size = sizeof(uint8);
		for (uint32 i = 0; i < this->size(); i++) {
			size += this->at(i).getNetworkMessageSize();
		}
		return size;
	}
};

#endif // NETWORKVECTOR_H
