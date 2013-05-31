#ifndef MODULEHANDLER_H
#define MODULEHANDLER_H

#include <set>
#include <string>

#include <dlfcn.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include "tools/logger.h"

class Packet;
class IModule;
class ModuleThread;
class ModuleServer;

typedef IModule* create_t();
typedef void destroy_t(IModule*);

class ModuleHandler : public Loggable
{
public:
	ModuleHandler(std::string libraryFilePath, ModuleServer* server = NULL);
	~ModuleHandler ();
	virtual const string getName () const;

	ModuleHandler* onNewPacket(Packet* p);
	ModuleHandler* onThreadFinished();

	IModule* getModuleInstance ();
private:
	void* getLibraryPointer ();
	create_t* getModuleConstructor ();
	destroy_t* getModuleDestructor ();

	std::string getModuleFolder ();

	std::set<ModuleThread*>* getThreads();

	std::string _libraryPath;
	void* _libraryPtr;
	create_t* _moduleConstructor;
	destroy_t* _moduleDestructor;
	IModule* _moduleInstance;

	ModuleServer* _moduleServer;
	std::set<ModuleThread*>* _threads;
	pthread_mutex_t _mutex;
};

#endif // MODULEHANDLER_H
