TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = \
	tools \
	network \
	module \
	app \
	dummymodule \
	AudioModule \
	FacialModule

network.depends = tools
module.depends = tools
app.depends = module

dummymodule.depends = module
AudioModule.depends = module
FacialModule.depends= module
