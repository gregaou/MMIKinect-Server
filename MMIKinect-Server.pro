TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = \
	tools \
	network \
	module \
	app \
	dummymodule

network.depends = tools
module.depends = network tools
app.depends = module network tools

dummymodule.depends = module network tools
