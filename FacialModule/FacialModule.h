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

    FacialModule();
    const string getName() const;
    void onNewPacket(Packet* p);

private:

    void loadCSVFile(const string &filename, char separator = ';');
    void loadFaceRecognizer(Ptr<FaceRecognizer> _faceRecognizer, const string& filename);

    void onListRequest(Packet *p);
    void onTrainRequest(Packet *p);
    void onScoreRequest(Packet *p);

    vector<int> _labels;
    map<int, string> _names;
    int lastClassLabel(const string& filename);
    int labelFromName(const string& filename, string findName);
};

#endif //MMIFRM_H
