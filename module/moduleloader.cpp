#include "moduleloader.h"

ModuleLoader::ModuleLoader (std::string libraryFilePath) :
	_libraryFilePath(libraryFilePath),
	_libraryPtr(0),
	_moduleConstructor(0),
	_moduleDestructor(0),
	_moduleInstance(0)
{}

ModuleLoader::~ModuleLoader() {
	dlclose(_libraryPtr);
}

void* ModuleLoader::getLibraryPointer () {
	if (!_libraryPtr) {
		dlerror();
		_libraryPtr = dlopen(_libraryFilePath.c_str(), RTLD_LAZY);
		if (!_libraryPtr) {
			throw ModuleException(dlerror(), -1);
		}
	}
	return _libraryPtr;
}

create_t* ModuleLoader::getModuleConstructor () {
	if (!_moduleConstructor) {
		dlerror();
		_moduleConstructor = (create_t*) dlsym(getLibraryPointer(), "create");
		if (!_moduleConstructor) {
			throw ModuleException(dlerror(), -1);
		}
	}
	return _moduleConstructor;
}

destroy_t* ModuleLoader::getModuleDestructor () {
	if (!_moduleDestructor) {
		dlerror();
		_moduleDestructor = (destroy_t*) dlsym(getLibraryPointer(), "destroy");
		if (!_moduleDestructor) {
			throw ModuleException(dlerror(), -1);
		}
	}
	return _moduleDestructor;
}

IModule* ModuleLoader::getModuleInstance () {
	if (!_moduleInstance) {
		_moduleInstance = getModuleConstructor()();
	}
	return _moduleInstance;
}

ModuleLoader* ModuleLoader::destroyInstance (IModule* module) {
	(getModuleDestructor())(module);
	return this;
}
