#include <iostream>
#include <string>

#include <cerrno>
#include <cstdlib>
#include <cstring>

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <signal.h>

#include "Pool.hpp"
#include "connectionReaderThread.hpp"
#include "ConnectionData.hpp"
#include "ThreadList.hpp"
#include "workerThread.hpp"
#include "writeLock.hpp"

using namespace std;

/* Prints help about the command line arguments for this program in stderr
 */
void printUsage();

/* Check the arguements and puts them in the appropriate variable.
 * Returns -1 for error, 0 for successs.
 */
int getArgs(int argc, char **argv, int& port, int& thread_pool_size, int& queue_size);

/* Deletes dataPool. Called atexit.
 */
void deletePool();

/* Returns a new TCP socket listening on port <port> or negative for failure.
 */
int makeSocket(int port);

/* Accepts a connection from socket and then creates a new thread to handle that connection.
 * Puts the thread in the list of active threads.
 * Returns 0 for success and negative for failure.
 */
int processConnection(int socket);

/* Sets exitCond to true and waits for all threads to finish what they're doing and exit.
 * Called atexit.
 */
void joinThreads();

/* Creates a worker thread and puts it in the list of active threads.
 */
int createWorker();

/* Deletes the writer mutex atexit.
 */
void deleteWMutex();

/* Maximum number of connections waiting to be accepted.
 */
#define BACKLOG 5

//Keeps all the data that must be retrieved. Accesed atomically.
Pool* dataPool;
//True if the server is exiting
bool exitCond;
//List of all active threads except for main. Accessed atomically.
ThreadList threadList;
//Used for concurrent writing
pthread_mutex_t wMtx;

int main(int argc, char **argv)
{
	dataPool=NULL;
	int port=-1;
	int thread_pool_size=-1;
	int queue_size=-1;
	int sock;
	
	pthread_mutex_init(&wMtx, 0);
	atexit(deleteWMutex);
	
	//Get and check arguments
	if(getArgs(argc,argv,port,thread_pool_size,queue_size)<0) {
		cerr<<"ERROR: Unable to get arguments. Exiting..."<<endl;
		exit(EXIT_FAILURE);
	}
	//Create the required data objects. Make them clear atexit.
	dataPool=new Pool(queue_size,10);
	atexit(deletePool);
	//Create the thread pool
	atexit(joinThreads);
	for(int i=0;i<thread_pool_size;++i) {
		if(createWorker()<0) {
			cerr<<"ERROR: Unable to create worker. Exiting..."<<endl;
			exit(EXIT_FAILURE);
		}
	}
	//Create the socket
	sock=makeSocket(port);
	//Set options
	int option_value=1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&option_value,sizeof(option_value));
	setsockopt(sock,SOL_SOCKET,SO_REUSEPORT,&option_value,sizeof(option_value));
	//Start the server and wait for a connection
	while(!exitCond) {
		if(processConnection(sock)<0) {
			close(sock);
			cerr<<"ERROR: Unable to accept connection. Exiting..."<<endl;
			exit(EXIT_FAILURE);
		}
	}
	clog<<"DEBUG: Exit condition set. Exiting..."<<endl;
	close(sock);
	exit(EXIT_SUCCESS);
}

void printUsage() {
	cerr	<<"./dataServer -p <port> -s <thread_pool_size> -q <queue_size>\n"
			<<"\tport\tThe server's listen port.\n"
			<<"\tthread_pool_size\tThe number of worker threads in the thread pool.\n"
			<<"\tqueue_size\tThe size of the queue holding the requests.\n"
			<<endl;
}

int getArgs(int argc, char **argv, int& port, int& thread_pool_size, int& queue_size) {
	if(argc!=7) {
		cerr<<"ERROR: Need exactly 6 arguments."<<endl;
		printUsage();
		return -1;
	}
	port=-1;
	thread_pool_size=-1;
	queue_size=-1;
	
	char* pPort=NULL;
	char* pThread=NULL;
	char* pQueue=NULL;
	
	//Get flags and args
	for(int i=1;i<7;i+=2) {
		if(string("-p")==argv[i]) {
			if(pPort!=NULL) {
				cerr<<"ERROR: Duplicate -p flag."<<endl;
				return -1;
			}
			pPort=argv[i+1];
		}
		else if(string("-s")==argv[i]) {
			if(pThread!=NULL) {
				cerr<<"ERROR: Duplicate -s flag."<<endl;
				return -1;
			}
			pThread=argv[i+1];
		}
		else if(string("-q")==argv[i]) {
			if(pQueue!=NULL) {
				cerr<<"ERROR: Duplicate -q flag."<<endl;
				return -1;
			}
			pQueue=argv[i+1];
		}
		else {
			cerr<<"ERROR: Unknown flag: "<<argv[i]<<endl;
			printUsage();
			return -1;
		}
	}
	
	//Convert
	port=stoi(string(pPort));
	thread_pool_size=stoi(string(pThread));
	queue_size=stoi(string(pQueue));
	
	//Check
	if(port<=0||port>65535) {
		cerr<<"ERROR: Port number must be greater than zero and less than or equal to 65535."<<endl;
		return -1;
	}
	
	if(thread_pool_size<=0) {
		cerr<<"ERROR: Thread pool size must be greater than zero."<<endl;
		return -1;
	}
	if(queue_size<=0) {
		cerr<<"ERROR: Queue size must be greater than zero."<<endl;
		return -1;
	}
	
	return 0;
}

void deletePool() {
	if(dataPool!=NULL) {
		delete dataPool;
		dataPool=NULL;
	}
}

int makeSocket(int port) {
	int sock;
	struct sockaddr_in server;
	//Create socket
	sock=socket(PF_INET,SOCK_STREAM,0);
	if(socket<0) {
		cerr<<"ERROR:makeSocket:socket:"<<strerror(errno)<<endl;
		return -1;
	}
	//Bind socket to port
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=htonl(INADDR_ANY) ;
	server.sin_port=htons(port);
	if(bind(sock,(struct sockaddr *)(&server),sizeof(server)) < 0) {
		cerr<<"ERROR:makeSocket:bind:"<<strerror(errno)<<endl;
		return -1;
	}
	//Mark socket as passive listener
	if(listen(sock,BACKLOG)<0) {
		cerr<<"ERROR:makeSocket:listen:"<<strerror(errno)<<endl;
		return -1;
	}
	
	return sock;
}

void joinThreads() {
	pthread_t* thread=NULL;
	exitCond=true;
	while((thread=threadList.pop())!=NULL) {
		pthread_join(*thread,NULL);
		delete thread;
	}
}

int processConnection(int sock) {
	int newsock=-1;
	struct sockaddr_in client;
	socklen_t clientlen;
	//Get the new connection data
	if ((newsock=accept(sock,(struct sockaddr *)(&client),&clientlen))<0) {
		writeTimedLock();
		cerr<<"ERROR:processConnection:accept:"<<strerror(errno)<<endl;
		writeTimedUnlock();
		return -1;
	}
	//Set options
	int option_value=1;
	setsockopt(newsock,SOL_SOCKET,SO_REUSEADDR,&option_value,sizeof(option_value));
	setsockopt(newsock,SOL_SOCKET,SO_REUSEPORT,&option_value,sizeof(option_value));
	//Package it in an object
	ConnectionData * connData = new ConnectionData(newsock,client,clientlen);
	pthread_t* thread = new pthread_t;
	//And pass it to a new thread
	if(pthread_create(thread,NULL,connectionReaderThread,connData)<0)
	{
		writeTimedLock();
		cerr<<"ERROR:processConnection:pthread_create:"<<strerror(errno)<<endl;
		writeTimedUnlock();
		connData->linksDecrement();
		delete thread;
		return -1;
	}
	//Put the new thread in the thread list
	threadList.push(thread);
	//And exit succesfuly
	//(Does not decrement links in connData because it is simply handed over to the thread)
	return 0;
}

int createWorker() {
	//Start the new thread
	pthread_t* thread = new pthread_t;
	if(pthread_create(thread,NULL,workerThread,NULL)<0) {
		cerr<<"ERROR:createWorker:pthread_create:"<<strerror(errno)<<endl;
		delete thread;
		return -1;
	}
	//Put the new thread in the thread list
	threadList.push(thread);
	//And exit succesfuly
	return 0;
}

void deleteWMutex() {
	pthread_mutex_destroy(&wMtx);
}