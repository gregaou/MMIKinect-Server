#include "message.h"

Message::Message() : _type(0), _size(0), _data(0)
{
}

//Message* Message::getMessage()
//{
//	if(!_data) {
//		Server::getInstance()->getMessage();
//	}
//	return this;
//}
