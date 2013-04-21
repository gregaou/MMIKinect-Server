#include "clientthread.h"
#include <fstream>

#include "packet.h"
#include "facetracking.h"

ClientThread::ClientThread(int socketDescriptor) :
	QThread(), _socketDescriptor(socketDescriptor)
{}


void ClientThread::run()
{
	displayMessage("Client connected");
	int num=0;
	while(1)
	{
		try {
			displayMessage("Creating packet");
			Packet p(_socketDescriptor);
			displayMessage("Saving file");
			std::ostringstream filename;
			filename << "File" << num++ << ".jpg";
			std::ofstream file(filename.str().c_str(),std::ios::out);
			file.write((const char*)p.getData(),p.getBodySize());
			displayMessage("File saved!");
			FaceTracking ft;
			ft.getFacesFromImage(filename.str().c_str());
		} catch(NetworkException e) {
			displayMessage(e.what());
			displayMessage("Disconnected");
			close(_socketDescriptor);
			break;
		}
	}
}

ClientThread* ClientThread::displayMessage(std::string msg) {
	std::cout << "Client(" << _socketDescriptor << ")" << " : " << msg <<
							 std::endl;
	return this;
}
