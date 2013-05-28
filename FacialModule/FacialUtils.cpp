#include "FacialUtils.h" 


void FacialUtils::newUser(const string& filename, const int id, const string name) {
    ofstream file(filename.c_str(), ios_base::app);
    if (!file) {
        string error_message = "(W) No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
    file << id << ";" << name << endl;
    file.close();
}

/**
 * @brief FacialUtils::reloadFromCSVFile
 * @param filename Le fichier CSV source
 * @param separator Le separateur (ici ';')
 * On charge les noms et les id des personnes enregistrées
 * dans les structures prévues à cet effet
 */

map<int, string> FacialUtils::reloadFromCSVFile(const string &filename, const char separator) {

    map<int, string> _names;

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
            _names.insert(pair<int, string>(atoi(classlabel.c_str()), name));
        }
    }
    file.close();

    return _names;
}

/**
 * @brief FacialUtils::loadFaceRecognizer
 * Si filename n'existe pas (donc un facerecognizer vierge)
 * on créer un nouveau FaceRecognizer et on le sauvegarde dans filename
 * @param filename
 *
 */
void FacialUtils::loadFaceRecognizer(Ptr<FaceRecognizer> &f, const string& filename) {
    f = createLBPHFaceRecognizer();
    cout << "Looking for Face Recognizer..." << endl;
    ifstream file(filename.c_str(), ios_base::in);
    if (!file) {
        cout << "!! Face Recognizer not found !!" << endl;
        cout << "Initializing new Face Recognizer..." << endl;
        f->save(filename);
    }
    else {
        cout << "Face Recognizer found (" << filename << ")" << endl;
        try {
            f->load(filename);
        } catch(std::exception& e) { cout << e.what() << endl; }
    }
    file.close();
}

/**
 * @brief FacialUtils::lastClassLabel
 * @param filename
 * @return Le dernier label (id) contenu dans le fichier csv
 */
int FacialUtils::lastClassLabel(const string &filename, const char separator) {

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
    file.close();
    return min + 1;
}

/**
 * @brief FacialUtils::labelFromName
 * @param filename
 * @param findName
 * @return Le label (l'id) associé à la personne donnée en paramètre
 */
int FacialUtils::labelFromName(const string& filename, string findName, const char separator) {

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
    file.close();
    return -1;
}

