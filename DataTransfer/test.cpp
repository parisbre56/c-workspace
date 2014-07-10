#include <iostream>

#include <cstring>
#include <cstdlib>

#include <sys/sendfile.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

using namespace std;

int main(int argc,char** argv) {
	pthread_cond_t cond_nonempty;
	pthread_cond_t cond_nonfull;
	pthread_mutex_t mtx;
	pthread_condattr_t cond_attr;
	pthread_mutexattr_t mtx_attr;
	int retval=pthread_condattr_init(&cond_attr);	
	retval=pthread_condattr_setclock(&cond_attr, CLOCK_MONOTONIC);	
	retval=pthread_mutexattr_init(&mtx_attr);
	retval=pthread_mutex_init(&mtx, &mtx_attr);
	retval=pthread_mutexattr_destroy(&mtx_attr);
	retval=pthread_cond_init(&cond_nonempty, &cond_attr);
	retval=pthread_cond_init(&cond_nonfull, &cond_attr);
	retval=pthread_condattr_destroy(&cond_attr);
	
	//Lock mutex
	if(pthread_mutex_lock(&mtx)<0) {
		cerr<<"ERROR:"<<to_string(pthread_self())<<":place:mutex_lock:"<<strerror(errno)<<endl;
		return -1;
	}
	clog<<"Mutex locked"<<endl;
	//Make absolute timeout
	struct timespec curr_time;
	struct timespec timeout_abs;
	clock_gettime(CLOCK_MONOTONIC,&curr_time);
	timeout_abs.tv_sec=curr_time.tv_sec+5;
	timeout_abs.tv_nsec=curr_time.tv_nsec;
	//Test signal
	errno=0;
	while(true) {
		clock_gettime(CLOCK_MONOTONIC,&curr_time);
		if(pthread_cond_timedwait(&cond_nonempty,&mtx,&timeout_abs)<0&&errno!=ETIMEDOUT) {
			cerr<<"ERROR:"<<to_string(pthread_self())<<":obtain:cond_wait:"<<strerror(errno)<<endl;
			pthread_mutex_unlock(&mtx);
			return -1;
		}
		if(errno==ETIMEDOUT) {
			clog<<"DEBUG:"<<to_string(pthread_self())<<":Timeout:"<<strerror(errno)<<endl;
			return 1;
		}
	}
	
	exit(EXIT_SUCCESS);
}