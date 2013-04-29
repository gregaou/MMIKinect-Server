#include "server.h"

Server::Server() : _pSocket(0) {
	io::info << "Server : Running..." << io::endl;
}

Server::~Server() {
	if(!_pSocket) delete _pSocket;
}

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
			io::err << e.what() << io::endl;
			break;
		}
	}
	return this;
}
