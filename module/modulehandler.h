#ifndef MODULEHANDLER_H
#define MODULEHANDLER_H

#include <set>
#include <string>

class Packet;
class IModule;
class ModuleThread;
class ModuleServer;

typedef IModule* create_t();
typedef void destroy_t(IModule*);

class ModuleHandler
{
public:
	ModuleHandler(std::string libraryFilePath, ModuleServer* server = NULL);
	~ModuleHandler ();

	ModuleHandler* onNewPacket(Packet* p);
	ModuleHandler* onThreadFinished();

	IModule* getModuleInstance ();
private:
	void* getLibraryPointer ();
	create_t* getModuleConstructor ();
	destroy_t* getModuleDestructor ();

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
