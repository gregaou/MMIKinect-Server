#ifndef PACKET_H
#define PACKET_H

#include <string>
#include <sstream>
#include <vector>

#include "typedef.h"
#include "logger.h"
#include "tcpsocket.h"

#define PACKET_VERSION 0x01

enum PacketType {
	UNDEFINED_TYPE = 0x00,
	AUDIO_TYPE     = 0x10,
	VIDEO_TYPE     = 0x20,
	HISTOGRAM_TYPE = 0x30,
	BROADCAST_TYPE = 0xF0,

	UNDEFINED_ACTION = 0x00,
	TRAINING_REQUEST = 0x01,
	TRAINING_RESULT  = 0x02,
	SCORING_REQUEST  = 0x03,
	SCORING_RESULT   = 0x04,
	LISTING_REQUEST  = 0x05,
	LISTING_RESULT   = 0x06,

	AUDIO_TRAINING_REQUEST = AUDIO_TYPE | TRAINING_REQUEST,
	AUDIO_TRAINING_RESULT  = AUDIO_TYPE | TRAINING_RESULT,
	AUDIO_SCORING_REQUEST  = AUDIO_TYPE | SCORING_REQUEST,
	AUDIO_SCORING_RESULT   = AUDIO_TYPE | SCORING_RESULT,
	AUDIO_LISTING_REQUEST  = AUDIO_TYPE | LISTING_REQUEST,
	AUDIO_LISTING_RESULT   = AUDIO_TYPE | LISTING_RESULT,

	VIDEO_TRAINING_REQUEST = VIDEO_TYPE | TRAINING_REQUEST,
	VIDEO_TRAINING_RESULT  = VIDEO_TYPE | TRAINING_RESULT,
	VIDEO_SCORING_REQUEST  = VIDEO_TYPE | SCORING_REQUEST,
	VIDEO_SCORING_RESULT   = VIDEO_TYPE | SCORING_RESULT,
	VIDEO_LISTING_REQUEST  = VIDEO_TYPE | LISTING_REQUEST,
	VIDEO_LISTING_RESULT   = VIDEO_TYPE | LISTING_RESULT,

	HISTOGRAM_TRAINING_REQUEST = HISTOGRAM_TYPE | TRAINING_REQUEST,
	HISTOGRAM_TRAINING_RESULT  = HISTOGRAM_TYPE | TRAINING_RESULT,
	HISTOGRAM_SCORING_REQUEST  = HISTOGRAM_TYPE | SCORING_REQUEST,
	HISTOGRAM_SCORING_RESULT   = HISTOGRAM_TYPE | SCORING_RESULT,
	HISTOGRAM_LISTING_REQUEST  = HISTOGRAM_TYPE | LISTING_REQUEST,
	HISTOGRAM_LISTING_RESULT   = HISTOGRAM_TYPE | LISTING_RESULT
};

class Packet : public Loggable {
public:

	/**
		 * @brief Créer un Objet Packet
		 * @param d Un Stream qui lit/ecrit le Packet
		 */
	Packet(int socketDescriptor);

	/**
	 * @brief Créé une copie d'un Packet
	 * @param p le Packet à copier
	 */
	Packet(Packet& p);

	/**
	 *	@brief Detruit le packet.
	 */
	~Packet();

	/**
	 * @brief Retourne le nom du packet
	 * @return un string correponsant au packet
	 */
	const std::string getName() const;

	/**
	 * @brief Retourne la version de la trame
	 * @return un uint8 correspondant à la version de la trame
	 */
	uint8 getVersion();

	/**
		 * @brief Retourne le type de message
		 * @return un uchar correspodant au type de message
		 */
	uint8 getType();

	/**
	 * @brief Retourne l'identifiant de la trame
	 * @return un id (unsigned short int) correspondant au type de message
	 */
	uint16 getId();

	/**
		 * @brief Retourne les données du packet
		 * @return un Quint8Array contenant les données du Packet
		 */
	uint8 *getData();

	/**
	 * @brief Défini la version du packet
	 * @param version un uint8 correspondant à la version du packet
	 * @return Le packet d'origine
	 */
	Packet* setVersion(uint8 version);

	/**
		 * @brief Défini le type du packet
		 * @param type un uchar correspondant à un type de message
		 * @return Le packet d'origine
		 */
	Packet* setType(uint8 type);

	/**
	 * @brief Défini l'identifiant du packet
	 * @param identifiant un id correspondant à l'identifiant du message
	 * @return Le packet d'origine
	 */
	Packet* setId(uint16 identifiant);

	/**
		 * @brief Défini les données du packet
		 * @param data un vecteur correspondant aux données du Packet
		 * @return Le packet d'origine
		 */
	Packet* setData(std::vector<uint8>* data = NULL);

	/**
	 * @brief Défini les données du packet
	 * @param data un tableau de uint8 correspondant aux données du Packet
	 * @param size la taille du tableau de données
	 * @return  Le packet d'origine
	 */
	Packet* setData(uint8* data, int size);

	/**
		 * @brief Envoie le packet correctement formaté
		 * @return Le packet d'origine
		 */
	Packet* doSend ();

	/**
		 * @brief Retourne la taille du message
		 * @return un uint32 correspodant à la taille des données
		 */
	uint32 getBodySize();

private:

	/**
		 * @brief Défini la taille des données
		 * @param type un uint32 correspondant à la taille des données
		 * @return Le packet d'origine
		 */
	Packet* setBodySize(uint32 size);

	/**
	 * @brief Lis et rempli le champ 'version'
	 * @return le packet d'origine
	 */
	Packet* doReadVersion();

	/**
		 * @brief Lis et rempli les champs 'type' , 'id' et 'bodySize'
		 * @return le packet d'origine
		 * @throw NetworkException une exception lorsque la version n'est pas correcte.
		 */
	Packet* doReadHeader() throw (NetworkException);

	/**
		 * @brief Lis et rempli les données
		 * @return le packet d'origine
		 */
	Packet* doReadData();

	/**
		 * @brief Lis les données du Stream et les écris dans le buffer
		 * @param data le Quint8Array utilisé comme buffer
		 * @param length la longueur des données lue (en octet).
		 * @return le packet d'origine
		 */
	//Packet* doReadStream(std::vector<uint8>* data, int length);

	uint8 _version;
	uint8 _type;
	uint16 _id;
	uint32 _bodySize;
	static const int _headerSize = 8;
	uint8* _pData;
	TcpSocket _pSocket;
};

#endif // PACKET_H
