#ifndef HISTOGRAMRGB_H
#define HISTOGRAMRGB_H

#include <string>
#include <pthread.h>

#include "tools/typedef.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

class Histogram
{
public:
	Histogram(Histogram& h);
	Histogram(const char* filename);
	Histogram(string filename);
	Histogram(Mat m);
	Histogram(vector<uint8> vec);
	Histogram(uint8* data, int size);

	MatND init (Mat m);
	MatND init (vector<uint8> vec);
	MatND init (uint8* data, int size);

	Histogram* saveToFile (const char* filename);
	Histogram* saveToFile (string filename);

	MatND getMatND ();

	Histogram* update(Histogram& h, double weight);
	Histogram* update(MatND m, double weight);

	double compare(Histogram& h);
	double compare(MatND m);

	static void lockMutex();
	static void unlockMutex();
private:
	MatND _histogram;
	static pthread_mutex_t* _mutex;
};

#endif // HISTOGRAMRGB_H
