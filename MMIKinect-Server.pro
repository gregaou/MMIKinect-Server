TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = \
	tools \
	network \
	module \
	app \
	dummymodule \
    FacialModule

network.depends = tools
module.depends = network tools
app.depends = module network tools
testApp.depends = module network tools

dummymodule.depends = module network tools
