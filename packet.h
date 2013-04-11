#ifndef PACKET_H
#define PACKET_H

#include <QDataStream>
#include <QtEndian>
#include <QDebug>

class Packet {

public:

    Packet(QIODevice *d);
    int getHeaderSize() { return _headerLength; }
    char getType() { return _type; }
    uint getBodySize();
    QByteArray *getData();
    void setBodySize();
    void readBuffer(QByteArray *data, int length);

private:

    char _type;
    static const int _headerLength = 5;
    QByteArray *_pData;
    QDataStream *_pStream;

};

#endif // PACKET_H
