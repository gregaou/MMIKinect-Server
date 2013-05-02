#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <cstring>

#include "typedef.h"
#include "inetworkmessage.h"
#include "networkvector.h"

class Person : public INetworkMessage
{
public:
	Person(std::string id);
	Person(char* data, int size);
	Person(const Person& p);

	Person* setId(std::string id);
	std::string getId ();

	static Person* fromNetworkMessage(uint8 *data);

	uint8* toNetworkMessage ();
	int getNetworkMessageSize ();

private :
	std::string _id;
};

typedef NetworkVector<Person> PeopleVector;

#endif // PERSON_H
