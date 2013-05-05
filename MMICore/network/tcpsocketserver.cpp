#include "tcpsocketserver.h"

TcpSocketServer::TcpSocketServer() : TcpSocket(){}
TcpSocketServer::~TcpSocketServer() {}

TcpSocketServer* TcpSocketServer::doBuildAddr()
{
	_sin.sin_addr.s_addr = htonl(INADDR_ANY);
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(_port);
	_builded = true;
	return this;
}


TcpSocketServer* TcpSocketServer::doListen() {
	if(!isListening())
		if(listen(getSocket(),SOMAXCONN) == SOCKET_ERROR)
			throw NetworkException("listen", errno);
	return this;
}

socklen_t TcpSocketServer::doAccept(void) {
	doListen();
	int newConnection;
	sockaddr_in clientaaddr;
	socklen_t clientAddrLen = sizeof(sockaddr_in);

	newConnection = accept(getSocket(),(sockaddr*)&clientaaddr,&clientAddrLen);
	if(newConnection == INVALID_SOCKET) throw NetworkException("accept", errno);
	return newConnection;
}

bool TcpSocketServer::isListening() {
	int val;
	socklen_t len = sizeof(val);
	getsockopt(getSocket(),SOL_SOCKET,SO_ACCEPTCONN, &val, &len);
	return (val!=0);
}
