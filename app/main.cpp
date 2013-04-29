#include "server.h"

int main()
{
	Server s;
	s.doWork();
	ModuleServer::kill();

	return 0;
}

