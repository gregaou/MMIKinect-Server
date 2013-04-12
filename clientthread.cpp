#include "clientthread.h"

ClientThread::ClientThread(int socketDescriptor, QObject *parent) :
	QThread(parent), _socketDescriptor(socketDescriptor)
{
}


void ClientThread::run()
{
	//thread starts here
	qDebug() << _socketDescriptor << " Starting thread";
	_pTcpSocket = new QTcpSocket();
	if(!_pTcpSocket->setSocketDescriptor(_socketDescriptor))
	{
		emit error(_pTcpSocket->error());
		return;
	}

	connect(_pTcpSocket,SIGNAL(readyRead()),this,SLOT(readyRead()),Qt::DirectConnection);
	connect(_pTcpSocket,SIGNAL(disconnected()),this,SLOT(disconnected()),Qt::DirectConnection);

	qDebug() << _socketDescriptor << " Client Connected";

	exec();
}

void ClientThread::readyRead()
{
		//QByteArray Data = _pTcpSocket->readAll();
        Packet *p = new Packet(_pTcpSocket);
        qDebug() << _socketDescriptor << " Data in: " << QString::fromUtf8(p->getData()->data());
        QString *str = new QString("Greg, C'est toi la pute!");
        QByteArray array = str->toUtf8();
        p->setData(&array)->doSend();
		//_pTcpSocket->write(*p->getData());
}

void ClientThread::disconnected()
{
	qDebug() << _socketDescriptor << " Disconnected";

	_pTcpSocket->deleteLater();
	exit(0);
}
