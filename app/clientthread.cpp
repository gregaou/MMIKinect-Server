#include "clientthread.h"

ClientThread::ClientThread(int socketDescriptor) :
	Thread(),
	_socketDescriptor(socketDescriptor),
	_moduleServer(0)
{}

ClientThread::~ClientThread () {
	if (_moduleServer) delete _moduleServer;
}

const std::string ClientThread::getName() const {
	std::ostringstream ret;
	ret << "Client(" << _socketDescriptor << ")";
	return ret.str();
}

void ClientThread::run()
{
	*this << INFO << "connected" << std::endl;
	while(1)
	{
		Packet* p = NULL;
		try {
			p = new Packet(_socketDescriptor);
			getModuleServer()->onNewPacket(p);
			delete p;
			p = NULL;
		} catch(NetworkException e) {
			*this << ERROR << e.what() << std::endl;
			*this << INFO << "Disconnected" << std::endl;
			close(_socketDescriptor);
			if (p) delete p;
			break;
		}
	}
}

ModuleServer* ClientThread::getModuleServer() {
	if (!_moduleServer) {
		_moduleServer = new ModuleServer();
	}
	return _moduleServer;
}
