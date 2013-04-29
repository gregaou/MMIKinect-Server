#include "clientthread.h"

ClientThread::ClientThread(int socketDescriptor) :
	Thread(),
	_socketDescriptor(socketDescriptor)
{}


void ClientThread::run()
{
	io::info << "Client(" << _socketDescriptor << ") : connected" << io::endl;
//	int num=0;
	while(1)
	{
		try {
			io::dbg << "Client(" << _socketDescriptor << ") : Creating packet" << io::endl;
			Packet p(_socketDescriptor);
			io::dbg << "Client(" << _socketDescriptor << ") : Retrieving packet data" << io::endl;
			p.getData();
			io::dbg << "Client(" << _socketDescriptor << ") : Sending packet to modules." << io::endl;
			ModuleServer::getInstance()->onPacketReceived(&p);
		} catch(NetworkException e) {
			io::err << "Client(" << _socketDescriptor << ") : " << e.what() << io::endl;
			io::info << "Client(" << _socketDescriptor << ") : Disconnected" << io::endl;
			close(_socketDescriptor);
			break;
		}
	}
}
