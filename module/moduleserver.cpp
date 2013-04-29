#include "moduleserver.h"

std::string* ModuleServer::_path = NULL;

ModuleServer::ModuleServer() : _modules(0), _threads(0)
{
	// Ouverture du dossier
	DIR* directory = opendir(getPath()->c_str());
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
				path << getPath()->c_str() << filename;
				addModuleLoader(new ModuleLoader(path.str()));
			}
		}
		closedir(directory);
	}
}

ModuleServer::~ModuleServer() {
	if (_modules) delete _modules;
	if (_threads) {
		for (uint i = 0; i < _threads->size(); ++i) {
			_threads->at(i)->wait();
		}
	}
}

void ModuleServer::setPath(std::string* path) {
	_path = path;
	kill();
}

std::string* ModuleServer::getPath() {
	if (!_path) {
		_path = new std::string("../lib/");
	}
	return _path;
}

ModuleServer* ModuleServer::addModuleLoader (ModuleLoader* module) {
	getModules()->push_back(module);
	return this;
}

std::vector<ModuleLoader*>* ModuleServer::getModules () {
	if (!_modules) {
		_modules = new std::vector<ModuleLoader*>();
	}
	return _modules;
}

std::vector<ModuleThread*>* ModuleServer::getModuleThreads() {
	if (!_threads) {
		_threads = new std::vector<ModuleThread*> ();
	}
	return _threads;
}

ModuleServer* ModuleServer::onPacketReceived (Packet* p) {
	for (uint i = 0; i < getModules()->size(); ++i) {
		try {
			IModule* module = getModules()->at(i)->getModuleInstance();
			ModuleThread* t = new ModuleThread(module);
			t->setPacket(p)->start();
			getModuleThreads()->push_back(t);
		} catch (ModuleException &e) {
			io::err << e.what() << io::endl;
		}
	}
	return this;
}
