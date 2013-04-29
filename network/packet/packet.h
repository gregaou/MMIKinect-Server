#ifndef PACKET_H
#define PACKET_H

#include <vector>

#include "typedef.h"
#include "logger.h"
#include "tcpsocket.h"

enum PacketType {
	UNDEFINED_TYPE = 0x00,
	AUDIO_TYPE     = 0x10,
	VIDEO_TYPE     = 0x20,
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
	VIDEO_LISTING_RESULT   = VIDEO_TYPE | LISTING_RESULT
};

class Packet {
public:

	/**
		 * @brief Créer un Objet Packet
		 * @param d Un Stream qui lit/ecrit le Packet
		 */
	Packet(int socketDescriptor);

	/**
		 * @brief Retourne le type de message
		 * @return un uchar correspodant au type de message
		 */
	PacketType getType();

	/**
		 * @brief Retourne les données du packet
		 * @return un QByteArray contenant les données du Packet
		 */
	byte *getData();

	/**
		 * @brief Défini le type du packet
		 * @param type un uchar correspondant à un type de message
		 * @return Le packet d'origine
		 */
	Packet* setType(PacketType type);

	/**
		 * @brief Défini les données du packet
		 * @param data un QByteArray correspondant aux données du Packet
		 * @return Le packet d'origine
		 */
	Packet* setData(std::vector<byte>* data);

	/**
		 * @brief Envoie le packet correctement formaté
		 * @return Le packet d'origine
		 */
	Packet* doSend ();

	/**
		 * @brief Retourne la taille du message
		 * @return un uint correspodant à la taille des données
		 */
	uint getBodySize();

private:

	/**
		 * @brief Défini la taille des données
		 * @param type un uint correspondant à la taille des données
		 * @return Le packet d'origine
		 */
	Packet* setBodySize(uint size);

	/**
		 * @brief Lis et rempli les champs 'type' et 'bodySize'
		 * @return le packet d'origine
		 */
	Packet* doReadHeader();

	/**
		 * @brief Lis et rempli les données
		 * @return le packet d'origine
		 */
	Packet* doReadData();

	/**
		 * @brief Lis les données du Stream et les écris dans le buffer
		 * @param data le QByteArray utilisé comme buffer
		 * @param length la longueur des données lue (en octet).
		 * @return le packet d'origine
		 */
	//Packet* doReadStream(std::vector<byte>* data, int length);

	PacketType _type;
	static const int _headerSize = 5;
	uint _bodySize;
	std::vector<byte>* _pData;
	TcpSocket _pSocket;
};

#endif // PACKET_H
