#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QThread>

#include "logger.h"

class ClientThread : public QThread
{
	Q_OBJECT
public:
	ClientThread(int socketDescriptor);
	void run();

private:
	int _socketDescriptor;

    ClientThread* displayMessage(std::string msg, int type);

};

#endif // CLIENTTHREAD_H
