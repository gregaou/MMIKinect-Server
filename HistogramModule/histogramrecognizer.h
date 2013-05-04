#ifndef HISTOGRAMRECOGNIZER_H
#define HISTOGRAMRECOGNIZER_H

#include <stdint.h>
#include <string>
#include <fstream>

#include "histogram.h"

#include "typedef.h"
#include "packet/score.h"

#define CSV_FILE "histogram.csv"

using namespace std;
using namespace cv;

typedef struct {
	string name;
	string filename;
	int size;
}	HistoPerson;

class HistogramRecognizer
{
public:
	HistogramRecognizer(uint8* data, int size);
	HistogramRecognizer(string filename);
	HistogramRecognizer* doTrain (string id);
	HistogramRecognizer* doTest  ();
	ScoringVector* getScoring();

	static PeopleVector* getPeopleVector ();
	vector<HistoPerson> getHistoPeople ();
	HistoPerson getHistoPerson (string id);

	HistogramRecognizer* updateRecord(HistoPerson person);

private:
	Histogram _histogram;
	ScoringVector* _scoring;
};

#endif // HISTOGRAMRECOGNIZER_H
