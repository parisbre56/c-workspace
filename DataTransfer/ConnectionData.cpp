#include "ConnectionData.hpp"
#include <unistd.h>

#include <iostream>

#include "writeLock.hpp"

using namespace std;

ConnectionData::ConnectionData(int Csock, struct sockaddr_in Cclient, socklen_t Cclientlen)
{
	sock=Csock;
	client=Cclient;
	clientlen=Cclientlen;
	links=1;
	pthread_mutex_init(&mtx, 0);
}

int ConnectionData::lock_mutex() {
	return pthread_mutex_lock(&mtx);
}

int ConnectionData::timedLock_mutex(unsigned int timeout_secs) {
	struct timespec timeout;
	timeout.tv_nsec=0;
	timeout.tv_sec=timeout_secs;
	return pthread_mutex_timedlock(&mtx,&timeout);
}

int ConnectionData::unlock_mutex() {
	return pthread_mutex_unlock(&mtx);
}

void ConnectionData::linksIncrement() {
	pthread_mutex_lock(&mtx);
	++links;
	pthread_mutex_unlock(&mtx);
}

void ConnectionData::linksDecrement() {
	pthread_mutex_lock(&mtx);
	--links;
	writeTimedLock();
	clog<<"DEBUG: Links:"<<links<<endl;
	writeTimedUnlock();
	if(links<=0) {
		delete this;
	}
	else {
		pthread_mutex_unlock(&mtx);
	}
}

ConnectionData::~ConnectionData()
{
	writeTimedLock();
	clog<<"DEBUG: No links left to connection data object. Commiting suicide..."<<endl;
	writeTimedUnlock();
	int temp=1;
	write(sock,&temp,sizeof(int));
	close(sock);
	pthread_mutex_unlock(&mtx);
	pthread_mutex_destroy(&mtx);
}

