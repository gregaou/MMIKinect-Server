#ifndef MODULESERVER_H
#define MODULESERVER_H

#include <string>
#include <set>

class ModuleHandler;
class Packet;

class ModuleServer
{
public:
	ModuleServer ();
	~ModuleServer ();

	ModuleServer* addModuleHandler (ModuleHandler* module);
	ModuleServer* onPacketReceived (Packet* p);
	ModuleServer* onAllThreadsDone (ModuleHandler* module);

	ModuleServer* reload(std::string fromPath = "./lib/");
private:
	ModuleServer* load(std::string fromPath = "./lib/");
	std::set<ModuleHandler*>* getModuleHandlers();

	static ModuleServer* _instance;

	std::set<ModuleHandler*>* _modules;
};

#endif // MODULESERVER_H
