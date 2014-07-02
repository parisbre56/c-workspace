#include "ThreadList.hpp"

#include "writeLock.hpp"

#include <string>
#include <iostream>

#include <cstdlib>
#include <cstring>

using namespace std;

ThreadListObject::ThreadListObject(pthread_t* cThread) {
	this->next=NULL;
	this->thread=cThread;
}

ThreadList::ThreadList()
{
	pthread_mutex_init(&mtx, 0);
	this->root=NULL;
}

void ThreadList::push(pthread_t* data) {
	//Lock the mutex
	if(pthread_mutex_lock(&mtx)<0) {
		writeTimedLock();
		cerr<<"ERROR:"<<to_string(pthread_self())<<":push:mutex_lock:"<<strerror(errno)<<endl;
		writeTimedUnlock();
		return;
	}
	if(root==NULL) {
		root=new ThreadListObject(data);
		//Release the mutex
		if(pthread_mutex_unlock(&mtx)<0) {
			writeTimedLock();
			cerr<<"ERROR:"<<to_string(pthread_self())<<":push:mutex_unlock:"<<strerror(errno)<<endl;
			writeTimedUnlock();
			return;
		}
		return;
	}
	ThreadListObject* temp=root;
	while(temp->next!=NULL) {
		temp=temp->next;
	}
	temp->next=new ThreadListObject(data);
	//Release the mutex
	if(pthread_mutex_unlock(&mtx)<0) {
		writeTimedLock();
		cerr<<"ERROR:"<<to_string(pthread_self())<<":push:mutex_unlock:"<<strerror(errno)<<endl;
		writeTimedUnlock();
		return;
	}
}

pthread_t* ThreadList::pop() {
	//Lock the mutex
	if(pthread_mutex_lock(&mtx)<0) {
		writeTimedLock();
		cerr<<"ERROR:"<<to_string(pthread_self())<<":pop:mutex_lock:"<<strerror(errno)<<endl;
		writeTimedUnlock();
		return NULL;
	}
	if(root==NULL) {
		//Release the mutex
		if(pthread_mutex_unlock(&mtx)<0) {
			writeTimedLock();
			cerr<<"ERROR:"<<to_string(pthread_self())<<":pop:mutex_unlock:"<<strerror(errno)<<endl;
			writeTimedUnlock();
			return NULL;
		}
		return NULL;
	}
	pthread_t* retThread=root->thread;
	ThreadListObject* temp=root;
	root=root->next;
	delete temp;
	//Release the mutex
	if(pthread_mutex_unlock(&mtx)<0) {
		writeTimedLock();
		cerr<<"ERROR:"<<to_string(pthread_self())<<":pop:mutex_unlock:"<<strerror(errno)<<endl;
		writeTimedUnlock();
		return NULL;
	}
	return retThread;
}

void ThreadList::remove(pthread_t data) {
	//Lock the mutex
	if(pthread_mutex_lock(&mtx)<0) {
		writeTimedLock();
		cerr<<"ERROR:"<<to_string(pthread_self())<<":remove:mutex_lock:"<<strerror(errno)<<endl;
		writeTimedUnlock();
		return;
	}
	if(root==NULL) {
		//Release the mutex
		if(pthread_mutex_unlock(&mtx)<0) {
			writeTimedLock();
			cerr<<"ERROR:"<<to_string(pthread_self())<<":remove:mutex_unlock:"<<strerror(errno)<<endl;
			writeTimedUnlock();
			return;
		}
		return;
	}
	ThreadListObject * temp=root;
	if(*(root->thread)==data) {
		root=root->next;
		delete temp->thread;
		delete temp;
		//Release the mutex
		if(pthread_mutex_unlock(&mtx)<0) {
			writeTimedLock();
			cerr<<"ERROR:"<<to_string(pthread_self())<<":remove:mutex_unlock:"<<strerror(errno)<<endl;
			writeTimedUnlock();
			return;
		}
		return;
	}
	while(temp->next!=NULL) {
		if(*(temp->next->thread)==data) {
			ThreadListObject * toDel = temp->next;
			temp->next=temp->next->next;
			delete toDel->thread;
			delete toDel;
			break;
		}
	}
	//Release the mutex
	if(pthread_mutex_unlock(&mtx)<0) {
		writeTimedLock();
		cerr<<"ERROR:"<<to_string(pthread_self())<<":remove:mutex_unlock:"<<strerror(errno)<<endl;
		writeTimedUnlock();
		return;
	}
}

ThreadList::~ThreadList()
{
	//Delete all thread objects to clear memory
	pthread_t * thread=NULL;
	while((thread=this->pop())!=NULL) {
		delete thread;
	}
	pthread_mutex_destroy(&mtx);
}

