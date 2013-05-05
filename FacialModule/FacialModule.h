#ifndef MMIFRM_H
#define MMIFRM_H

#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#include "tools/logger.h"
#include "module/imodule.h"
#include "network/packet/packet.h"

#include <vector>
#include <map>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string>
#include <iostream>

using namespace std;
using namespace cv;

class FacialModule : public IModule {

public:
		const std::string getName() const;

    void onNewPacket(Packet* p);
	
private:

    void loadCSVFile(const string &filename, char separator = ';');
    void loadFaceRecognizer(const string& filename);

    void onListRequest(Packet *p);
    void onTrainRequest(Packet *p);
    void onScoreRequest(Packet *p);

    Ptr<FaceRecognizer> _faceRecognizer;
    vector<Mat> _images;
    vector<int> _labels;
    map<int, string> _names;
    int getFilesCount(string dir);
    int keyFromValue(string name);
    bool dirExists(string dirName);

};

#endif //MMIFRM_H
