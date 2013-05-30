#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>
#include <map>

#include "tools/logger.h"
#include "tools/singleton.h"

class Param;

class Config : public Singleton<Config>, public Loggable
{
	friend class Singleton<Config>;

public:
	Config* load(int argc, char** argv);
	Config* load(std::string filename);

	bool isGood();

	Config* addParam(Param p);

	Config* insertValue(std::string key, std::string value);
	bool paramExists(std::string key);
	std::string getParamValue(std::string key);

	Config* help();

	virtual const string getName () const;

private:
	Config();

	std::string _app;
	std::map<std::string,Param> _params;
	std::map<std::string, std::string> _values;
};

class Param
{
public:
	Param(const std::string name, const bool isMandatory,
				const bool isArgRequired, const std::string description)
		: _name(name), _isMandatory(isMandatory),
			_isArgRequired(isArgRequired), _description(description) {}

	std::string	_name;
	bool				_isMandatory;
	bool				_isArgRequired;
	std::string	_description;
};

#endif // CONFIG_H
