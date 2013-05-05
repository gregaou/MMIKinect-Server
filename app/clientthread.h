#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include "tools/thread.h"
#include "network/packet/packet.h"
#include "server.h"

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
