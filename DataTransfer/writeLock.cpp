#include "writeLock.hpp"
#include "externData.hpp"

#include <time.h>

void writeTimedLock() {
	struct timespec timeout;
	clock_gettime(CLOCK_REALTIME, &timeout);
	timeout.tv_sec+=__WRITELOCK__TIMEOUTSECS__;
	pthread_mutex_timedlock(&wMtx,&timeout);
}

void writeTimedUnlock() {
	pthread_mutex_unlock(&wMtx);
}
