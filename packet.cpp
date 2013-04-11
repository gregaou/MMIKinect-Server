#include "packet.h"

Packet::Packet(QIODevice *d) : _pData(0), _pStream(new QDataStream(d)) {
}

uint Packet::getBodySize() {
    return (uint)qFromLittleEndian(_pData->mid(1, 4).toInt());
}

void Packet::setBodySize() {
    _pData->resize(getHeaderSize() + getBodySize());
}

QByteArray* Packet::getData() {
    if(!_pData) {
        _pData = new QByteArray();
        readBuffer(_pData, getHeaderSize());
        setBodySize();
        if(getBodySize() != 0)
            readBuffer(_pData, getBodySize());
    }
    return _pData;
}

void Packet::readBuffer(QByteArray *data, int length) {
    int n = 0, r;
    if(!_pStream->device()->canReadLine())
        qDebug() << "The stream cannot read";

    while(n < length) {
        char buffer[length - n];
        r = _pStream->readRawData(buffer, length - n);
        n += r;
        data->append(buffer, r);
    }
}
