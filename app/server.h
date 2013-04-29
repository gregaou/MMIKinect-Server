#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <sys/types.h>
#include <dirent.h>

#include "logger.h"
#include "moduleloader.h"
#include "moduleserver.h"
#include "tcpsocketserver.h"
#include "clientthread.h"

class Server
{
public:
	Server();
	~Server();
	Server* doWork();

private:
	TcpSocketServer* _pSocket;
};

#endif // SERVER_H
