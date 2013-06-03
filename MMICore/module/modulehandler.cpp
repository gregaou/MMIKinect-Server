#include "modulehandler.h"

#include "tools/config.h"

#include "module/imodule.h"
#include "module/moduleexception.h"
#include "module/moduleserver.h"
#include "module/modulethread.h"

ModuleHandler::ModuleHandler (std::string libraryPath, ModuleServer* server) :
	_libraryPath(libraryPath),
	_libraryPtr(0),
	_moduleConstructor(0),
	_moduleDestructor(0),
	_moduleInstance(0),
	_moduleServer(server),
	_threads(0)
{ pthread_mutex_init(&_mutex, NULL); }

ModuleHandler::~ModuleHandler() {
	if (_threads) {
		for (std::set<ModuleThread*>::iterator it = _threads->begin();
				 it != _threads->end(); ++it) {
			(*it)->wait();
			delete (*it);
		}
		delete _threads;
	}
	if (_moduleInstance)
		(getModuleDestructor())(_moduleInstance);
	if (_libraryPtr) {
		*this << DEBUG << "Unloading module : " << _libraryPath << endl;
		dlclose(_libraryPtr);
	}
}

const string ModuleHandler::getName () const {
	return "ModuleHandler";
}


ModuleHandler* ModuleHandler::onNewPacket(Packet* p) {
	ModuleThread* t = new ModuleThread(getModuleInstance(),this);
	pthread_mutex_lock(&_mutex);
	getThreads()->insert(t);
	pthread_mutex_unlock(&_mutex);
	t->start(p);

	return this;
}

ModuleHandler* ModuleHandler::onThreadFinished() {
	pthread_mutex_lock(&_mutex);
	int size = getThreads()->size();
	pthread_mutex_unlock(&_mutex);

	if (!size && _moduleServer) {
		_moduleServer->onAllThreadsDone(this);
	}

	return this;
}

std::set<ModuleThread*>* ModuleHandler::getThreads() {
	if (!_threads) {
		_threads = new std::set<ModuleThread*>();
	}
	return _threads;
}

void* ModuleHandler::getLibraryPointer () {
	if (!_libraryPtr) {
		dlerror();
		_libraryPtr = dlopen(_libraryPath.c_str(), RTLD_LAZY);
		*this << DEBUG << "Loading module : " << _libraryPath << endl;
		if (!_libraryPtr) {
			throw ModuleException(dlerror(), -1);
		}
	}
	return _libraryPtr;
}

create_t* ModuleHandler::getModuleConstructor () {
	if (!_moduleConstructor) {
		dlerror();
		_moduleConstructor = (create_t*) dlsym(getLibraryPointer(), "create");
		if (!_moduleConstructor) {
			throw ModuleException(dlerror(), -1);
		}
	}
	return _moduleConstructor;
}

destroy_t* ModuleHandler::getModuleDestructor () {
	if (!_moduleDestructor) {
		dlerror();
		_moduleDestructor = (destroy_t*) dlsym(getLibraryPointer(), "destroy");
		if (!_moduleDestructor) {
			throw ModuleException(dlerror(), -1);
		}
	}
	return _moduleDestructor;
}

IModule* ModuleHandler::getModuleInstance () {
	if (!_moduleInstance) {
		_moduleInstance = getModuleConstructor()();
		_moduleInstance->setFolder(getModuleFolder());
	}
	return _moduleInstance;
}

std::string ModuleHandler::getModuleFolder () {
	std::string folder = Config::getInstance()->getParamValue("modulePath");

	int start = _libraryPath.find_last_of('/') + 4;

	std::string f = folder +
									_libraryPath.substr(start,
																			_libraryPath.find_last_of('.') - start) + "/";

	DIR* dir = opendir(f.c_str());
	if (dir == NULL) {
		DIR* moduleDir = opendir(folder.c_str());
		if (moduleDir == NULL) {
			if (mkdir(folder.c_str(), S_IRWXG | S_IRWXO | S_IRWXU) == -1)
				throw ModuleException("Unable to create module folder",-1);
		}
		else
			closedir(moduleDir);

		if (mkdir(f.c_str(), S_IRWXG | S_IRWXO | S_IRWXU) == -1)
			throw ModuleException("Unable to create module folder",-1);
	}
	else
		closedir(dir);

	return f;
}
