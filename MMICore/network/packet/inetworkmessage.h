#ifndef INETWORKMESSAGE_H
#define INETWORKMESSAGE_H

#include "tools/typedef.h"

class INetworkMessage {
public:
	INetworkMessage () {}
	virtual ~INetworkMessage () { }
	virtual uint8* toNetworkMessage () = 0;
	virtual int getNetworkMessageSize () = 0;
};

#endif // INETWORKMESSAGE_H
