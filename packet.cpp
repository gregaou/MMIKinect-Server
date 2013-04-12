#include "packet.h"

Packet::Packet(QIODevice *d) : _type(0xff), _bodySize(0), _pData(0), _pStream(new QDataStream(d)) {}

uchar Packet::getType() {
    if (_type == 0xFF) { doReadHeader(); }
    return _type;
}

uint Packet::getBodySize() {
    if (_bodySize == 0) { doReadHeader(); }
    return _bodySize;
}

QByteArray* Packet::getData() {
    if(!_pData) { doReadData(); }
    return _pData;
}

Packet* Packet::setType(uchar type) {
    _type = type;
    return this;
}

Packet* Packet::setBodySize(uint size) {
    _bodySize = size;
    return this;
}

Packet* Packet::setData(QByteArray* data) {
    _pData = data;
    setBodySize(_pData->size());
    return this;
}

Packet* Packet::doReadHeader() {
    QByteArray *buffer = new QByteArray();
    doReadStream(buffer,_headerSize);

    setType(buffer->at(0));
    uint size;
    memcpy(&size,buffer->mid(1,4).data(),sizeof(uint));
    setBodySize(size);

    delete(buffer);
    return this;
}

Packet* Packet::doReadData () {
    _pData = new QByteArray();
    doReadStream(_pData, getBodySize());
    return this;
}

Packet* Packet::doReadStream(QByteArray *data, int length) {
		int n = 0, r;
		if(!_pStream->device()->canReadLine())
                qDebug() << "Cannot read stream";

		while(n < length) {
				char buffer[length - n];
				r = _pStream->readRawData(buffer, length - n);
				n += r;
				data->append(buffer, r);
		}
        return this;
}

Packet* Packet::doSend () {
    QByteArray *msg = new QByteArray();
    msg->append(getType());
    char size[sizeof(uint)];
    uint value = qToLittleEndian(getBodySize());
    memcpy(size,&value,sizeof(uint));
    msg->append(size,sizeof(uint));
    msg->append(*getData());
    _pStream->writeRawData(msg->data(), msg->size());
    return this;
}
