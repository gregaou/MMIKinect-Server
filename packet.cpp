#include "packet.h"

Packet::Packet(int socketDescriptor): _type(0xff), _bodySize(0), _pData(0)
{
	_pSocket.setSocket(socketDescriptor);
}

byte Packet::getType() {
	if (_type == 0xFF) { doReadHeader(); }
	return _type;
}

uint Packet::getBodySize() {
	if (_bodySize == 0) { doReadHeader(); }
	return _bodySize;
}

byte* Packet::getData() {
	if(!_pData) { doReadData(); }
	return (byte*)_pData->data();
}

Packet* Packet::setType(byte type) {
	_type = type;
	return this;
}

Packet* Packet::setBodySize(uint size) {
	_bodySize = size;
	return this;
}

Packet* Packet::setData(std::vector<byte>* data) {
	_pData = data;
	setBodySize(_pData->size());
	return this;
}

Packet* Packet::doReadHeader() {
	byte buffer[_headerSize];
	_pSocket.readBuffer(buffer,_headerSize);
	setType(buffer[0]);

	uint size;
	memcpy(&size,&buffer[1],sizeof(uint));
	setBodySize(size);

	return this;
}

Packet* Packet::doReadData () {
	_pData = new std::vector<byte>();
	uint length = getBodySize();
	std::cout << "Reading data. Length : " << length << std::endl;
	_pData->resize(length);
	_pSocket.readBuffer(_pData->data(), length);
	return this;
}

Packet* Packet::doSend () {
//	byte message[_headerSize + getBodySize()];
//	message[0] = getType();
//	uint value = htonl(getBodySize());
//	memcpy(&message[1],&value,sizeof(uint));
//	memcpy(&message[_headerSize],getData()->data(),
//				 getData()->size()*sizeof(byte));
//	_pSocket.writeBuffer(message,_headerSize+getBodySize());
	return this;
}
