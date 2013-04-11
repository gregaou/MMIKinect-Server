#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include "clientthread.h"

class Server : public QTcpServer
{
	Q_OBJECT
public:
	Server(QObject *parent = 0);

protected:
		void incomingConnection(qintptr socketDescriptor);

signals:

public slots:

};

#endif // SERVER_H
