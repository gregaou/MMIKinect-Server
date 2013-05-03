#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include "thread.h"
#include "server.h"
#include "packet/packet.h"

class ClientThread : public Thread {
public:
	ClientThread(int socketDescriptor);
	~ClientThread ();

	const std::string getName() const;

	void run();

private:
	ModuleServer* getModuleServer();

	int _socketDescriptor;
	ModuleServer* _moduleServer;
};

#endif // CLIENTTHREAD_H
