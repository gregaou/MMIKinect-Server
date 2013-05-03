#include "clientthread.h"

ClientThread::ClientThread(int socketDescriptor) :
	Thread(),
	_socketDescriptor(socketDescriptor)
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
		try {
			Packet* p = new Packet(_socketDescriptor);
			getModuleServer()->onNewPacket(p);
			delete p;
		} catch(NetworkException e) {
			*this << ERROR << e.what() << std::endl;
			*this << INFO << "Disconnected" << std::endl;
			close(_socketDescriptor);
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
