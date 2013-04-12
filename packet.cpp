#include "packet.h"

Packet::Packet(QTcpSocket *d) : _type(0xff), _bodySize(0), _pData(0), _pSocket(d) {}

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
    uint length = getBodySize();
    qDebug() << "Reading data. Length : " << length;
    doReadStream(_pData, length);
    return this;
}

Packet* Packet::doReadStream(QByteArray *data, int length) {
    qDebug() << "Reading Stream : " << length;
    int n = 0, r;
    while(n < length) {
        char array[length - n];
        if (!_pSocket->waitForReadyRead()) {
            qDebug() << "No data received." << _pSocket->errorString();
            exit(1);
        }
        r = _pSocket->read(array, length - n);
        if (r == -1) {
            qDebug() << "Reading error." << _pSocket->errorString();
            exit(1);
        }
        n += r;
        qDebug() << n <<" octets lus.";
        data->append(array,r);
    }
    return this;
}

Packet* Packet::doSend () {
    QByteArray msg;
    msg.append(getType());
    char size[sizeof(uint)];
    uint value = qToLittleEndian(getBodySize());
    memcpy(size,&value,sizeof(uint));
    msg.append(size,sizeof(uint));
    msg.append(*getData());
    _pSocket->write(msg);
    return this;
}
