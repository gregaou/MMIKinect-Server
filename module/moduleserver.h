#ifndef MODULESERVER_H
#define MODULESERVER_H

#include <string>
#include <set>

#include "logger.h"

class ModuleHandler;
class Packet;

class ModuleServer : public Loggable
{
public:
	ModuleServer ();
	~ModuleServer ();

	const std::string getName() const;

	ModuleServer* addModuleHandler (ModuleHandler* module);
	ModuleServer* onNewPacket (Packet* p);
	ModuleServer* onAllThreadsDone (ModuleHandler* module);

	ModuleServer* reload(std::string fromPath = "./lib/");
private:
	ModuleServer* load(std::string fromPath = "./lib/");
	std::set<ModuleHandler*>* getModuleHandlers();

	static ModuleServer* _instance;

	std::set<ModuleHandler*>* _modules;
};

#endif // MODULESERVER_H
