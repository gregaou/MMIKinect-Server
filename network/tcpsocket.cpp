#include "tcpsocket.h"

TcpSocket::TcpSocket() :
	_port(PORT_DEFAULT),
	_sock(INVALID_SOCKET),
	_builded(false)
{}

TcpSocket::~TcpSocket()
{
//	if (_sock != INVALID_SOCKET)
//		closesocket(_sock);
}

void TcpSocket::writeBuffer(const byte* buffer, size_t len)
{
	int r;
	r = send(_sock, (const char*)buffer, len, 0);

	/* Paquet successffuly send */
	if (r == (int)len) return;


	if (r == SOCKET_ERROR) throw NetworkException("send == -1 ", errno);
	if (r == 0) throw NetworkException("send == 0", errno);

	/* Paquet partially send (should not occur) */
	if (r < (int)len) throw NetworkException("Paquet partially send", -1);
}

void TcpSocket::readBuffer(byte* buffer, size_t len)
{
	int r;
	fd_set set;

	FD_ZERO(&set);
	FD_SET(_sock, &set);
	r = select(_sock+1, &set, NULL, NULL, NULL);

	if (r < 0) throw NetworkException("select < 0", errno);
	if (r == 0) throw NetworkException("select == 0", errno);

	if (FD_ISSET(_sock, &set))
	{
		size_t n = 0;

		while (n < len)
		{
			r = recv(_sock, (char*)buffer+n, len-n, 0);
			if (r < 0) throw NetworkException("rev < 0", errno);
			if (r == 0) throw NetworkException("rev == 0", errno);
			n += r;
		}
	}
}

sockaddr_in *TcpSocket::getAddr(void) {
	if(!_builded) doBuildAddr();
	return &_sin;
}

int TcpSocket::getSocket() {
	if(_sock == INVALID_SOCKET) {
		if((_sock = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
			throw NetworkException("socket", errno);
		doBind();
	}
	return _sock;
}

TcpSocket* TcpSocket::doBind() {
	if(bind(getSocket(),(sockaddr *)getAddr(),sizeof(sockaddr_in)) ==
		 SOCKET_ERROR)
		throw NetworkException("bind", errno);
	return this;
}

TcpSocket* TcpSocket::doBuildAddr()
{
	_sin.sin_addr.s_addr = inet_addr("127.0.0.1");
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(_port);
	_builded = true;
	return this;
}

TcpSocket* TcpSocket::setPort(int port) {
	_port = port;
	return this;
}

TcpSocket* TcpSocket::setSocket(int sock){
	_sock = sock;
	return this;
}

TcpSocket* TcpSocket::doClose(){
	close(_sock);
	return this;
}
