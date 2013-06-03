#include "thread.h"

Thread::Thread()
{
	d = new ThreadPrivate();
}

Thread::~Thread()
{
	pthread_mutex_lock(&d->mutex);
	if (d->running && !d->finished)
		*this << WARNING << "Destroyed while thread is still running" << std::endl;
	pthread_mutex_unlock(&d->mutex);
	delete d;
}

const std::string Thread::getName() const { return "Thread"; }

bool Thread::isFinished() const
{
	pthread_mutex_lock(&d->mutex);
	bool ret = d->finished;
	pthread_mutex_unlock(&d->mutex);
	return ret;
}

bool Thread::isRunning() const
{
	pthread_mutex_lock(&d->mutex);
	bool ret = d->running;
	pthread_mutex_unlock(&d->mutex);
	return ret;
}

void Thread::setStackSize(unsigned int stackSize)
{
	pthread_mutex_lock(&d->mutex);
	if (d->running)
	{
		*this << WARNING << "Cannot change stack size while thread is running!" << std::endl;
		pthread_mutex_unlock(&d->mutex);
		return;
	}
	d->stackSize = stackSize;
	pthread_mutex_unlock(&d->mutex);
}

unsigned int Thread::stackSize() const
{
	pthread_mutex_lock(&d->mutex);
	bool ret = d->stackSize;
	pthread_mutex_unlock(&d->mutex);
	return ret;
}

void Thread::start()
{
	pthread_mutex_lock(&d->mutex);
	if (d->running) {
		pthread_mutex_unlock(&d->mutex);
		return;
	}

	sigset_t sigset, oldset;
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGINT);
	pthread_sigmask(SIG_BLOCK, &sigset, &oldset);

	d->running = true;
	d->finished = false;

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_attr_setinheritsched(&attr, PTHREAD_INHERIT_SCHED);
	int code = pthread_create(&d->thread_id, &attr, ThreadPrivate::start, this);
	pthread_attr_destroy(&attr);

	if (code)
	{
		*this << ERROR << "Thread creation error: " << code << std::endl;

		d->running = false;
		d->finished = false;
		d->thread_id = 0;
	}
	else
	{
		// Restore the old signal mask only for this thread.
		pthread_sigmask(SIG_SETMASK, &oldset, NULL);
	}
	pthread_mutex_unlock(&d->mutex);
}

void Thread::terminate()
{
	pthread_mutex_lock(&d->mutex);

	if (!d->thread_id) {
		pthread_mutex_unlock(&d->mutex);
		return;
	}

	int code = pthread_cancel(d->thread_id);
	if (code)
	{
		*this << ERROR << "Thread termination error: " << code << std::endl;
	}
	else
	{
		d->terminated = true;
	}
	pthread_mutex_unlock(&d->mutex);
}

void Thread::wait()
{
	pthread_mutex_lock(&d->mutex);
	if (d->finished || !d->running) {
		pthread_mutex_unlock(&d->mutex);
		return;
	}

	while (d->running) {
		pthread_cond_wait(&d->cond, &d->mutex);
	}
	pthread_join(d->thread_id, NULL);
	pthread_mutex_unlock(&d->mutex);
}

int Thread::idealThreadCount()
{
	int cores = -1;
	cores = (int)sysconf(_SC_NPROCESSORS_ONLN);
	return cores;
}
