#ifndef MODULELOADER_H
#define MODULELOADER_H

#include <iostream>
#include <string>
#include <exception>

#include <cstdlib>
#include <dlfcn.h>

#include "imodule.h"
#include "moduleexception.h"

class ModuleLoader
{
public:
	ModuleLoader(std::string libraryFilePath);

	~ModuleLoader ();


	IModule* getModuleInstance ();
	ModuleLoader* destroyInstance (IModule* module);

private:
	void* getLibraryPointer ();
	create_t* getModuleConstructor ();
	destroy_t* getModuleDestructor ();

	std::string _libraryFilePath;
	std::string _moduleType;

	void* _libraryPtr;
	create_t* _moduleConstructor;
	destroy_t* _moduleDestructor;
	IModule* _moduleInstance;
};

#endif // MODULELOADER_H
