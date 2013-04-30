#include "audiosample.h"

AudioSample::AudioSample(uint8* data, int size, AudioFormat format)
    : _scoring(0), _data(data), _size(size), _format(format) {}

AudioSample::AudioSample(string filename, AudioFormat format)
    : _scoring(0), _data(0), _format(format) {
    doLoadDataFromFile(filename);
}

AudioSample::~AudioSample() {
    delete(_data);
}

AudioSample* AudioSample::doLoadDataFromFile(string filename) {
    if (_data) delete(_data);

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
    if (_scoring) {
        _scoring->clear();
    }

    return this;
}

AudioSample* AudioSample::doAddScoring (string person, double scoring) {
    if (!_scoring) {
				_scoring = new ScoringVector();
    }
		Score msg = Score(person, scoring);
		_scoring->push_back(msg);

    return this;
}

ScoringVector* AudioSample::getScoring() {
    if (!this->_scoring) {
        doTest();
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

    doSaveDataToFile(inFile);
    system(cmd.str().c_str());
    _format = AUDIO_FORMAT_PRM;
    doLoadDataFromFile(outFile);

    return this;
}

AudioSample* AudioSample::doNormFeat() {
    if (_format != AUDIO_FORMAT_PRM) return this;

    string inFile = "/tmp/in.prm";
    string outFile = inFile;
    string outPath = "/tmp/";
    string params = "--config ./cfg/NormFeat.cfg";
    string exec = "/usr/local/bin/NormFeat";
    stringstream cmd;
    cmd << exec
        << " " << params
        << " --inputFeatureFilename " << inFile
        << " --featureFilesPath " << outPath;

    doSaveDataToFile(inFile);
    system(cmd.str().c_str());
    _format = AUDIO_FORMAT_NORM_PRM;
    doLoadDataFromFile(outFile);

    return this;
}

AudioSample* AudioSample::doTest() {
    return doNormalize()->doComputeTest();
}

AudioSample* AudioSample::doComputeTest() {
    if (_format != AUDIO_FORMAT_NORM_PRM) return this;

    string exec = "/usr/local/bin/ComputeTest";
    string params = "--config cfg/computeTest.cfg";
    string outFile = "/tmp/out.res";
    string ndxFile = "./ndx/testing.ndx";
    string ndxList = "./ndx/list.ndx";
    string normFile = "./prm/testing.norm.prm";

    stringstream cmd;
    cmd << exec
        << " " << params
        << " --outputFilename " << outFile
        << " --ndxFilename " << ndxFile;

    ifstream list(ndxList.c_str());
    ofstream ndx(ndxFile.c_str());
    ndx.seekp(0, ios::beg);
    string buff;
    getline(list,buff);
    ndx << "testing" << buff ;
    ndx.close();
    list.close();

    doSaveDataToFile(normFile);
    system(cmd.str().c_str());

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

    return this;
}

AudioSample* AudioSample::doTrain(string person) {
    return doNormalize()->doTrainTarget(person);
}

AudioSample* AudioSample::doTrainTarget(string person) {
    if (_format != AUDIO_FORMAT_NORM_PRM) return this;

    stringstream personDir;
    personDir << "./prm/" << person << "/";
    stringstream ndxFile;
    ndxFile << "./ndx/" << person << ".ndx";

    DIR* dir = opendir(personDir.str().c_str());
    if (dir == NULL) {
        mkdir(personDir.str().c_str(), S_IRWXG | S_IRWXO | S_IRWXU);
        ofstream ndx(ndxFile.str().c_str());
        ndx << person;
        ndx.close();
        dir = opendir(personDir.str().c_str());
        ofstream list("./ndx/list.ndx", ios::app);
        list << " " << person;
        list.close();
    }

    // We need a unique file name.
    int i;
    for (i = -2; readdir(dir) != NULL; i++);

    stringstream trainFile;
    trainFile << personDir.str() << person << "-" << i << ".norm.prm";
    string params = "--config ./cfg/trainTarget.cfg --inputWorldFilename ./gmm/world_independent_512.gmm";
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
    return this;
}

string AudioSample::scoringToString (ScoringVector* scoring) {
    stringstream str("");
    for (int i = 0; i < (scoring ? scoring->size() : 0); i++) {
				Score score = (Score&) scoring->at(i);
				str << "\"" << score.getPerson().getId() << "\""
						<< "scored " << score.getScore() << endl;
    }
    return str.str();
}
