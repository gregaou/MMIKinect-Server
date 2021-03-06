#ifndef QTHREAD_H
#define QTHREAD_H

#include <iostream>

#include <signal.h>
#include <unistd.h>
#include <stdio.h>

#include "tools/logger.h"
#include "tools/threadprivate.h"

class ThreadPrivate;

class Thread : public Loggable
{
public:
	explicit Thread();
	virtual ~Thread();

	const std::string getName() const;

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
