#ifndef INETWORKMESSAGE_H
#define INETWORKMESSAGE_H

#include "typedef.h"

class INetworkMessage {
public:
	INetworkMessage () : _data(0), _size(0) {}
	//virtual INetworkMessage* fromNetworkMessage (uint8* data, int size) = 0;
	virtual uint8* toNetworkMessage () = 0;
	virtual int getNetworkMessageSize () = 0;
protected:
	uint8* _data;
	int _size;
};

#endif // INETWORKMESSAGE_H
