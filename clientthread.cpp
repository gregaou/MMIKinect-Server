#include "clientthread.h"
#include <sstream>
#include <fstream>

using namespace std;

ClientThread::ClientThread(int socketDescriptor, QObject *parent) :
    QThread(parent), _socketDescriptor(socketDescriptor)
{
    num = 0;
}


void ClientThread::run()
{
    //thread starts here
    qDebug() << _socketDescriptor << " Starting thread";
    _pTcpSocket = new QTcpSocket();
    if(!_pTcpSocket->setSocketDescriptor(_socketDescriptor))
    {
        emit error(_pTcpSocket->error());
        return;
    }

    connect(_pTcpSocket,SIGNAL(readyRead()),this,SLOT(readyRead()), Qt::DirectConnection);
    connect(_pTcpSocket,SIGNAL(disconnected()),this,SLOT(disconnected()), Qt::DirectConnection);

    qDebug() << _socketDescriptor << " Client Connected";

    exec();
}

int ClientThread::getSafeNumber () {
    return num ++;
}

void ClientThread::readyRead()
{
    //mutex.lock();
    qDebug() << "Creating packet";
    Packet p(_pTcpSocket);
    qDebug() << "Saving file";
    ostringstream filename;
    filename << "File" << getSafeNumber() << ".jpeg";
    ofstream file(filename.str().c_str(),ios::out);
    file.write(p.getData()->data(),p.getData()->size());
    qDebug() << "File saved!";
    //mutex.unlock();
}

void ClientThread::disconnected()
{
    qDebug() << _socketDescriptor << " Disconnected";

    _pTcpSocket->deleteLater();
    exit(0);
}
