#include "server.h"

Server::Server() : _pSocket(0) {
    displayMessage("Creating...", Logger::INFO);
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
            displayMessage(e.what(), Logger::ERROR);
			break;
		}
	}
	return this;
}

Server* Server::displayMessage(std::string msg, int type) {
    std::ostringstream message;
    message << "Server : " << msg;
    Logger::getLogger()->message(message.str(), type);
    return this;
}

