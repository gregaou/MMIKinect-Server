#ifndef MESSAGE_H
#define MESSAGE_H

#include <QByteArray>

class Message
{
public:
	Message();

private:
	int _type;
	int _size;
	QByteArray *_data;
};

#endif // MESSAGE_H
