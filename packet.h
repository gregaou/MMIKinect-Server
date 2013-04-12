#ifndef PACKET_H
#define PACKET_H

#include <QDataStream>
#include <QtEndian>
#include <QDebug>

class Packet {

public:

    /**
     * @brief Créer un Objet Packet
     * @param d Un Stream qui lit/ecrit le Packet
     */
    Packet(QIODevice *d);

    /**
     * @brief Retourne le type de message
     * @return un uchar correspodant au type de message
     */
    uchar getType();

    /**
     * @brief Retourne les données du packet
     * @return un QByteArray contenant les données du Packet
     */
    QByteArray *getData();

    /**
     * @brief Défini le type du packet
     * @param type un uchar correspondant à un type de message
     * @return Le packet d'origine
     */
    Packet* setType(uchar type);

    /**
     * @brief Défini les données du packet
     * @param data un QByteArray correspondant aux données du Packet
     * @return Le packet d'origine
     */
    Packet* setData(QByteArray* data);

private:

    /**
     * @brief Retourne la taille du message
     * @return un uint correspodant à la taille des données
     */
    uint getBodySize();

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
     * @return
     */
    Packet* doReadStream(QByteArray *data, int length);

    uchar _type;
    static const int _headerSize = 5;
    uint _bodySize;
    QByteArray *_pData;
    QDataStream *_pStream;

};

#endif // PACKET_H
