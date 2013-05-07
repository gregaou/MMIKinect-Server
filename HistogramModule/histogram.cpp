#include "histogram.h"

pthread_mutex_t* Histogram::_mutex = 0;

Histogram::Histogram(Histogram& h)
	: _histogram(h.getMatND().clone()) {}

Histogram::Histogram(const char* file) {
	string filename(file);
	string extension = filename.substr(filename.find_last_of('.'));
	if (extension == ".xml" || extension == ".yaml" || extension == "yml") {
		lockMutex();
		FileStorage fs(filename, FileStorage::READ);
		fs["histogram"] >> _histogram;
		fs.release();
		unlockMutex();
	} else {
		_histogram = init(imread(filename));
	}
}

Histogram::Histogram(string filename) {
	string extension = filename.substr(filename.find_last_of('.'));
	if (extension == ".xml" || extension == ".yaml" || extension == ".yml") {
		lockMutex();
		FileStorage fs(filename, FileStorage::READ);
		fs["histogram"] >> _histogram;
		fs.release();
		unlockMutex();
	} else {
		_histogram = init(imread(filename));
	}
}

Histogram::Histogram(Mat m)
	: _histogram(init(m)) {}

Histogram::Histogram(vector<uint8> vec)
	: _histogram(init(vec)) {}

Histogram::Histogram(uint8* data, int size)
	: _histogram(init(data,size)) {}

MatND Histogram::init (uint8* data, int size) {
	return init(vector<uint8>(data, data + size));
}

MatND Histogram::init (vector<uint8> vec) {
	Mat bgr = imdecode(vec, CV_LOAD_IMAGE_COLOR);
	return init(bgr);
}

MatND Histogram::init (Mat m) {
	cvtColor(m, m, CV_BGR2HSV);

	MatND result;
	int channels[] = { 0, 1 };
	int h_bins = 50; int s_bins = 60;
	int histSize[] = { h_bins, s_bins };
	float h_ranges[] = { 0, 256 };
	float s_ranges[] = { 0, 180 };
	const float* ranges[] = { h_ranges, s_ranges };

	calcHist(&m, 1, channels, Mat(), result, 2, histSize, ranges, true, false);
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	return result;
}

Histogram* Histogram::saveToFile (const char* filename) {
	return saveToFile(string(filename));
}

Histogram* Histogram::saveToFile (string filename) {
		lockMutex();
	FileStorage fs(filename, FileStorage::WRITE);
	fs << "histogram" << _histogram;
	fs.release();
	unlockMutex();

	return this;
}

MatND Histogram::getMatND () {
	return _histogram;
}

Histogram* Histogram::update(Histogram& h, double weight) {
	return update(h.getMatND(), weight);
}

Histogram* Histogram::update(MatND m, double weight) {
	addWeighted(_histogram,1-weight,m,weight,0.0,_histogram);
	return this;
}

double Histogram::compare(Histogram& h) {
	return compare(h.getMatND());
}

double Histogram::compare(MatND m) {
	return compareHist(_histogram, m, CV_COMP_CHISQR);
}

void Histogram::lockMutex() {
	if (!_mutex) {
		_mutex = new pthread_mutex_t;
		pthread_mutex_init(_mutex,NULL);
	}
	pthread_mutex_lock(_mutex);
}

void Histogram::unlockMutex() {
	pthread_mutex_unlock(_mutex);
}
