#include "MMIFaceRecognitionModule.h"

extern "C" IModule* create () {
    return new Test();
}

extern "C" void destroy (IModule* module) {
    delete module;
}

MMIFaceRecognitionModule::MMIFaceRecognitionModule(const string& filename) : IModule() {
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "Fichier introuvable.";
        CV_Error(CV_StsBadArg, error_message);
        _faceRecognizer = createFisherFaceRecognizer();
    }
    else _faceRecognizer->load(filename);
}

/**
 * @brief MMIFaceRecognitionModule::onNewPacket
 * @param p Le packet reçu
 */
void MMIFaceRecognitionModule::onNewPacket(Packet p) {
    if (!p) io::warn << "Empty packet!" << io::endl;
    else {
        io::info << "Packet received!" << io::endl;
        /* Switch sur la requete fournie dans le paquet */

        /* Exemple : cas de demande de reconnaissance
         *
         * 1. On charge le csv
         * vector<Mat> images;
         * vector<int> &labels;
         * map<int, string> names;
         * loadCSVFile("faces.csv", &images, &labels, &noms);
         *
         * 2. On train
         * _faceRecognizer->train(images, label);
         *
         * 3. On charge le haarCascade
         * CascadeClassifier haar_cascade;
         * haar_cascade.load("haarcascade_frontalface_alt.xml");
         *
         * 4. On charge les données du packet dans une Mat
         * Mat m = imread(<IMAGE A TRAITER, PEUT ETRE ENREGISTRER EN JPEG ?>, CV_LOAD_IMAGE_COLOR);
         * int prediction = model->predict(m);
         *
         * 5. Retour de prediction, a mapper sur une liste de prénoms
         * >> names.find(prediction);
         */
    }
}

/**
 * @brief MMIFaceRecognitionModule::loadCSVFile
 * @param filename Le fichier source (du type Chemin;id;nom)
 * @param images L'ensemble des images
 * @param labels L'ensemble des id
 * @param separator Le separateur (ici ';')
 */
void MMIFaceRecognitionModule::loadCSVFile(const string &filename, vector<Mat> &images, vector<int> &labels, map<int, string> &noms, char separator) {
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
        getline(liness, name);
        if(!path.empty() && !classlabel.empty()) {
            images.push_back(imread(path, 0));
            labels.push_back(atoi(classlabel.c_str()));
            if(!noms.find(atoi(classlabel.c_str()))) noms.insert(std::pair<int, string>(atoi(classlabel.c_str()), name));
        }
    }
}
