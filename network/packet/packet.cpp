#include "packet.h"

Packet::Packet(Packet* p) {
	_pSocket = p->_pSocket;
	setVersion(p->getVersion());
	setType(p->getType());
	setId(p->getId());
	setData(p->_pData);
}

Packet::Packet(int socketDescriptor): _version(0x00), _type(UNDEFINED_TYPE), _id(0), _bodySize(0), _pData(0)
{
	_pSocket.setSocket(socketDescriptor);
}

uint8 Packet::getVersion(){
	if (!_version) { doReadVersion(); }
	return _version;
}

uint8 Packet::getType(){
	if (_type == UNDEFINED_TYPE) { doReadHeader(); }
	return _type;
}

uint16 Packet::getId(){
	if (!_id) { doReadHeader(); }
	return _id;
}

uint32 Packet::getBodySize() {
	if (_bodySize == 0) { doReadHeader(); }
	return _bodySize;
}

uint8* Packet::getData() {
	if(!_pData) { doReadData(); }
	return (uint8*)_pData->data();
}

Packet* Packet::setVersion(uint8 version) {
	_version = version;
	return this;
}

Packet* Packet::setType(uint8 type) {
	_type = type;
	return this;
}

Packet* Packet::setId(uint16 identifiant) {
	_id = identifiant;
	return this;
}

Packet* Packet::setBodySize(uint32 size) {
	_bodySize = size;
	return this;
}

Packet* Packet::setData(std::vector<uint8>* data) {
	if (!data) { data = new std::vector<uint8>(); }
	_pData = data;
	setBodySize(_pData->size());
	return this;
}

Packet* Packet::doReadVersion() {
	uint8 version;
	_pSocket.readBuffer(&version,1);
	setVersion(version);

	return this;
}

Packet* Packet::doReadHeader() throw (NetworkException){
	if (getVersion() != PACKET_VERSION) {
		std::stringstream msg;
		msg << "Wrong packet version : "
				<< ((uint32)getVersion()) << " given, "
				<< PACKET_VERSION << " expected";
		throw NetworkException(msg.str().c_str(), -1);
	}

	uint8 buffer[_headerSize];
	_pSocket.readBuffer(buffer,_headerSize-1);
	setType((PacketType)buffer[1]);

	uint16 identifiant;
	memcpy(&identifiant, &buffer[2], sizeof(uint16));
	setId(identifiant);

	uint32 size;
	memcpy(&size,&buffer[4],sizeof(uint32));
	setBodySize(size);

	return this;
}

Packet* Packet::doReadData () {
	_pData = new std::vector<uint8>();
	uint32 length = getBodySize();
	io::dbg << "Reading data. Length : " << length << io::endl;
	_pData->resize(length);
	_pSocket.readBuffer(_pData->data(), length);
	return this;
}

Packet* Packet::doSend () {
//	uint8 message[_headerSize + getBodySize()];
//	message[0] = getType();
//	uint32 value = htonl(getBodySize());
//	memcpy(&message[1],&value,sizeof(uint32));
//	memcpy(&message[_headerSize],getData()->data(),
//				 getData()->size()*sizeof(uint8));
//	_pSocket.writeBuffer(message,_headerSize+getBodySize());
	return this;
}
