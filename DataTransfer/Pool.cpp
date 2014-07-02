#include "Pool.hpp"

#include "writeLock.hpp"

#include <iostream>

#include <cerrno>
#include <cstdio>
#include <cstring>

using namespace std;

Pool::Pool(int size,unsigned short timeout_secs):poolSize(size) {
	start=0;
	end=-1;
	count=0;
	timeout.tv_nsec=0;
	timeout.tv_sec=timeout_secs;
	pthread_mutex_init(&mtx, 0);
	pthread_cond_init(&cond_nonempty, 0);
	pthread_cond_init(&cond_nonfull, 0);
	data = new DataObject*[size];
}

int Pool::place(DataObject*& toPlace) {
	//Get the lock
	if(pthread_mutex_lock(&mtx)<0) {
		writeTimedLock();
		cerr<<"ERROR:"<<to_string(pthread_self())<<":place:mutex_lock:"<<strerror(errno)<<endl;
		writeTimedUnlock();
		return -1;
	}
	//Release the lock and wait for the pool to become nonfull
	while(count>=poolSize) {
		if(pthread_cond_wait(&cond_nonfull,&mtx)<0) {
			writeTimedLock();
			cerr<<"ERROR:"<<to_string(pthread_self())<<":place:cond_wait:"<<strerror(errno)<<endl;
			writeTimedUnlock();
			pthread_mutex_unlock(&mtx);
			return -1;
		}
	}
	//Place the data
	end = (end+1)%poolSize;
	data[end]=toPlace;
	++count;
	//Unlock the mutex
	if(pthread_mutex_unlock(&mtx)<0) {
		writeTimedLock();
		cerr<<"ERROR:"<<to_string(pthread_self())<<":place:mutex_unlock:"<<strerror(errno)<<endl;
		writeTimedUnlock();
		return -1;
	}
	//Notify others that this is no logner empty
	if(pthread_cond_signal(&cond_nonempty)<0) {
		writeTimedLock();
		cerr<<"ERROR:"<<to_string(pthread_self())<<":place:cond_signal:"<<strerror(errno)<<endl;
		writeTimedUnlock();
		return -1;
	}
	return 0;
}

int Pool::obtainIfNotEmpty(DataObject*& store) {
	//Get the mutex
	if(pthread_mutex_lock(&mtx)<0) {
		writeTimedLock();
		cerr<<"ERROR:"<<to_string(pthread_self())<<":obtain:mutex_lock:"<<strerror(errno)<<endl;
		writeTimedUnlock();
		return -1;
	}
	//If it is empty, release the mutex and return __POOL__EMPTY__
	if(this->count<=0) {
		if(pthread_mutex_unlock(&mtx)<0) {
			writeTimedLock();
			cerr<<"ERROR:"<<to_string(pthread_self())<<":obtain:mutex_unlock:"<<strerror(errno)<<endl;
			writeTimedUnlock();
			return -1;
		}
		return __POOL__EMPTY__;
	}
	//Release the lock and wait for the pool to become nonempty
	while(count<=0) {
		if(pthread_cond_timedwait(&cond_nonempty,&mtx,&timeout)<0) {
			if(errno==ETIMEDOUT) {
				return __POOL__TIMEOUT__;
			}
			writeTimedLock();
			cerr<<"ERROR:"<<to_string(pthread_self())<<":obtain:cond_wait:"<<strerror(errno)<<endl;
			writeTimedUnlock();
			pthread_mutex_unlock(&mtx);
			return -1;
		}
	}
	//Retrieve the data
	store=data[start];
	start = (start+1)%poolSize;
	--count;
	//Release the mutex
	if(pthread_mutex_unlock(&mtx)<0) {
		writeTimedLock();
		cerr<<"ERROR:"<<to_string(pthread_self())<<":obtain:mutex_unlock:"<<strerror(errno)<<endl;
		writeTimedUnlock();
		return -1;
	}
	//Notify others that this is no logner full
	if(pthread_cond_signal(&cond_nonfull)<0) {
		writeTimedLock();
		cerr<<"ERROR:"<<to_string(pthread_self())<<":obtain:cond_signal:"<<strerror(errno)<<endl;
		writeTimedUnlock();
		return -1;
	}
	return 0;
}

int Pool::obtain(DataObject*& store) {
	//Get the mutex
	if(pthread_mutex_lock(&mtx)<0) {
		writeTimedLock();
		cerr<<"ERROR:"<<to_string(pthread_self())<<":obtain:mutex_lock:"<<strerror(errno)<<endl;
		writeTimedUnlock();
		return -1;
	}
	//Release the lock and wait for the pool to become nonempty
	while(count<=0) {
		if(pthread_cond_timedwait(&cond_nonempty,&mtx,&timeout)<0) {
			if(errno==ETIMEDOUT) {
				return __POOL__TIMEOUT__;
			}
			writeTimedLock();
			cerr<<"ERROR:"<<to_string(pthread_self())<<":obtain:cond_wait:"<<strerror(errno)<<endl;
			writeTimedUnlock();
			pthread_mutex_unlock(&mtx);
			return -1;
		}
	}
	//Retrieve the data
	store=data[start];
	start = (start+1)%poolSize;
	--count;
	//Release the mutex
	if(pthread_mutex_unlock(&mtx)<0) {
		writeTimedLock();
		cerr<<"ERROR:"<<to_string(pthread_self())<<":obtain:mutex_unlock:"<<strerror(errno)<<endl;
		writeTimedUnlock();
		return -1;
	}
	//Notify others that this is no logner full
	if(pthread_cond_signal(&cond_nonfull)<0) {
		writeTimedLock();
		cerr<<"ERROR:"<<to_string(pthread_self())<<":obtain:cond_signal:"<<strerror(errno)<<endl;
		writeTimedUnlock();
		return -1;
	}
	return 0;
}

Pool::~Pool() {
	pthread_cond_destroy(&cond_nonempty) ;
	pthread_cond_destroy(&cond_nonfull);
	pthread_mutex_destroy(&mtx);
	delete[] data;
}

