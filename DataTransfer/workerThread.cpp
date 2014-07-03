#include "workerThread.hpp"

#include <iostream>

#include <cstring>

#include <sys/sendfile.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "externData.hpp"
#include "writeLock.hpp"

/* Communication protocol is as follows:
 * 
 * 	Server listens, client initiates connection
 * 	Server creates new reader thread. Reader locks socket mutex.
 * 	Client sends length of string (size_t) or 0 for error
 * 	Client sends string (char array with the above length)
 * 		Client sends only the special character '\0' (null) if it wants the server to terminate
 * 	Client waits for response (int)
 * 	Server reads the data and checks if it's OK.
 * 		If data not OK, Server sends negative or 0 response indicating an error (int), an error string and closes the connection.
 * 		The negative response indicates the size of the string containing the error description.
 * 	If data OK, the server sends positive response which is PAGESIZE (int).
 * 	Client now waits for status indicator (int). 
 * 		If Client receives 1, then it knows there are no more data and terminates succesfully.
 * 		If Client receives 0, then it knows there are more data coming and gets ready to process a new file
 * 		If client receives negative, then it knows there is an error and it waits for an error string with a size of -status.
 * 	Server gathers data for the pool. If there is a problem, sends negative to Client.
 * 	Server puts data in pool. Reader unlocks socket mutex.
 * 	Reader thread exits now.
 * 	Writer thread takes data from the pool and prepares to send it. Locks socket mutex. If all OK, sends 0 to indicate data incoming. Else sends -1 to indicate error.
 * 	Server sends file string length (size_t) or 0 for error
 * 	Server sends file string (char array with the above length)
 * 	Client starts reading data segments for this file and writing them to socket until it receives 0 or negative.
 * 	While there are more data in this file to transmit
 * 		Server sends positive dataSize (int) or negative for error.
 * 		Server sends data (void* data of the above dataSize)
 * 	When there are no more data in this file to transmit
 * 		Server sends 0 (int). Writer unlocks mutex.
 * 	Writer reads from pool again while Client waits for response again
 */

using namespace std;

/* Processes the data, opening the file and sending it to the client
 * Returns -1 if there is some sort of fatal error, else returns 0.
 */
int processData(DataObject* currentData);

void * workerThread(void* not_used) {
	DataObject* currentData;
	//Return value for obtaining data
	int retVal;
	//Keep looping until the exit signal is received
	while(!exitCond) {
		//Get the data
		if((retVal=dataPool->obtain(currentData))<0) {
			threadList.remove(pthread_self());
			return NULL;
		}
		//If this timed out, keep trying to get more data
		if(retVal==__POOL__TIMEOUT__) {
			continue;
		}
		//If all is well, process data
		if(processData(currentData)<0) {
			//If there's a problem, exit.
			delete currentData;
			threadList.remove(pthread_self());
			return NULL;
		}
	}
	//When the exit signal has been received
	//Keep looping until the pool is empty and no readers are active
	while(true) {
		//Get the data
		if((retVal=dataPool->obtain(currentData))<0) {
			threadList.remove(pthread_self());
			return NULL;
		}
		//If this timed out, keep trying to get more data
		if(retVal==__POOL__TIMEOUT__) {
			continue;
		}
		//If the pool is empty
		if(retVal==__POOL__EMPTY__) {
			//If there are more reader threads, continue waiting for data
			if(threadList.size()>thread_pool_size) {
				continue;
			}
			//If there are no more reader threads, exit
			else {
				break;
			}
		}
		//If all is well, process data
		if(processData(currentData)<0) {
			//If there's a problem, exit.
			delete currentData;
			threadList.remove(pthread_self());
			return NULL;
		}
		delete currentData;
	}
	return NULL;
}

int processData(DataObject* currentData) {
	struct stat file_stat;
	int file_fd;
	int socket_int;
	size_t socket_size;
	//Writer thread takes data from the pool and prepares to send it. 
	//First stat the file
	if(stat((currentData->req).c_str(),&file_stat)<0) {
		writeTimedLock();
		cerr<<"ERROR:"<<to_string(pthread_self())<<": Unable to get file data for "<<currentData->req<<endl;
		cerr<<"ERROR:"<<to_string(pthread_self())<<":"<<strerror(errno)<<endl;
		writeTimedUnlock();
		return 0;
	}
	//Open the file
	if((file_fd=open((currentData->req).c_str(),O_RDONLY))<0) {
		writeTimedLock();
		cerr<<"ERROR:"<<to_string(pthread_self())<<": Unable to open file "<<currentData->req<<endl;
		cerr<<"ERROR:"<<to_string(pthread_self())<<":"<<strerror(errno)<<endl;
		writeTimedUnlock();
		return 0;
	}
	//Obtain the socket mutex 
	if(currentData->retLoc->lock_mutex()<0) {
		writeTimedLock();
		cerr<<"ERROR:"<<to_string(pthread_self())<<": Unable to obtain mutex while processing file "<<currentData->req<<endl;
		cerr<<"ERROR:"<<to_string(pthread_self())<<":"<<strerror(errno)<<endl;
		writeTimedUnlock();
		close(file_fd);
		return 0;
	}
	//If all OK, sends 0 to indicate data incoming. Else sends -1 to indicate error.
	socket_int=0;
	if(write(currentData->retLoc->sock,&socket_int,sizeof(int))!=sizeof(int)) {
		writeTimedLock();
		cerr<<"ERROR:"<<to_string(pthread_self())<<": Unable to write ready signal to socket while processing file "<<currentData->req<<endl;
		cerr<<"ERROR:"<<to_string(pthread_self())<<":"<<strerror(errno)<<endl;
		writeTimedUnlock();
		close(file_fd);
		currentData->retLoc->unlock_mutex();
		return 0;
	}
	//Server sends file string length (size_t) or 0 for error
	socket_size=(currentData->req).size();
	if(write(currentData->retLoc->sock,&socket_size,sizeof(size_t))!=sizeof(size_t)) {
		writeTimedLock();
		cerr<<"ERROR:"<<to_string(pthread_self())<<": Unable to write file string length to socket while processing file "<<currentData->req<<endl;
		cerr<<"ERROR:"<<to_string(pthread_self())<<":"<<strerror(errno)<<endl;
		writeTimedUnlock();
		close(file_fd);
		currentData->retLoc->unlock_mutex();
		return 0;
	}
	//Server sends file string (char array with the above length)
	if(write(currentData->retLoc->sock,(currentData->req).c_str(),(currentData->req).size())!=(currentData->req).size()) {
		writeTimedLock();
		cerr<<"ERROR:"<<to_string(pthread_self())<<": Unable to write file string to socket while processing file "<<currentData->req<<endl;
		cerr<<"ERROR:"<<to_string(pthread_self())<<":"<<strerror(errno)<<endl;
		writeTimedUnlock();
		close(file_fd);
		currentData->retLoc->unlock_mutex();
		return 0;
	}
	//Number of bytes left to transfer
	off_t size_left = file_stat.st_size;
	off_t current_ofset = 0;
	//Client starts reading data segments for this file and writing them to socket until it receives 0 or negative.
	//While there are more data in this file to transmit
	//		Server sends positive dataSize (int) or negative for error.
	//		Server sends data (void* data of the above dataSize)
	while(size_left>0) {
		//Get the number of bytes we need to transmit
		if(size_left>=getpagesize()) {
			socket_int=getpagesize();
		}
		else {
			socket_int=size_left;
		}
		size_left-=socket_int;
		//Server sends positive data size
		if(write(currentData->retLoc->sock,&socket_int,sizeof(int))!=sizeof(int)) {
			writeTimedLock();
			cerr<<"ERROR:"<<to_string(pthread_self())<<": Unable to write datapacket size to socket while processing file "<<currentData->req<<endl;
			cerr<<"ERROR:"<<to_string(pthread_self())<<":"<<strerror(errno)<<endl;
			writeTimedUnlock();
			close(file_fd);
			currentData->retLoc->unlock_mutex();
			return 0;
		}
		//Server sends data
		if(sendfile(currentData->retLoc->sock,file_fd,&current_ofset,socket_int)!=socket_int) {
			writeTimedLock();
			cerr<<"ERROR:"<<to_string(pthread_self())<<": Unable to write datapacket to socket while processing file "<<currentData->req<<endl;
			cerr<<"ERROR:"<<to_string(pthread_self())<<":"<<strerror(errno)<<endl;
			writeTimedUnlock();
			close(file_fd);
			currentData->retLoc->unlock_mutex();
			return 0;
		}
	}
	//When there are no more data in this file to transmit
	//		Server sends 0 (int). Writer unlocks mutex.
	socket_int=0;
	if(write(currentData->retLoc->sock,&socket_int,sizeof(int))!=sizeof(int)) {
		writeTimedLock();
		cerr<<"ERROR:"<<to_string(pthread_self())<<": Unable to write end of file signal to socket while processing file "<<currentData->req<<endl;
		cerr<<"ERROR:"<<to_string(pthread_self())<<":"<<strerror(errno)<<endl;
		writeTimedUnlock();
		close(file_fd);
		currentData->retLoc->unlock_mutex();
		return 0;
	}
	//Close and exit
	if(close(file_fd)<0) {
		writeTimedLock();
		cerr<<"ERROR:"<<to_string(pthread_self())<<": Unable to close the file descriptor while processing file "<<currentData->req<<endl;
		cerr<<"ERROR:"<<to_string(pthread_self())<<":"<<strerror(errno)<<endl;
		writeTimedUnlock();
		currentData->retLoc->unlock_mutex();
		return 0;
	}
	if(currentData->retLoc->unlock_mutex()<0) {
		writeTimedLock();
		cerr<<"ERROR:"<<to_string(pthread_self())<<": Unable to unlock socket mutex while processing file "<<currentData->req<<endl;
		cerr<<"ERROR:"<<to_string(pthread_self())<<":"<<strerror(errno)<<endl;
		writeTimedUnlock();
		return 0;
	}
	return 0;
}