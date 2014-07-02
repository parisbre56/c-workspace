#include "writeLock.hpp"
#include "externData.hpp"

void writeTimedLock() {
	struct timespec timeout;
	timeout.tv_nsec=0;
	timeout.tv_sec=__WRITELOCK__TIMEOUTSECS__;
	pthread_mutex_timedlock(&wMtx,&timeout);
}

void writeTimedUnlock() {
	pthread_mutex_unlock(&wMtx);
}