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

	FacialModule() : IModule() {}
	const string getName() const;
	void onNewPacket(Packet* p);

private:

	/* Traitement du paquet */
	void onListRequest(Packet *p);
	void onTrainRequest(Packet *p);
	void onScoreRequest(Packet *p);

	/* Traitement de données */
	int lastClassLabel(const string& filename, const char separator = ';');
	int labelFromName(const string& filename, string findName, const char separator = ';');
	void reloadFromCSVFile(const string &filename, const char separator = ';');
	Ptr<FaceRecognizer> loadFaceRecognizer(const string& filename);

	/* Support d'informations */
	vector<int> _labels;
	map<int, string> _names;

};

#endif //MMIFRM_H
