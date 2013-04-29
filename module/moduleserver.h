#ifndef MODULESERVER_H
#define MODULESERVER_H

#include <vector>

#include <dirent.h>

#include "logger.h"
#include "packet/packet.h"
#include "moduleloader.h"
#include "singleton.h"

class ModuleServer : public Singleton<ModuleServer>
{
	friend class Singleton<ModuleServer>;

public:
	static std::string* getPath ();
	static void setPath(std::string* path);
	ModuleServer* addModuleLoader (ModuleLoader* module);
	ModuleServer* onPacketReceived (Packet* p);

private:
	ModuleServer ();
	~ModuleServer ();
	std::vector<ModuleLoader*>* getModules();
	std::vector<ModuleThread*>* getModuleThreads();

	static ModuleServer* _instance;
	static std::string* _path;

	std::vector<ModuleLoader*>* _modules;
	std::vector<ModuleThread*>* _threads;
};

#endif // MODULESERVER_H
