#ifndef INETWORKMESSAGE_H
#define INETWORKMESSAGE_H

#include "typedef.h"

class INetworkMessage {
public:
	INetworkMessage () : _data(0), _size(0) {}
	//virtual INetworkMessage* fromNetworkMessage (byte* data, int size) = 0;
	virtual byte* toNetworkMessage () = 0;
	virtual int getNetworkMessageSize () = 0;
protected:
	byte* _data;
	int _size;
};

#endif // INETWORKMESSAGE_H
