#include "trainpacket.h"

TrainRequestPacket::TrainRequestPacket(Packet* p)
	: Packet(p), _person(0), _pTrainData(0) {}

Person* TrainRequestPacket::getPerson () {
	if (!_person) { doReadPerson(); }
	return _person;
}

uint8* TrainRequestPacket::getTrainData() {
	if (!_pTrainData) { doReadData(); }
	return _pTrainData;
}

int TrainRequestPacket::getTrainDataSize() {
	return getBodySize() - getPerson()->getNetworkMessageSize();
}

TrainRequestPacket* TrainRequestPacket::doReadPerson () {
	uint16 size;
	uint8* index = getData();
	memcpy(&size, index, sizeof(uint16));
	_person = new Person(index + sizeof(uint16), size);

	return this;
}

TrainRequestPacket* TrainRequestPacket::doReadData () {
	_pTrainData = getData() + getPerson()->getNetworkMessageSize();
	return this;
}

TrainResultPacket::TrainResultPacket (Packet *p) : Packet(p)
{
	setType(getType() ^ 0x01)->setData(NULL);
}
