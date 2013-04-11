#include "server.h"

Server::Server(QObject *parent) :
	QTcpServer(parent) {
	listen(QHostAddress::Any,1337);
}

void Server::incomingConnection(qintptr socketDescriptor)
{
		ClientThread *thread = new ClientThread(socketDescriptor, this);
		connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
		thread->start();
}
