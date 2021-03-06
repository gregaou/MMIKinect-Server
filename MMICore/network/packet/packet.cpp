#include "packet.h"

Packet::Packet(Packet& p) {
	_pSocket = p._pSocket;
	_version = p._version;
	_type = p._type;
	_id = p._id;
	if (p._pData) {
		_pData = new uint8[p._bodySize];
		if (p._bodySize)
			memcpy(_pData, p._pData, p._bodySize);
		_bodySize = p._bodySize;
	} else {
		_pData = 0;
		_bodySize = 0;
	}
}

Packet::Packet(int socketDescriptor)
	: _version(0x00), _type(UNDEFINED_TYPE), _id(0), _bodySize(0), _pData(0)
{
	_pSocket.setSocket(socketDescriptor);
}

Packet::~Packet() {
	if (_pData)
		delete[] _pData;
}

const std::string Packet::getName() const { return "Packet"; }

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
	return _pData;
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
	if (_pData) { delete[] _pData; }
	if (!data) { data = new std::vector<uint8>(); }
	_pData = data->data();
	setBodySize(data->size());
	return this;
}

Packet* Packet::setData(uint8* data, int size) {
	if (_pData) { delete[] _pData; }
	_pData = data;
	setBodySize(size);
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

	uint8 buffer[_headerSize-1];
	_pSocket.readBuffer(buffer,_headerSize-1);
	setType(buffer[0]);

	uint16 identifiant;
	memcpy(&identifiant, &buffer[1], sizeof(uint16));
	setId(identifiant);

	uint32 size;
	memcpy(&size,&buffer[3],sizeof(uint32));
	setBodySize(size);

	return this;
}

Packet* Packet::doReadData () {
	uint32 length = getBodySize();
	_pData = new uint8[length];
	if (length != 0) {
		*this << DEBUG << "Reading data. Length : " << length << std::endl;
		_pSocket.readBuffer(_pData, length);
	}
	return this;
}

Packet* Packet::doSend () {
	*this << DEBUG << "Creating packet" << std::endl;
	int length = _headerSize + _bodySize;
	uint8 message[length];
	uint8* index = message;

	uint8 version = _version;
	memcpy(index, &version, sizeof(uint8)); index += sizeof(uint8);
	uint8 type = _type;
	memcpy(index, &type, sizeof(uint8)); index += sizeof(uint8);
	uint16 id = _id;
	memcpy(index, &id, sizeof(uint16)); index += sizeof(uint16);
	uint32 bodySize = _bodySize;
	memcpy(index, &bodySize, sizeof(uint32)); index += sizeof(uint32);
	if (_pData)
		memcpy(index, _pData, sizeof(uint8) * _bodySize);
	*this << DEBUG << "Writing data. Length : " << length << std::endl;
	_pSocket.writeBuffer(message,length);

	return this;
}
