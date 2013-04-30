#ifndef MMIFRM_H
#define MMIFRM_H

#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#include <vector>
#include <map>

//#include "logger.h"
//#include "imodule.h"
//#include "packet/packet.h"

class Test : public IModule
{
public:
	Test() : IModule () {}

	void onNewPacket (const Packet* p);
};

using namespace std;
using namespace cv;

class MMIFaceRecognitionModule : public IModule {

public:
	MMIFaceRecognitionModule(const string& filename);
	~MMIFaceRecognitionModule();
	
	void onNewPacket(const Packet* p);
	
private:

	void loadCSVFile(vector<Mat>& images, vector<int>& labels, map<int, string>& noms, char separator = ';');
	Ptr<FaceRecognizer> _faceRecognizer;
}

#endif //MMIFRM_H