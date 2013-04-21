#include "server.h"

Server::Server() : _pSocket(0) {
	displayMessage("Creating ...");
}

Server::~Server() {
	if(!_pSocket) delete _pSocket;
}

Server* Server::doWork() {
	_pSocket = new TcpSocketServer();
	while(1) {
		try {
			int socketDescriptor = _pSocket->doAccept();
			ClientThread *ct = new ClientThread(socketDescriptor);
			ct->start();
		}
		catch (NetworkException e)
		{
			displayMessage(e.what());
			break;
		}
	}
	return this;
}

Server* Server::displayMessage(std::string msg) {
	std::cout << "Server : " << msg << std::endl;
	return this;
}

