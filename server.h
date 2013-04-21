#ifndef SERVER_H
#define SERVER_H

#include "tcpsocketserver.h"
#include "clientthread.h"

class Server
{
public:
	Server();
	~Server();
	Server* doWork();

private:
	static Server instance;
	TcpSocketServer* _pSocket;
	Server* displayMessage(std::string msg);
};

#endif // SERVER_H
