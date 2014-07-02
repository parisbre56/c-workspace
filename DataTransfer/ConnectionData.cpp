#include "ConnectionData.hpp"
#include <unistd.h>

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
	pthread_mutex_unlock(&mtx);
	if(links<=0) {
		delete this;
	}
}

ConnectionData::~ConnectionData()
{
	int temp=1;
	write(sock,&temp,sizeof(int));
	close(sock);
	pthread_mutex_destroy(&mtx);
}

