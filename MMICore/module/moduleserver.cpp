#include "moduleserver.h"

#include <dirent.h>

#include "tools/logger.h"
#include "network/packet/packet.h"
#include "module/moduleexception.h"
#include "module/modulehandler.h"

ModuleServer::ModuleServer()
	: _modules(0) { load(); }

ModuleServer::~ModuleServer() {
	if (_modules) {
		for ( std::set<ModuleHandler*>::iterator it = _modules->begin();
					it != _modules->end(); ++it) {
			delete (*it);
		}
		delete _modules;
	}
}

const std::string ModuleServer::getName() const { return "ModuleServer"; }

ModuleServer* ModuleServer::load(std::string fromPath) {
	*this << DEBUG << "Loading modules" << endl;
	// Ouverture du dossier
	DIR* directory = opendir(fromPath.c_str());
	if (directory != NULL) {
		struct dirent* entry;
		// Lecture de chaque fichier
		while ((entry = readdir(directory)) != NULL) {
			std::string filename(entry->d_name);
			int index = filename.find_last_of(".");
			// Si c'est une librairie
			if(index != -1 && filename.substr(index) == ".so") {
				// On la charge
				std::stringstream path;
				path << fromPath.c_str() << filename;
				addModuleHandler(new ModuleHandler(path.str(), this));
			}
		}
		closedir(directory);
	}

	return this;
}

ModuleServer* ModuleServer::reload(std::string fromPath) {
	getModuleHandlers()->clear();
	return load(fromPath);
}

ModuleServer* ModuleServer::addModuleHandler (ModuleHandler* module) {
	getModuleHandlers()->insert(module);
	return this;
}

std::set<ModuleHandler*>* ModuleServer::getModuleHandlers () {
	if (!_modules) {
		_modules = new std::set<ModuleHandler*>();
	}
	return _modules;
}

ModuleServer* ModuleServer::onNewPacket (Packet* p) {
	// Make sure that there's some data to read.
	p->getData();

	if (p->getType() == (SERVER_RELOADING_REQUEST)) {
		reload();
		Packet result(*p);
		result.setType(RELOADING_RESULT)->setData()->doSend();
		return this;
	}

	for (std::set<ModuleHandler*>::iterator i = getModuleHandlers()->begin();
			 i != getModuleHandlers()->end(); ++i) {
		try {
			(*i)->onNewPacket(new Packet(*p));
		} catch (std::exception& e) {
			*this << ERROR << e.what() << std::endl;
		}
	}
	return this;
}

ModuleServer* ModuleServer::onAllThreadsDone (ModuleHandler* module) {
	// Si le module n'est plus dans la liste des modules
	// Alors les modules ont été rechargé
	if (getModuleHandlers()->find(module) == getModuleHandlers()->end()) {
		// On peut donc se débarasser de delui-la
		delete module;
	}
	return this;
}
