#include "trainpacket.h"

TrainRequestPacket::TrainRequestPacket(Packet* p)
	: Packet(*p), _person(0), _trainData(0), _trainDataSize(0) {}

TrainRequestPacket::TrainRequestPacket(int socket)
	: Packet(socket), _person(0), _trainData(0), _trainDataSize(0) {}

TrainRequestPacket::~TrainRequestPacket() {
	if (_person) delete _person;
}

Person* TrainRequestPacket::getPerson () {
	if (!_person) {
		_person = Person::fromNetworkMessage(getData());
	}
	return _person;
}

uint8* TrainRequestPacket::getTrainData() {
	if (!_trainData) {
		_trainData = getData() + getPerson()->getNetworkMessageSize();
	}
	return _trainData;
}

int TrainRequestPacket::getTrainDataSize() {
	if (!_trainDataSize) {
		_trainDataSize = getBodySize() - getPerson()->getNetworkMessageSize();
	}
	return _trainDataSize;
}

TrainRequestPacket* TrainRequestPacket::setPerson (Person* p) {
	_person = p;
	doUpdateData();
	return this;
}

TrainRequestPacket* TrainRequestPacket::setTrainData (uint8* data, int size) {
	_trainData = data;
	_trainDataSize = size;
	doUpdateData();
	return this;
}

TrainRequestPacket* TrainRequestPacket::setTrainData (std::vector<uint8>* data) {
	if (!data) { data = new std::vector<uint8>(); }
	_trainData = data->data();
	_trainDataSize = data->size();
	doUpdateData();
	return this;
}

TrainRequestPacket* TrainRequestPacket::doUpdateData() {
	if (!_person || !_trainData) { return this; }

	int size = getPerson()->getNetworkMessageSize() + getTrainDataSize();
	uint8* data = new uint8[size];
	uint8* index = data;
	uint8* msg = getPerson()->toNetworkMessage();
	memcpy(index, msg, getPerson()->getNetworkMessageSize());
	index += getPerson()->getNetworkMessageSize();
	memcpy(index, getTrainData(), getTrainDataSize());

	setData(data, size);
	return this;
}


TrainResultPacket::TrainResultPacket (Packet* p) : Packet(*p)
{
	setType((getType() & 0xF0) | TRAINING_RESULT )->setData();
}

TrainResultPacket::TrainResultPacket(int socket) : Packet(socket) {
	setType((getType() & 0xF0) | TRAINING_RESULT )->setData();
}
