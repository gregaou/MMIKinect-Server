#ifndef PACKET_H
#define PACKET_H

#include <vector>

#include "typedef.h"
#include "tcpsocket.h"
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
		byte getType();

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
		Packet* setType(byte type);

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

		byte _type;
		static const int _headerSize = 5;
		uint _bodySize;
		std::vector<byte>* _pData;
		TcpSocket _pSocket;

};

#endif // PACKET_H
