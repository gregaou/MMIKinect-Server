#include "packet.h"

Packet::Packet(int socketDescriptor): _version(0x00), _type(UNDEFINED_TYPE), _id(0), _bodySize(0), _pData(0)
{
	_pSocket.setSocket(socketDescriptor);
}

byte Packet::getVersion(){
	if (!_version) { doReadVersion(); }
	return _version;
}

PacketType Packet::getType(){
	if (_type == UNDEFINED_TYPE) { doReadHeader(); }
	return _type;
}

id Packet::getId(){
	if (!_id) { doReadHeader(); }
	return _id;
}

uint Packet::getBodySize() {
	if (_bodySize == 0) { doReadHeader(); }
	return _bodySize;
}

byte* Packet::getData() {
	if(!_pData) { doReadData(); }
	return (byte*)_pData->data();
}

Packet* Packet::setVersion(byte version) {
	_version = version;
	return this;
}

Packet* Packet::setType(PacketType type) {
	_type = type;
	return this;
}

Packet* Packet::setId(id identifiant) {
	_id = identifiant;
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

Packet* Packet::doReadVersion() {
	byte version;
	_pSocket.readBuffer(&version,1);
	setVersion(version);

	return this;
}

Packet* Packet::doReadHeader() throw (NetworkException){
	if (getVersion() != PACKET_VERSION) {
		std::stringstream msg;
		msg << "Wrong packet version : "
				<< ((uint)getVersion()) << " given, "
				<< PACKET_VERSION << " expected";
		throw NetworkException(msg.str().c_str(), -1);
	}

	byte buffer[_headerSize];
	_pSocket.readBuffer(buffer,_headerSize-1);
	setType((PacketType)buffer[1]);

	id identifiant;
	memcpy(&identifiant, &buffer[2], sizeof(id));
	setId(identifiant);

	uint size;
	memcpy(&size,&buffer[4],sizeof(uint));
	setBodySize(size);

	return this;
}

Packet* Packet::doReadData () {
	_pData = new std::vector<byte>();
	uint length = getBodySize();
	io::dbg << "Reading data. Length : " << length << io::endl;
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
