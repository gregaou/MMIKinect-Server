#ifndef FACETRACKING_H
#define FACETRACKING_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "typedef.h"

#include "rect.h"

class FaceTracking
{
public:
	FaceTracking();

	std::vector<Rect>* getFacesFromImageByteArray(byte* img);
	std::vector<Rect>* getFacesFromImage(const char* imgPath);

private:
	static CvHaarClassifierCascade* _cascade;
	static CvHaarClassifierCascade* getCascade();
};

#endif // FACETRACKING_H
