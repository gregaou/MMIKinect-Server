#include "FacialModule.h"
#include "packet/scorepacket.h"
#include "packet/listpacket.h"
#include "packet/trainpacket.h"
#include "packet/person.h"

#include <fstream>
#include <iostream>
#include <sys/stat.h>

extern "C" IModule* create () {
    return new FacialModule();
}

extern "C" void destroy (IModule* module) {
    delete module;
}

const std::string FacialModule::getName() const { return "Facial Module"; }

FacialModule::FacialModule() : IModule() {

    _faceRecognizer = createFisherFaceRecognizer();
    _haar_cascade.load("haarcascade_frontalface_alt.xml");
    loadCSVFile("FacesDB/faces.csv");
    _faceRecognizer->train(_images, _labels);

}

/**
 * @brief FacialModule::onNewPacket
 * @param p Le packet reçu
 */
void FacialModule::onNewPacket(Packet *p) {
		if (!p) { *this << WARNING << "Empty packet!" << std::endl; return; }

    uint16 action = p->getType() & 0x0F;
    uint16 type = p->getType() & 0xF0;

    if (type != VIDEO_TYPE && type != BROADCAST_TYPE) { return; }

    switch (action) {
    case LISTING_REQUEST :
        onListRequest(p);
        break;
    case TRAINING_REQUEST :
        onTrainRequest(p);
        break;
    case SCORING_REQUEST :
        onScoreRequest(p);
        break;
    default :
				*this << DEBUG << "Facial Module : Nothing to do (id=" << p->getId() << ")" << std::endl;
        break;
    }
}

/**
 * @brief FacialModule::onListRequest
 * @param p
 */
void FacialModule::onListRequest(Packet *p) {
    map<int, string>::iterator iter;
    PeopleVector *vp = new PeopleVector();
    for(iter = _names.begin(); iter != _names.end(); ++iter) {
        vp->push_back(Person(iter->second));
    }

    ListResultPacket pReturn(p);
    pReturn.setPeopleVector(vp)->doSend();
}

/**
 * @brief FacialModule::onTrainRequest
 * @param p
 */
void FacialModule::onTrainRequest(Packet *p) {
    ofstream f, updatingFile;
    ostringstream o, x;
    TrainRequestPacket trp(p);
    string name = trp.getPerson()->getId();
    string dir = string("FacesDB/" + name);
    updatingFile.open("FacesDB/faces.csv", ofstream::app);
    if(!dirExists(dir)) mkdir(dir.c_str(), S_IRWXG | S_IRWXO | S_IRWXU);
    int lastFile = getFilesCount(dir);
    o << lastFile;
    string fileName(o.str() + ".pgm");
    f.open(fileName.c_str());
    f.write((char*)trp.getTrainData(), trp.getTrainDataSize());
    f.close();
    int classIndex = keyFromValue(name);
    x << ((classIndex != -1)? classIndex:0);
    string toWrite(dir + "/" + fileName + ";" + x.str());
    updatingFile.write(toWrite.c_str(), toWrite.length());
    updatingFile.close();

    stringstream liness(toWrite);
    string path, label;
    getline(liness, path, ';');
    getline(liness, label);
    _images.push_back(imread(path, 0));
    _labels.push_back(atoi(label.c_str()));

    _faceRecognizer->update(_images, _labels);

    TrainResultPacket pReturn(p);
    pReturn.doSend();
}

/**
 * @brief FacialModule::onScoreRequest
 * @param p
 */
void FacialModule::onScoreRequest(Packet *p) {

    ofstream f;
    f.open("sample.jpeg");
    f.write((char*)p->getData(), p->getBodySize());
    f.close();
    Mat m = imread("sample.jpeg", CV_LOAD_IMAGE_GRAYSCALE);

    //On a recuperé la prediction;
    double confidence = 0.0;
    int predictedLabel = -1;
    _faceRecognizer->predict(m, predictedLabel, confidence);

    //On renvoie le paquet
    ScoreResultPacket pReturn(p);
    ScoringVector *score = new ScoringVector();
    if(predictedLabel != -1) score->push_back(Score(_names[predictedLabel], confidence));
    pReturn.setScoringVector(score)->doSend();

}

/**
 * @brief FacialModule::loadCSVFile
 * @param filename Le fichier source (du type Chemin;id)
 * @param images L'ensemble des images
 * @param labels L'ensemble des id
 * @param separator Le separateur (ici ';')
 */
void FacialModule::loadCSVFile(const string &filename, char separator) {
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
    string line, path, classlabel, name;
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);
        int nameFirstIndex = path.find('/');
        int nameLastIndex = path.rfind('/');
        name = path.substr(nameFirstIndex, nameLastIndex - nameFirstIndex);
        if(!path.empty() && !classlabel.empty()) {
            _images.push_back(imread(path, 0));
            _labels.push_back(atoi(classlabel.c_str()));
            if(_names.find(atoi(classlabel.c_str())) == _names.end()) _names.insert(std::pair<int, string>(atoi(classlabel.c_str()), name));
        }
    }
}

/**
 * @brief FacialModule::loadFaceRecognizer
 * @param filename
 */
void FacialModule::loadFaceRecognizer(const string& filename) {
    ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "Fichier introuvable.";
        CV_Error(CV_StsBadArg, error_message);
    }
    else _faceRecognizer->load(filename);
}
