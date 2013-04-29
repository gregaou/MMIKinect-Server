#ifndef QTHREAD_H
#define QTHREAD_H

#include <iostream>

#include <signal.h>
#include <unistd.h>
#include <stdio.h>

#include "logger.h"
#include "threadprivate.h"

class ThreadPrivate;

class Thread
{
public:
	explicit Thread();
	virtual ~Thread();

	bool isFinished() const;
	bool isRunning() const;

	void start();
	void terminate();
	void wait();
	void setStackSize(unsigned int stackSize);
	unsigned int stackSize() const;
	static int idealThreadCount();

protected:
	// events
	virtual void started() {}
	virtual void finished() {}
	virtual void terminated() {}

	// main loop
	virtual void run() {}

private:
	ThreadPrivate *d;
	friend class ThreadPrivate;
};

#endif // QTHREAD_H
