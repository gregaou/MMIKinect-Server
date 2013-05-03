#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <sys/types.h>
#include <dirent.h>

#include "logger.h"
#include "modulehandler.h"
#include "moduleserver.h"
#include "tcpsocketserver.h"
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
