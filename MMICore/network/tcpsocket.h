#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <stdexcept>
#include <sstream>
#include <iostream>

#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "tools/typedef.h"
#include "network/networkexception.h"

#define PORT_DEFAULT 1338
#define INVALID_SOCKET		-1
#define SOCKET_ERROR		-1

class TcpSocket
{

public:
	TcpSocket();
	virtual ~TcpSocket();

	virtual void writeBuffer(const uint8* buffer, size_t len);
	virtual void readBuffer(uint8* buffer, size_t len);

	virtual TcpSocket* setPort(int port);
	virtual TcpSocket* setSocket(int sock);

	virtual TcpSocket* doClose();
protected:
	sockaddr_in _sin;
	int _port;
	int _sock;
	bool _builded;

	virtual TcpSocket* doBuildAddr(void);
	virtual TcpSocket* doBind(void);

	virtual int getSocket(void);
	virtual sockaddr_in* getAddr(void);
};

#endif // TCPSOCKET_H
