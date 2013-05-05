#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <sys/types.h>
#include <dirent.h>

#include "tools/logger.h"
#include "network/tcpsocketserver.h"
#include "module/modulehandler.h"
#include "module/moduleserver.h"
#include "clientthread.h"

class Server : public Loggable
{
public:
	Server();
	~Server();

	const std::string getName() const;

	Server* doWork();

private:
	TcpSocketServer* _pSocket;
};

#endif // SERVER_H
