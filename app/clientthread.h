#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include "thread.h"
#include "server.h"
#include "packet/packet.h"

class ClientThread : public Thread {
public:
	ClientThread(int socketDescriptor);
	void run();

private:
	int _socketDescriptor;
};

#endif // CLIENTTHREAD_H
