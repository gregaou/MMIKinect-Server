#include <fstream>

#include "tools/config.h"
#include "server.h"

int main(int argc, char** argv)
{
	Config* c = Config::getInstance();

	c   ->addParam(Param("config", false, true, "A configuration file."))
			->addParam(Param("libPath", true, true, "The libraries loading path."))
			->addParam(Param("modulePath", true, true, "The modules working path."))
			->addParam(Param("DEBUG.prefix", false, true, "The debug prefix."))
			->addParam(Param("DEBUG.stream", false, true, "The debug stream."))
			->addParam(Param("DEBUG.file", false, true, "The debug log file."))
			->addParam(Param("INFO.prefix", false, true, "The debug prefix."))
			->addParam(Param("INFO.stream", false, true, "The debug stream."))
			->addParam(Param("INFO.file", false, true, "The debug log file."))
			->addParam(Param("WARN.prefix", false, true, "The debug prefix."))
			->addParam(Param("WARN.stream", false, true, "The debug stream."))
			->addParam(Param("WARN.file", false, true, "The debug log file."))
			->addParam(Param("ERROR.prefix", false, true, "The debug prefix."))
			->addParam(Param("ERROR.stream", false, true, "The debug stream."))
			->addParam(Param("ERROR.file", false, true, "The debug log file."))
			->addParam(Param("helpMe",false,false,"Print a parameter description."));

	c->load(argc, argv);

	if (c->paramExists("config"))
		c->load(c->getParamValue("config"));

	if (!c->isGood() || c->paramExists("help")) {
		c->help();
		return 0;
	}

	Logger::configure();

	Server s;
	s.doWork();

	return 0;
}

