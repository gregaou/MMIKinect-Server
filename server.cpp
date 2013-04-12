#include "server.h"

Server::Server(QObject *parent) :
	QTcpServer(parent) {
    if (!listen(QHostAddress::Any,1337)) {
        qDebug() << "Unable to bind port.";
        exit(1);
    }
}

void Server::incomingConnection(qintptr socketDescriptor)
{
		ClientThread *thread = new ClientThread(socketDescriptor, this);
		connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
		thread->start();
}
