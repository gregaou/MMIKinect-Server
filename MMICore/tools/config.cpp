#include "tools/config.h"

#include <string.h>
#include <iostream>
#include <fstream>
#include <stdexcept>

Config::Config()
{}

Config* Config::load(int argc, char** argv) {
	_app = std::string(argv[0], strlen(argv[0]));

	for (int i = 1; i < argc; ++i) {
		std::string arg(argv[i], strlen(argv[i]));
		std::string name;
		std::string value;
		if (arg.find("--") == 0) {
			int eq = arg.find('=');
			if (eq == -1) {
				name = arg.substr(2);
				value = "";
			} else {
				name = arg.substr(2, eq - 2);
				value = arg.substr(eq + 1);
			}

			insertValue(name, value);
		} else {
			*this << WARNING << "Parameter " << arg
											 << " is malformed. Ignored." << std::endl;
		}
	}

	return this;
}

Config* Config::load(std::string filename)
{
	std::ifstream config(filename.c_str());

	if (!config.is_open()) {
		*this << WARNING << "File " << filename << " not found." << std::endl;
	}

	while (config.good()) {
		std::string name;
		std::string value;
		getline(config, name, '=');
		getline(config, value);

		if (!name.empty())
			insertValue(name, value);
	}
	config.close();

	return this;
}

Config* Config::insertValue(std::string key, std::string value)
{
	try {
		Param p = _params.at(key);

		if (p._isArgRequired && value.empty()) {
			*this << WARNING << "Value required for argument "
											 << key << ", but not provided. Ignored." << std::endl;
		} else if (!p._isArgRequired && !value.empty()){
			*this << WARNING << "Value not required for argument "
											 << key << ", but provided. Ignored." << std::endl;
		} else if (!paramExists(key)) {
			_values.insert(std::pair<std::string,std::string>(key,value));
		}
	} catch (std::out_of_range&) {
		*this << WARNING << "Unknown parameter " << key
										 << ". Ignored." << std::endl;
	}

	return this;
}

bool Config::isGood()
{
	std::map<std::string, Param>::iterator it;
	for (it = _params.begin(); it != _params.end(); it++) {
		Param p = it->second;
		if (p._isMandatory && !paramExists(p._name)) {
			return false;
		}
	}

	return true;
}

Config* Config::addParam(Param p)
{
	_params.insert( std::pair<std::string,Param>(p._name,p));

	return this;
}

bool Config::paramExists(std::string key)
{
	return _values.find(key) != _values.end();
}

std::string Config::getParamValue(std::string key)
{
	return _values.at(key);
}

const string Config::getName () const {
	return "Config";
}

Config* Config::help()
{
	std::cout << "Usage : " << _app << " [<param>=<value> [...]]" << std::endl;
	std::cout << std::endl;
	std::cout << "Parameters :" << std::endl;

	std::map<std::string, Param>::iterator it;
	for (it = _params.begin(); it != _params.end(); it++) {
		Param p = it->second;
		std::cout << " --" << p._name << "\t: "
							<< ((p._isMandatory) ? "[MANDATORY] " : "[OPTIONNAL] ")
							<< ((!p._isArgRequired) ? "[NO ARGUMENT] " : "")
							<< p._description << std::endl;
	}

	std::cout << std::endl;

	return this;
}
