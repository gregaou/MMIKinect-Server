#ifndef TCPSOCKETSERVER_H
#define TCPSOCKETSERVER_H

#include "tcpsocket.h"

class TcpSocketServer : public TcpSocket
{
public:
	TcpSocketServer();
	~TcpSocketServer();
	virtual socklen_t doAccept(void);

protected:
	virtual TcpSocketServer* doBuildAddr(void);
	virtual TcpSocketServer* doListen(void);
	virtual bool isListening(void);
};

#endif // TCPSOCKETSERVER_H
