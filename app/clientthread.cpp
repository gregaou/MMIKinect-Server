#include "clientthread.h"

ClientThread::ClientThread(int socketDescriptor) :
	Thread(),
	_socketDescriptor(socketDescriptor)
{}

ClientThread::~ClientThread () {
	if (_moduleServer) delete _moduleServer;
}


void ClientThread::run()
{
	io::info << "Client(" << _socketDescriptor << ") : connected" << io::endl;
	while(1)
	{
		try {
			Packet* p = new Packet(_socketDescriptor);
			getModuleServer()->onPacketReceived(p);
			delete p;
		} catch(NetworkException e) {
			io::err << "Client(" << _socketDescriptor << ") : " << e.what() << io::endl;
			io::info << "Client(" << _socketDescriptor << ") : Disconnected" << io::endl;
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
