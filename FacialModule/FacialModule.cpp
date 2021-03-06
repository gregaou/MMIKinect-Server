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

    map<int, string>::iterator iter;
    PeopleVector *vp = new PeopleVector();

    map<int, string> names = FacialUtils::reloadFromCSVFile(FICHIER);

    for(iter = names.begin(); iter != names.end(); ++iter) {
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
 * 2 - Absence de la personne
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

    *this << DEBUG << "Train Request" << endl;

    *this << DEBUG << "Loading FaceRecognizer..." << endl;
    FacialUtils::loadFaceRecognizer(_faceRecognizer, FACERECO);
    *this << DEBUG << "FaceRecognizer loaded" << endl;
    TrainRequestPacket trp(p);
    string name = trp.getPerson()->getId(); //nom de la personne
    vector<uint8> img(trp.getTrainData(), trp.getTrainData() + trp.getTrainDataSize());

    char a[50];

    Mat m = imdecode(img, CV_LOAD_IMAGE_GRAYSCALE);

    int labelName = FacialUtils::labelFromName(FICHIER, name);

    vector<Mat> newImage;
    vector<int> newLabel;

    newImage.push_back(m);

    if(labelName == -1) {
        labelName = FacialUtils::lastClassLabel(FICHIER);
        FacialUtils::newUser(FICHIER, labelName, name);
    }

    newLabel.push_back(labelName);

    _faceRecognizer->update(newImage, newLabel);

    *this << DEBUG << "Train Request complete" << endl;

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

    ScoreResultPacket pReturn(p);
    ScoringVector *score = new ScoringVector();

    FacialUtils::loadFaceRecognizer(_faceRecognizer, FACERECO);

    vector<uint8> img(p->getData(), p->getData() + p->getBodySize());

    Mat m = imdecode(img, CV_LOAD_IMAGE_GRAYSCALE);

    //On recupère la prediction;
    double confidence = 0.0;
    int predictedLabel = -1;

    try {
        _faceRecognizer->predict(m, predictedLabel, confidence);
    } catch(Exception) {
        *this << WARNING << "Impossible de prédire un score" << endl;
        score->push_back(Score("Inconnu", confidence));
    }

    map<int, string> names = FacialUtils::reloadFromCSVFile(FICHIER);

    if(predictedLabel != -1) {
        score->push_back(Score(names[predictedLabel], confidence));
        //*this << INFO << names[predictedLabel] << confidence << endl;
    }

    pReturn.setScoringVector(score)->doSend();
}
