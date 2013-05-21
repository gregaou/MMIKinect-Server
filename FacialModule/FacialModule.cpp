#include "FacialModule.h"
#include "network/packet/scorepacket.h"
#include "network/packet/listpacket.h"
#include "network/packet/trainpacket.h"
#include "network/packet/person.h"

#include <fstream>
#include <iostream>
#include <sys/stat.h>

#define FICHIER (this->getFolder() + "identites.csv").c_str()
#define FACERECO (this->getFolder() + "facereco")

extern "C" IModule* create () {
    return new FacialModule();
}

extern "C" void destroy (IModule* module) {
    delete module;
}

const string FacialModule::getName() const { return "Facial Module"; }

/**
 * @brief FacialModule::onNewPacket
 * @param p Le packet reçu
 */
void FacialModule::onNewPacket(Packet *p) {
    if (!p) { *this << WARNING << "Empty packet!" << endl; return; }

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
        *this << DEBUG << "Facial Module : Nothing to do (id=" << p->getId() << ")" << endl;
        break;
    }
}

/**
 * @brief FacialModule::onListRequest
 * Listing de toutes les personnes contenues dans le fichier csv
 * @param p
 */
void FacialModule::onListRequest(Packet *p) {
    reloadFromCSVFile(FICHIER);
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
 * On cherche le dernier label du fichier csv
 * On vérifie que le nom envoyé n'est pas contenu dans le csv
 *
 * 2 - Non Présence de la personne
 * --- on ouvre le fichier csv en écriture et on enregiste la personne
 * --- sous la forme "dernierlabel;nom". On entraine ensuite le modele
 * --- et on met à jour les informations contenues dans le fichier csv
 *
 * 1 - Présence de la personne
 * --- on met juste à jour le modele en donnant l'image de personne
 * --- et son label associé
 *
 * @param p
 */
void FacialModule::onTrainRequest(Packet *p) {

    cout << "train request" << endl;

    Ptr<FaceRecognizer> _faceRecognizer;
    _faceRecognizer = loadFaceRecognizer(FACERECO);

    cout << "pass" << endl;

    TrainRequestPacket trp(p);
    string name = trp.getPerson()->getId(); //nom de la personne
    vector<uint8> img(trp.getTrainData(), trp.getTrainData() + trp.getTrainDataSize());
    Mat m = imdecode(img, CV_LOAD_IMAGE_GRAYSCALE);

    int labelName = labelFromName(FICHIER, name);

    vector<Mat> newImage;
    vector<int> newLabel;
    newImage.push_back(m);
    if(labelName == -1) {
        ofstream file(FICHIER, ios_base::app);
        if (!file) {
            string error_message = "(W) No valid input file was given, please check the given filename.";
            CV_Error(CV_StsBadArg, error_message);
        }
        int lastLabel = lastClassLabel(FICHIER);
        newLabel.push_back(lastLabel);
        file << lastLabel << ";" << name;
        file.close();
        _faceRecognizer->train(newImage, newLabel);
        reloadFromCSVFile(FICHIER);
    }

    else {
        newLabel.push_back(labelName);
        _faceRecognizer->update(newImage, newLabel);
    }

    _faceRecognizer->save(FACERECO);

    TrainResultPacket pReturn(p);
    pReturn.doSend();

}

/**
 * @brief FacialModule::onScoreRequest
 * On charge le modèle contenue dans le filename
 * On enregistre l'image de la personne dans une Mat
 * On tente la reconnaissance
 * @param p
 */
void FacialModule::onScoreRequest(Packet *p) {
    Ptr<FaceRecognizer> _faceRecognizer;
    _faceRecognizer = loadFaceRecognizer(FACERECO);

    vector<uint8> img(p->getData(), p->getData() + p->getBodySize());
    Mat m = imdecode(img, CV_LOAD_IMAGE_GRAYSCALE);

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
 * @brief FacialModule::reloadFromCSVFile
 * @param filename Le fichier source (du type Chemin;id)
 * @param separator Le separateur (ici ';')
 * On charge les noms et les id des personnes enregistrées
 * dans les structures prévues à cet effet
 */
void FacialModule::reloadFromCSVFile(const string &filename, const char separator) {
    _labels.clear();
    _names.clear();

    ifstream file(filename.c_str(), ios_base::in);
    if (!file) {
        string error_message = "(R) No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }

    string line, classlabel, name;
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, classlabel, separator);
        getline(liness, name);
        if(!classlabel.empty() && !name.empty()) {
            _labels.push_back(atoi(classlabel.c_str()));
            _names.insert(pair<int, string>(atoi(classlabel.c_str()), name));
        }
    }
}

/**
 * @brief FacialModule::loadFaceRecognizer
 * Si filename n'existe pas (donc un facerecognizer vierge)
 * on créer un nouveau FaceRecognizer et on le sauvegarde dans filename
 * @param filename
 *
 */
Ptr<FaceRecognizer> FacialModule::loadFaceRecognizer(const string& filename) {
    Ptr<FaceRecognizer> f;
    cout << "looking for facerecognizer" << endl;
    ifstream file(filename.c_str(), ios_base::in);
    if (!file) {
        cout << "no facerecognizer found" << endl;
        cout << "initializing new facerecognizer" << endl;
        f = createFisherFaceRecognizer();
        cout << "new facerecognizer declared" << endl;
        f->save(filename);
        cout << "new facerecognizer initialized" << endl;
    }
    else {
        cout << "facerecognizer found" << endl;
        f->load(filename);
        cout << "facerecognizer loaded" << endl;
    }
    return f;
}

/**
 * @brief FacialModule::lastClassLabel
 * @param filename
 * @return Le dernier label (id) contenu dans le fichier csv
 */
int FacialModule::lastClassLabel(const string &filename, const char separator) {

    ifstream file(filename.c_str(), ios_base::in);
    if (!file) {
        string error_message = "(R) No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }

    int min = -1;
    string line, classlabel, name;
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, classlabel, separator);
        getline(liness, name);
        min = (atoi(classlabel.c_str()) > min)? atoi(classlabel.c_str()):min;
    }
    return min + 1;
}

/**
 * @brief FacialModule::labelFromName
 * @param filename
 * @param findName
 * @return Le label (l'id) associé à la personne donnée en paramètre
 */
int FacialModule::labelFromName(const string& filename, string findName, const char separator) {

    ifstream file(filename.c_str(), ios_base::in);
    if (!file) {
        string error_message = "(R) No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }

    string line, classlabel, name;
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, classlabel, separator);
        getline(liness, name);
        if(name.compare(findName.c_str()) == 0) return atoi(classlabel.c_str());
    }
    return -1;
}
