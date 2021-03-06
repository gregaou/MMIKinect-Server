/**
 * \file audiosample.h
 * \brief Header de la classe AudioSample
 * \author Théo GOUDOUT
 * \version 0.1
 * \date 23 Avril 2013
 *
 * Ce fichier contient les entetes des methodes de la classe AudioSample
 * Ainsi que toutes les enumération et déclaration de types associés à celle-ci.
 *
 */

#ifndef AUDIOSAMPLE_H
#define AUDIOSAMPLE_H

#include <string>
#include <utility>
#include <vector>

#include <iostream>
#include <fstream>
#include <sstream>
#include <ios>

#include <cstdlib>
#include <cstring>

#include <dirent.h>
#include <sys/stat.h>

#include "tools/typedef.h"
#include "network/packet/score.h"

using namespace std;

/**
 * \enum AudioFormat
 * \brief L'enum AudioFormat
 *
 *  Cette énumération liste tous les formats audio gérés par la classe AudioSample.
 *
 */
enum AudioFormat {
	AUDIO_FORMAT_WAVE, /*!< Format Wave. Le format des données à la reception d'un paquet/ouverture d'un fichier. */
	AUDIO_FORMAT_PRM, /*!< Format PRM. Format de sortie du programme sfbcep. */
	AUDIO_FORMAT_NORM_PRM /*!< Format PRM normalisé. Format utilisé pour l'entrainement et les test.*/
};

/**
 * \class AudioSample
 * \brief L'enum AudioSample
 *
 * La classe gérant la reconnaissance vocale.
 * Elle permet soit de reconnaitre une personne,
 * soit d'entrainer le système à reconnaitre une personne.
 *
 */
class AudioSample
{
private:
	ScoringVector* _scoring; /*!< Le scoring resultant de la reconnaissance vocale.*/
	uint8* _data; /*!< Un tableau de données correspondant au contenu audio.*/
	int _size; /*!< La taille du tableau de données (necesaire pour la lecture/ecriture de fichier).*/
	AudioFormat _format; /*!< Le format des données audio de cette instance.*/
	string _folder;

	/**
	 * \brief Charge les données d'un fichier.
	 *
	 * Cette fonction lit les données brute dans le fichier passé en paramètre,
	 * et les stocke dans le tableau de donnée. Elle met aussi à jour la taille
	 * des données, mais ne s'occupe pas du format.
	 * Si des données étaient présente dans le tableau, elles seront supprimées.
	 *
	 * \param filename Le nom du fichier d'ou l'on charge les données.
	 * \return L'instance courante.
	 */
	AudioSample* doLoadDataFromFile (string filename);
	/**
	 * \brief Enregistre les données dans un fichier.
	 *
	 *  Cette fonction écrit les données brute dans le fichier passé en paramètre.
	 *
	 *
	 * \param filename Le nom du fichier ou l'on enregistre les données.
	 * \return L'instance courante.
	 */
	AudioSample* doSaveDataToFile (string filename);

	/**
	 * \brief Mutex bloquant lors d'une ecriture.
	 */
	static pthread_mutex_t* _mutex;

protected:
	/**
	 * \brief Accesseur du format audio d'une instance de AudioSample
	 * \return L'AudioFormat associé à l'instance.
	 */
	AudioFormat getAudioFormat();

	/**
	 * \brief Ajoute un scoring à l'instance.
	 *
	 * Cette methode ajoute un scoring dans le tableau des scoring.
	 * Si le tableau n'est pas initialisé, cette methode en créé un.
	 *
	 * @param person Un string représentant une personne enregistrée.
	 * @param scoring Le scoring associé à la personne.
	 * @return L'instance courante.
	 */
	AudioSample* doAddScoring (string person, double scoring);

	/**
	 * \brief Suppression du scoring actuel.
	 *
	 * Si le scoring existe, on le supprime.
	 *
	 * \return L'instance courante.
	 */
	AudioSample* doClearScoring();

	/**
	 * \brief Convertie les données au fromat AUDIO_FORMAT_NORM_PRM.
	 *
	 * Après l'appel à cette methode, les données seront au format AUDIO_FORMAT_NORM_PRM,
	 * peut importe leur format avant l'appel.
	 *
	 * \return L'instance courante.
	 */
	AudioSample* doNormalize();
	/**
	 * \brief Convertie les données du format AUDIO_FORMAT_WAVE au format AUDIO_FORMAT_PRM.
	 * \return L'instance courante.
	 */
	AudioSample* doSfbcep();
	/**
	 * \brief Convertie les données du format AUDIO_FORMAT_PRM au format AUDIO_FORMAT_NORM_PRM.
	 * \return L'instance courante.
	 */
	AudioSample* doNormFeat();

	/**
	 * \brief Lance un test de reconnaisance vocale.
	 *
	 * Avant l'appel de cette methode, le format audio doit etre
	 * AUDIO_FORMAT_NORM_PRM. Une fois cette methode executée,
	 * un scoring sera établi.
	 *
	 * \return L'instance courante.
	 */
	AudioSample* doComputeTest();
	/**
	 * \brief Entraine le système pour une personne donnée.
	 *
	 * Avant l'appel de cette methode, le format audio doit etre
	 * AUDIO_FORMAT_NORM_PRM. Un appel à cette methode enregistre
	 * les données d'entrainement dans le système, et (re)calcule un modèle
	 * d'identification pour la personne choisie.
	 *
	 * \param person La personne dont on veut calculer/affiner le modèle.
	 * \return L'instance courante.
	 */
	AudioSample* doTrainTarget(string person);

	/**
	 * \brief Bloque un mutex
	 */
	static void lockMutex();

	/**
	 * \brief Débloque un mutex déjà bloqué.
	 */
	static void unlockMutex();

public:
	/**
	 * \brief Constructeur
	 *
	 * Ce constructeur prend en paramètre toutes les informations necessaires
	 * à une instance de AudioSample, et initialise tous les champs de l'instance.
	 *
	 * \param data Un tableau contenant des informations audio.
	 * \param size La taille du tableau de données.
	 * \param format Le format audio des données.
	 */
	AudioSample(uint8* data, int size, AudioFormat format, string folder = "./");
	/**
	 * \brief Constructeur
	 *
	 * Ce constructeur va lire les données du fichier passé en paramètre, et
	 * les stockera dans un tableau de données. Le format audio passé en paramètre
	 * n'est pas vérifié.
	 *
	 * \param filename Le nom du fichier à charger
	 * \param format Le format audio du fichier.
	 */
	AudioSample(string filename, AudioFormat format, string folder = "./");
	/**
	 * \brief Destructeur
	 *
	 * Delete du tableau des données.
	 */
	~AudioSample();

	/**
	 * \brief Retourne le scoring des données auprès du système.
	 *
	 * Si le scoring a été calculé auparavant, on retourne ce scoring.
	 * Sinon, on formate les données, et on calcule le scoring.
	 *
	 * \return Un scoring des données (possiblement vide).
	 */
	ScoringVector* getScoring();
	/**
	 * \brief Lance une procedure d'identification.
	 *
	 * On formate les données, on supprime l'ancien scoring,
	 * et on calcule le nouveau scoring auprès du système.
	 *
	 * @return L'instance courante.
	 */
	AudioSample* doTest();
	/**
	 * \brief Lance une procédure d'entrainement du système.
	 *
	 * Cette methode formate les données, enregistre celles-ci auprès du système,
	 * et (re)calcule le modèle associée à une personne.
	 *
	 * \param person La personne dont on veut calculer/affiner le modèle.
	 * \return L'instance courante.
	 */
	AudioSample* doTrain(string person);

	/**
	 * \brief Retourne un vecteur des personnes précédemment enregistrées.
	 * \param folder le dossier dans lequel les fichiers sont situés.
	 * \return Un vecteur de personne. Peut etre vide.
	 */
	static PeopleVector* getPeopleVector (string folder);
};

#endif // AUDIOSAMPLE_H
