#ifndef QTHREAD_P_H
#define QTHREAD_P_H

#include <pthread.h>

#include "thread.h"
#include "typedef.h"

class ThreadPrivate
{
public:
	ThreadPrivate();
	~ThreadPrivate();

	pthread_cond_t cond;
	pthread_mutex_t mutex;

	bool running;
	bool finished;
	bool terminated;
	uint32 stackSize;

	pthread_t thread_id;
	static void *start(void *arg);
	static void finish(void *arg);
};

#endif // QTHREAD_P_H
