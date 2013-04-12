#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QDebug>
#include <QThread>
#include <QTcpSocket>
#include <QDataStream>
#include <QImage>
#include "packet.h"

class ClientThread : public QThread
{
	Q_OBJECT
public:
	ClientThread(int socketDescriptor, QObject *parent);
	void run();

signals:
	void error(QTcpSocket::SocketError socketError);

private:
	int _socketDescriptor;
	QTcpSocket* _pTcpSocket;
    int num;

private slots:
	void readyRead();
	void disconnected();
};

#endif // CLIENTTHREAD_H
