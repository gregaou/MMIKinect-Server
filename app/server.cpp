#include "server.h"

Server::Server() : _pSocket(0) {
	*this << INFO << "Running..." << std::endl;
}

Server::~Server() {
	if(!_pSocket) delete _pSocket;
}

const std::string Server::getName() const { return "Server"; }

Server* Server::doWork() {
	_pSocket = new TcpSocketServer();
	while(1) {
		try {
			int socketDescriptor = _pSocket->doAccept();
			ClientThread* client = new ClientThread(socketDescriptor);
			client->start();
		}
		catch (NetworkException e)
		{
			*this << ERROR << e.what() << std::endl;
			break;
		}
	}
	return this;
}
