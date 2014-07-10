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
 * Returns 1 for timeout.
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

/* How long it will wait for a connection before it times out
 */
#define CONNECTION_TIMEOUT 10

//Keeps all the data that must be retrieved. Accesed atomically.
Pool* dataPool;
//True if the server is exiting
bool exitCond;
//List of all active threads except for main. Accessed atomically.
ThreadList threadList;
ThreadList readerThreadList;
//Used for concurrent writing
pthread_mutex_t wMtx;
//Number of worker threads
int thread_pool_size;

int main(int argc, char **argv)
{
	exitCond=false;
	dataPool=NULL;
	int port=-1;
	thread_pool_size=-1;
	int queue_size=-1;
	int sock;
	
	pthread_mutexattr_t mtx_attr;
	pthread_mutexattr_init(&mtx_attr);
	pthread_mutex_init(&wMtx, &mtx_attr);
	pthread_mutexattr_destroy(&mtx_attr);
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
			writeTimedLock();
			cerr<<"ERROR: Unable to create worker. Exiting..."<<endl;
			writeTimedUnlock();
			exit(EXIT_FAILURE);
		}
	}
	//Create the socket
	sock=makeSocket(port);
	if(sock<0) {
		writeTimedLock();
		cerr<<"ERROR: Unable to create socket."<<endl;
		writeTimedUnlock();
		exit(EXIT_FAILURE);
	}
	//Set options to allow socket to be reused
	int option_value=1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&option_value,sizeof(option_value));
	//Set timeout for accepting connections
	option_value=CONNECTION_TIMEOUT;
	setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,&option_value,sizeof(option_value));
	setsockopt(sock,SOL_SOCKET,SO_SNDTIMEO,&option_value,sizeof(option_value));
	//Start the server and wait for a connection
	writeTimedLock();
	clog<<"DEBUG: Server started."<<endl;
	writeTimedUnlock();
	while(!exitCond) {
		if(processConnection(sock)<0) {
			close(sock);
			writeTimedLock();
			cerr<<"ERROR: Unable to accept connection. Exiting..."<<endl;
			writeTimedUnlock();
			exit(EXIT_FAILURE);
		}
	}
	writeTimedLock();
	clog<<"DEBUG: Exit condition set. Exiting..."<<endl;
	writeTimedUnlock();
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
	sock=socket(PF_INET,SOCK_STREAM|SOCK_NONBLOCK,0);
	if(socket<0) {
		writeTimedLock();
		cerr<<"ERROR:makeSocket:socket:"<<strerror(errno)<<endl;
		writeTimedUnlock();
		return -1;
	}
	//Bind socket to port
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=htonl(INADDR_ANY) ;
	server.sin_port=htons(port);
	if(bind(sock,(struct sockaddr *)(&server),sizeof(server)) < 0) {
		writeTimedLock();
		cerr<<"ERROR:makeSocket:bind:"<<strerror(errno)<<endl;
		writeTimedUnlock();
		return -1;
	}
	//Mark socket as passive listener
	if(listen(sock,BACKLOG)<0) {
		writeTimedLock();
		cerr<<"ERROR:makeSocket:listen:"<<strerror(errno)<<endl;
		writeTimedUnlock();
		return -1;
	}
	
	return sock;
}

void joinThreads() {
	pthread_t* thread=NULL;
	exitCond=true;
	while((thread=readerThreadList.pop())!=NULL) {
		pthread_join(*thread,NULL);
		delete thread;
	}
	while((thread=threadList.pop())!=NULL) {
		pthread_join(*thread,NULL);
		delete thread;
	}
}

int processConnection(int sock) {
	int newsock=-1;
	struct sockaddr_in client;
	socklen_t clientlen=sizeof(struct sockaddr_in);
	//Get the new connection data
	if((newsock=accept(sock,(struct sockaddr *)(&client),&clientlen))<0) {
		//If this is a timeout, return 1
		//(Remember, timeout is set by the define)
		if(errno==EAGAIN||errno==EWOULDBLOCK) {
			return 1;
		}
		writeTimedLock();
		cerr<<"ERROR:processConnection:accept:"<<strerror(errno)<<endl;
		writeTimedUnlock();
		return -1;
	}
	//Allow socket to be reused
	int option_value=1;
	setsockopt(newsock,SOL_SOCKET,SO_REUSEADDR,&option_value,sizeof(option_value));
	//Make it block
	option_value=0;
	setsockopt(newsock,SOL_SOCKET,SO_SNDTIMEO,&option_value,sizeof(option_value));
	setsockopt(newsock,SOL_SOCKET,SO_SNDTIMEO,&option_value,sizeof(option_value));
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
		writeTimedLock();
		cerr<<"ERROR:createWorker:pthread_create:"<<strerror(errno)<<endl;
		writeTimedUnlock();
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
