#include "clientthread.h"
#include <fstream>

#include "packet.h"
#include "facetracking.h"

ClientThread::ClientThread(int socketDescriptor) :
	QThread(), _socketDescriptor(socketDescriptor)
{}


void ClientThread::run()
{
    displayMessage("Client connected", Logger::INFO);
	int num=0;
	while(1)
	{
		try {
            displayMessage("Creating packet", Logger::INFO);
			Packet p(_socketDescriptor);
            displayMessage("Saving file", Logger::INFO);
			std::ostringstream filename;
			filename << "File" << num++ << ".jpg";
			std::ofstream file(filename.str().c_str(),std::ios::out);
			file.write((const char*)p.getData(),p.getBodySize());
            displayMessage("File saved!", Logger::INFO);
			FaceTracking ft;
			ft.getFacesFromImage(filename.str().c_str());
		} catch(NetworkException e) {
            displayMessage(e.what(), Logger::ERROR);
            displayMessage("Disconnected", Logger::INFO);
			close(_socketDescriptor);
			break;
		}
	}
}

ClientThread* ClientThread::displayMessage(std::string msg, int type) {
    std::ostringstream message;
    message << "Client(" << _socketDescriptor << ")" << " : " << msg;
    Logger::getLogger()->message(message.str(), type);
	return this;
}
