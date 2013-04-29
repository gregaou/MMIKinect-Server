#include "threadprivate.h"

ThreadPrivate::ThreadPrivate() :
	running(false), finished(false), terminated(false), stackSize(0)
{
	thread_id = 0;
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);
}

ThreadPrivate::~ThreadPrivate()
{
	pthread_cond_destroy(&cond);
	pthread_mutex_destroy(&mutex);
}

void *ThreadPrivate::start(void *arg)
{
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
	pthread_cleanup_push(ThreadPrivate::finish, arg);

	Thread *thr = reinterpret_cast<Thread *>(arg);

	thr->started();
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_testcancel();
	thr->run();

	pthread_cleanup_pop(1);
	return 0;
}

void ThreadPrivate::finish(void *arg)
{
	Thread *thr = reinterpret_cast<Thread *>(arg);
	ThreadPrivate *d = thr->d;
	pthread_mutex_lock(&d->mutex);

	d->running = false;
	d->finished = true;
	if (d->terminated)
		thr->terminated();
	d->terminated = false;
	thr->finished();

	d->thread_id = 0;
	pthread_cond_broadcast(&d->cond);
	pthread_mutex_unlock(&d->mutex);
}
