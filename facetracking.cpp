#include "facetracking.h"


#include <ctime>

FaceTracking::FaceTracking()
{}

std::vector<Rect>* FaceTracking::getFacesFromImageByteArray(byte* img){
	(void)img;
	return 0;
}

std::vector<Rect>* FaceTracking::getFacesFromImage(const char *imgPath){
	// Chargement images
	IplImage* img = cvLoadImage(imgPath);
	static CvMemStorage* storage = 0;
	storage = cvCreateMemStorage(0);
	clock_t temps_initial, temps_final;
	double temps_cpu;
	temps_initial = clock ();

	CvSeq* faces = cvHaarDetectObjects( img, getCascade(), storage,
																			1.1, 2, CV_HAAR_DO_CANNY_PRUNING);
	temps_final = clock ();
	temps_cpu = ((double)(temps_final - temps_initial)) / CLOCKS_PER_SEC * 1000; // millisecondes

	std::cout << temps_cpu << "ms" << std::endl;

	std::vector<Rect>* facesRect = new std::vector<Rect>();
	for(int i = 0; i < (faces ? faces->total : 0); i++ )
	{
		CvRect* r = (CvRect*)cvGetSeqElem( faces, i );
		Rect rec(r->x,r->y,r->x+r->width,r->y+r->height);
		facesRect->push_back(rec);
	}
	cvClearMemStorage( storage );
	return facesRect;
}

CvHaarClassifierCascade* FaceTracking::_cascade = 0;

CvHaarClassifierCascade* FaceTracking::getCascade() {
	if(!_cascade) {
		_cascade =
				(CvHaarClassifierCascade*)
				cvLoad("haarcascade_frontalface_alt.xml", 0, 0, 0 );
		//if(!_cascade) throw Exception;
	}
	return _cascade;
}
