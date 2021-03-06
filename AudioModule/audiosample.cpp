#include "audiosample.h"

pthread_mutex_t* AudioSample::_mutex = 0;

void AudioSample::lockMutex() {
	if (!_mutex) {
		_mutex = new pthread_mutex_t;
		pthread_mutex_init(_mutex,NULL);
	}
	pthread_mutex_lock(_mutex);
}

void AudioSample::unlockMutex() {
	pthread_mutex_unlock(_mutex);
}

AudioSample::AudioSample(uint8* data, int size, AudioFormat format, string folder)
	: _scoring(0), _size(size), _format(format), _folder(folder)
{
	_data = new uint8[size];
	memcpy(_data, data, size);
}

AudioSample::AudioSample(string filename, AudioFormat format, string folder)
	: _scoring(0), _data(0), _format(format), _folder(folder)
{
	doLoadDataFromFile(filename);
}

AudioSample::~AudioSample() {
	if (_data) delete[] _data;
}

AudioSample* AudioSample::doLoadDataFromFile(string filename) {
	if (_data) delete[] _data;

	ifstream input(filename.c_str(), ios::in|ios::binary|ios::ate);
	_size = input.tellg();
	_data = new uint8[_size];
	input.seekg(0, ios::beg);
	input.read((char*)_data, _size);
	input.close();

	return this;
}

AudioSample* AudioSample::doSaveDataToFile(string filename) {
	ofstream out(filename.c_str());
	out.seekp(0, ios::beg);
	out.write((char*)_data, _size);
	out.close();

	return this;
}

AudioSample* AudioSample::doClearScoring() {
	getScoring()->clear();

	return this;
}

AudioSample* AudioSample::doAddScoring (string person, double scoring) {
	Score msg = Score(person, scoring);
	getScoring()->push_back(msg);

	return this;
}

ScoringVector* AudioSample::getScoring() {
	if (!this->_scoring) {
		_scoring = new NetworkVector<Score>();
	}
	return this->_scoring;
}

AudioFormat AudioSample::getAudioFormat() {
	return _format;
}

AudioSample* AudioSample::doNormalize() {
	if (getAudioFormat() == AUDIO_FORMAT_WAVE) doSfbcep();
	if (getAudioFormat() == AUDIO_FORMAT_PRM) doNormFeat();

	return this;
}

AudioSample* AudioSample::doSfbcep() {
	if (_format != AUDIO_FORMAT_WAVE) return this;

	string inFile = "/tmp/in.wav";
	string outFile = "/tmp/out.prm";
	string params = "-F PCM16 -p 19 -e -D -A -k 0";
	string exec = "/usr/local/bin/sfbcep";
	stringstream cmd;
	cmd << exec << " "
			<< params << " "
			<< inFile << " "
			<< outFile;

	lockMutex();
	doSaveDataToFile(inFile);
	system(cmd.str().c_str());
	_format = AUDIO_FORMAT_PRM;
	doLoadDataFromFile(outFile);
	unlockMutex();

	return this;
}

AudioSample* AudioSample::doNormFeat() {
	if (_format != AUDIO_FORMAT_PRM) return this;

	string inFile = "/tmp/in.prm";
	string outFile = inFile;
	string outPath = "/tmp/";
	string params = "--config " + _folder + "cfg/NormFeat.cfg";
	string exec = "/usr/local/bin/NormFeat";
	stringstream cmd;
	cmd << exec
			<< " " << params
			<< " --inputFeatureFilename " << inFile
			<< " --featureFilesPath " << outPath;

	lockMutex();
	doSaveDataToFile(inFile);
	system(cmd.str().c_str());
	_format = AUDIO_FORMAT_NORM_PRM;
	doLoadDataFromFile(outFile);
	unlockMutex();

	return this;
}

AudioSample* AudioSample::doTest() {
	return doNormalize()->doComputeTest();
}

AudioSample* AudioSample::doComputeTest() {
	if (_format != AUDIO_FORMAT_NORM_PRM) return this;

	string exec = "/usr/local/bin/ComputeTest";
	string params = "--config " + _folder + "cfg/computeTest.cfg";
	string outFile = "/tmp/out.res";
	string ndxFile = _folder + "ndx/testing.ndx";
	string ndxList = _folder + "ndx/list.ndx";
	string normFile = _folder + "prm/testing.norm.prm";

	string cmd = exec
			+ " " + params
			+ " --outputFilename " + outFile
			+ " --ndxFilename " + ndxFile;

	lockMutex();
	//Testing if the file exists
	struct stat buffer;
	if (stat (ndxList.c_str(), &buffer) != 0) {
		unlockMutex();
		return this;
	}

	ifstream list(ndxList.c_str());
	ofstream ndx(ndxFile.c_str());
	ndx.seekp(0, ios::beg);
	string buff;
	getline(list,buff);
	ndx << "testing" << buff ;
	ndx.close();
	list.close();

	doSaveDataToFile(normFile);
	system(cmd.c_str());

	ifstream res(outFile.c_str());
	string person;
	double scoring;
	string dummy;
	if (res.is_open()) {
		doClearScoring();
		while (res.good()) {
			// Sex - Ignored
			if (!(res >> dummy)) break;
			// Person name
			if (!(res >> person)) break;
			// Number ... ?? - Ignored
			if (!(res >> dummy)) break;
			// File tested - Ignored
			if (!(res >> dummy)) break;
			// Scoring
			if (!(res >> scoring)) break;
			doAddScoring (person, scoring);
		}
	}
	unlockMutex();

	return this;
}

AudioSample* AudioSample::doTrain(string person) {
	return doNormalize()->doTrainTarget(person);
}

AudioSample* AudioSample::doTrainTarget(string person) {
	if (_format != AUDIO_FORMAT_NORM_PRM) return this;

	stringstream personDir;
	personDir << _folder << "prm/" << person << "/";
	stringstream ndxFile;
	ndxFile << _folder << "ndx/" << person << ".ndx";

	lockMutex();
	DIR* dir = opendir(personDir.str().c_str());
	if (dir == NULL) {
		mkdir(personDir.str().c_str(), S_IRWXG | S_IRWXO | S_IRWXU);
		ofstream ndx(ndxFile.str().c_str());
		ndx << person;
		ndx.close();
		dir = opendir(personDir.str().c_str());
		string f(_folder + "ndx/list.ndx");
		ofstream list(f.c_str(), ios::app);
		list << " " << person;
		list.close();
	}

	// We need a unique file name.
	int i;
	for (i = -2; readdir(dir) != NULL; i++);

	stringstream trainFile;
	trainFile << personDir.str() << person << "-" << i << ".norm.prm";
	string params = "--config " + _folder + "cfg/trainTarget.cfg --inputWorldFilename " + _folder + "gmm/world_independent_512.gmm";
	string exec = "/usr/local/bin/TrainTarget";
	stringstream cmd;
	cmd << exec
			<< " " << params
			<< " --targetIdList " << ndxFile.str()
			<< " --featureFilesPath " << personDir.str();

	doSaveDataToFile(trainFile.str());
	ofstream ndx(ndxFile.str().c_str(), ios::app);
	ndx << " " << person << "-" << i;
	ndx.close();
	system(cmd.str().c_str());
	unlockMutex();
	return this;
}


PeopleVector* AudioSample::getPeopleVector (string folder) {
	PeopleVector* vec = new PeopleVector();
	string filename(folder + "ndx/list.ndx");
	lockMutex();
	ifstream csv(filename.c_str());

	cout << "Reading file : " << filename << endl;

	string name;
	while (std::getline(csv,name,' ')) {
		if (!name.empty())
			vec->push_back(Person(name));
	}
	csv.close();
	unlockMutex();


	return vec;
}
