#ifndef __H_FACIALUTILS_H__
#define __H_FACIALUTILS_H__

#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv/cv.h"

#include <vector>
#include <map>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

class FacialUtils {

public :

    static void newUser(const string& filename, const int id, const string name);
	static int lastClassLabel(const string& filename, const char separator = ';');
	static int labelFromName(const string& filename, string findName, const char separator = ';');
	static map<int, string> reloadFromCSVFile(const string &filename, const char separator = ';');
    static void loadFaceRecognizer(Ptr<FaceRecognizer> &f, const string& filename);

private:
	FacialUtils() {}

};

#endif
