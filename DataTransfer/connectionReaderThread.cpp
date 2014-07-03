#include "connectionReaderThread.hpp"

#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>

#include <iostream>
#include <cstring>

#include "DataObjectList.hpp"
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

/* Writes a simple error message for write errors.
 */
void writeError(string pos);

/* Writes a simple error message for read errors.
 */
void readError(string pos);

/* Writes a simple error message for errors received from the other process.
 */
void receivedError(int errNum, string pos);

/* Writes a simple error message for errors involving files.
 */
void fileError(string fileName, string pos);

/* Recursively fills the <tempDataStack> with file transfer requests 
 * to be sent to the socket pointed to by <connectionData>. Starts from file <file_name>. 
 * If <file_name> is a file, then it simply creates a DataObject for that file and returns.
 * If <file_name> is a folder, then it calls itself for all its contents.
 * If there is an error, it returns negative, else it returns 0.
 * If there is an error, it automatically sends an error response to the client socket and writes an error
 * report to stderr.
 */
int fillWithDataObjects(ConnectionData * connectionData,DataObjectList& tempDataStack,string file_name);

void * connectionReaderThread(void* connectionData_temp) {
	DataObjectList tempDataStack;
	
	size_t socket_size;
	char* socket_data;
	string socket_data_string;
	int socket_int;
	struct stat file_stat;
	
	
	ConnectionData* connectionData=static_cast<ConnectionData*>(connectionData_temp);
	//Ignore failed write signals. We'll process them in the write level
	struct sigaction action;
	sigemptyset(&(action.sa_mask));
	action.sa_handler=SIG_IGN;
	sigaction(SIGPIPE, &action, NULL);
	//Client sends length of string (size_t) or 0 for error
	if(read(connectionData->sock,&socket_size,sizeof(size_t))!=sizeof(size_t)) {
		readError("read_initial_string_length");
		threadList.remove(pthread_self());
		connectionData->linksDecrement();
		return NULL;
	}
	if(socket_size==0) {
		receivedError(0,"read_initial_string_length");
		threadList.remove(pthread_self());
		connectionData->linksDecrement();
		return NULL;
	}
	//Client sends string (char array with the above length)
	socket_data=new char[socket_size];
	if(read(connectionData->sock,&socket_data,socket_size)!=socket_size) {
		readError("read_initial_string");
		delete[] socket_data;
		threadList.remove(pthread_self());
		connectionData->linksDecrement();
		return NULL;
	}
	//Client sends only the special character '\0' (null) if it wants the server to terminate
	if(socket_data[0]=='\0') {
		writeTimedLock();
		clog<<"DEBUG:"<<to_string(pthread_self())<<": Received exit command from client."<<endl;
		writeTimedUnlock();
		exitCond=true;
		delete[] socket_data;
		threadList.remove(pthread_self());
		connectionData->linksDecrement();
		return NULL;
	}
	socket_data_string.assign(socket_data,socket_size);
	delete[] socket_data;
	socket_data=NULL;
	//Client waits for response (int)
  	//Server reads the data and checks if it's OK.
	if(stat(socket_data_string.c_str(),&file_stat)<0) {
		//If data not OK, Server sends negative or 0 response indicating an error (int), an error string and closes the connection.
		//The negative response indicates the size of the string containing the error description.
		fileError(socket_data_string,"get_initial_file_info");
		socket_data_string="get_initial_file_info:"+to_string(errno)+":"+strerror(errno);
		socket_int=-socket_data_string.size();
		write(connectionData->sock,&socket_int,sizeof(int));
		write(connectionData->sock,socket_data_string.c_str(),socket_data_string.size());
		threadList.remove(pthread_self());
		connectionData->linksDecrement();
		return NULL;
	}
	//Recursively fill tempDataStack with the data we need to send back
	if(fillWithDataObjects(connectionData,tempDataStack,socket_data_string)<0) {
		//The function sends the error to the client and writes it in stderr automatically, 
		//so there's no need to write it here
		threadList.remove(pthread_self());
		connectionData->linksDecrement();
		return NULL;
	}
	//If data OK, the server sends positive response which is PAGESIZE (int).
	socket_int=getpagesize();
	if(write(connectionData->sock,&socket_int,sizeof(int))!=sizeof(int)) {
		writeError("return_page_size");
		threadList.remove(pthread_self());
		connectionData->linksDecrement();
		return NULL;
	}
	//Reader passes all dataObjects to the pool so that they can be processed by the worker threads.
	DataObject* tempObject;
	while((tempObject=tempDataStack.pop())!=NULL) {
		dataPool->place(tempObject);
	}
	//Reader removes self from threadList
	threadList.remove(pthread_self());
	//and decrements the links to connection data so that it will be deleted eventually
	connectionData->linksDecrement();
	return NULL;
}

void writeError(string pos) {
	writeTimedLock();
	cerr<<"ERROR:"<<to_string(pthread_self())<<":"<<pos<<":Reader thread encountered error while writing to socket"<<endl;
	cerr<<"ERROR:"<<to_string(pthread_self())<<":"<<pos<<":"<<strerror(errno)<<endl;
	writeTimedUnlock();
}

void readError(string pos) {
	writeTimedLock();
	cerr<<"ERROR:"<<to_string(pthread_self())<<":"<<pos<<":Reader thread encountered error while reading from socket"<<endl;
	cerr<<"ERROR:"<<to_string(pthread_self())<<":"<<pos<<":"<<strerror(errno)<<endl;
	writeTimedUnlock();
}

void receivedError(int errNum, string pos) {
	writeTimedLock();
	cerr<<"ERROR:"<<to_string(pthread_self())<<":"<<pos<<":Reader thread received error"<<errNum<<"from connected process"<<endl;
	cerr<<"ERROR:"<<to_string(pthread_self())<<":"<<pos<<":Aborting communication"<<endl;
	writeTimedUnlock();
}

void fileError(string fileName, string pos) {
	writeTimedLock();
	cerr<<"ERROR:"<<to_string(pthread_self())<<":"<<pos<<":Encountered error while processing file request "<<fileName<<endl;
	cerr<<"ERROR:"<<to_string(pthread_self())<<":"<<pos<<":"<<strerror(errno)<<endl;
	writeTimedUnlock();
}

int fillWithDataObjects(ConnectionData * connectionData,DataObjectList& tempDataStack,string file_name) {
	string socket_data_string;
	int socket_int;
	
	struct stat file_stat;
	
	//Get file stats
	if(stat(file_name.c_str(),&file_stat)<0) {
		fileError(file_name,"fillWithDataObjects:stat");
		socket_data_string="fillWithDataObjects:stat:"+to_string(errno)+":"+strerror(errno);
		socket_int=-socket_data_string.size();
		write(connectionData->sock,&socket_int,sizeof(int));
		write(connectionData->sock,socket_data_string.c_str(),socket_data_string.size());
		return -1;
	}
	
	//If this is a regular file
	if(S_ISREG(file_stat.st_mode)) {
		//Push a request for that file in the stack and exit
		tempDataStack.push(new DataObject(connectionData,file_name));
		return 0;
	}
	//Else if thiis is a directory
	else if(S_ISDIR(file_stat.st_mode)) {
		//Call self for all the contents of the directory 
		DIR* currDir=opendir(file_name.c_str()); //Remember to close dir on error
		if(currDir==NULL) {
			fileError(file_name,"fillWithDataObjects:opendir");
			socket_data_string="fillWithDataObjects:stat:"+to_string(errno)+":"+strerror(errno);
			socket_int=-socket_data_string.size();
			write(connectionData->sock,&socket_int,sizeof(int));
			write(connectionData->sock,socket_data_string.c_str(),socket_data_string.size());
			return -1;
		}
		struct dirent prevDirEntry;
		struct dirent * dirEntry=NULL;
		//Keep looping until all dir entries are processed
		while(true) {
			//Read the next folder entry
			if(readdir_r(currDir,&prevDirEntry,&dirEntry)!=0) {
				fileError(file_name,"fillWithDataObjects:readdir_r");
				socket_data_string="fillWithDataObjects:readdir_r:"+to_string(errno)+":"+strerror(errno);
				socket_int=-socket_data_string.size();
				write(connectionData->sock,&socket_int,sizeof(int));
				write(connectionData->sock,socket_data_string.c_str(),socket_data_string.size());
				closedir(currDir);
				return -1;
			}
			//Stop looping if we have processed all folder entries
			if(dirEntry==NULL) {
				break;
			}
			//Move on to next folder entry if this is pointer to the previous folder or the current folder
			if(string(".")==(dirEntry->d_name)||string("..")==(dirEntry->d_name)) {
				continue;
			}
			//Check this folder entry recursively
			string file_name_new=file_name+"/"+dirEntry->d_name;
			if(fillWithDataObjects(connectionData,tempDataStack,file_name_new)<0) {
				closedir(currDir);
				return -1;
			}
		}
		closedir(currDir);
		return 0;
	}
	//Else if this is not a regular file and not a directory
	//(if this is some kind of pipe, for example)
	else {
		//Just write a warning.
		writeTimedLock();
		clog	<<"DEBUG:"<<to_string(pthread_self())<<":Ignoring file "<<file_name<<" with mode (octal) "
				<<oct<<file_stat.st_mode<<endl;
		writeTimedUnlock();
		return 0;
	}
	//Return succesfully
	return 0;
}