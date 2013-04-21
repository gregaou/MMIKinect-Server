#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QThread>

class ClientThread : public QThread
{
	Q_OBJECT
public:
	ClientThread(int socketDescriptor);
	void run();

private:
	int _socketDescriptor;

	ClientThread* displayMessage(std::string msg);

};

#endif // CLIENTTHREAD_H
