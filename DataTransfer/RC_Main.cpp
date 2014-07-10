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

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#include <fcntl.h>


#include <iostream>
#include <string>

#include <cstdlib>
#include <cstring>

using namespace std;

/* Prints help about the command line arguments for this program in stderr
 */
void printUsage();

/* Check the arguements and puts them in the appropriate variable.
 * Returns -1 for error, 0 for successs.
 * Exit is set to true if a request should be made to the server to exit.
 */
int getArgs(int argc, char **argv, string& ip, int& port, string& directory, bool& exitCond);

/* Closes the socket, called atexit.
 */
void closeSock();

void deleteTempArray();

int createDirIfNecessaryAndOpen(string filePathString); //check for wrror!!!!!

int sock;
char* tempData;

int main(int argc, char **argv)
{
	string ip;
	int port=-1;
	string directory;
	int retval=-1;
	struct addrinfo* addressInfo=NULL;
	bool exitCond=false;
	
	//Get and check arguments
	if(getArgs(argc,argv,ip,port,directory,exitCond)<0) {
		cerr<<"ERROR: Unable to get arguments. Exiting..."<<endl;
		exit(EXIT_FAILURE);
	}
	
	//Ignore failed write signals. We'll process them synchronously
	struct sigaction action;
	sigemptyset(&(action.sa_mask));
	action.sa_handler=SIG_IGN;
	action.sa_flags=0;
	sigaction(SIGPIPE, &action, NULL);
	
	//Get address info
	struct addrinfo hint;
	hint.ai_family=AF_UNSPEC;//Any family (ipv4 or ipv6)
	hint.ai_socktype=SOCK_STREAM;//TCP only
	hint.ai_protocol=0;//Any protocol
	hint.ai_flags=(AI_V4MAPPED | AI_ADDRCONFIG);//Any address
	
	retval=getaddrinfo(ip.c_str(),to_string(port).c_str(),&hint,&addressInfo);
	if(retval<0||addressInfo==NULL) {
		cerr<<"ERROR: Unable to get address info for "<<ip<<":"<<port<<endl;
		cerr<<"ERROR: "<<gai_strerror(retval)<<endl;
		exit(EXIT_FAILURE);
	} 
	
	//Create socket
	sock=socket(addressInfo->ai_family,addressInfo->ai_socktype,addressInfo->ai_protocol);
	if(sock<0) {
		cerr	<<"ERROR: Unable to create socket type :"<<addressInfo->ai_family
				<<":"<<addressInfo->ai_socktype<<":"<<addressInfo->ai_protocol<<endl;
		cerr	<<"ERROR: "<<strerror(errno)<<endl;
		freeaddrinfo(addressInfo);
		exit(EXIT_FAILURE);
	}
	
	atexit(closeSock);
	
	//Connect to server
	retval=connect(sock,addressInfo->ai_addr,addressInfo->ai_addrlen);
	freeaddrinfo(addressInfo);
	if(retval<0) {
		cerr<<"ERROR: Unable to connect to server."<<endl;
		cerr<<"ERROR: "<<strerror(errno)<<endl;
		exit(EXIT_FAILURE);
	}
	
	size_t socket_size;
	int socket_int;
	
	//Client sends length of string (size_t) or 0 for error
	if(exitCond) {
		socket_size=sizeof(char);
	}
	else {
		socket_size=directory.size();
	}
	if(write(sock,&socket_size,sizeof(size_t))!=sizeof(size_t)) {
		cerr<<"ERROR: Unable to write message size to server."<<endl;
		cerr<<"ERROR: "<<strerror(errno)<<endl;
		exit(EXIT_FAILURE);
	}
	//Client sends string (char array with the above length)
	//Client sends only the special character '\0' (null) if it wants the server to terminate
	if(exitCond) {
		//Process exit signal and exit.
		char tempChar='\0';
		if(write(sock,&tempChar,socket_size)!=socket_size) {
			cerr<<"ERROR: Unable to write exit signal."<<endl;
			cerr<<"ERROR: "<<strerror(errno)<<endl;
			exit(EXIT_FAILURE);
		}
		clog<<"DEBUG: Exit command sent succesfully."<<endl;
		exit(EXIT_SUCCESS);
	}
	
	//Else if this is not an exit signal
	//Try to send the directory string
	if(write(sock,directory.c_str(),socket_size)!=socket_size) {
		cerr<<"ERROR: Unable to write directory string to server"<<endl;
		cerr<<"ERROR: "<<strerror(errno)<<endl;
		exit(EXIT_FAILURE);
	}
	
	//Client waits for response (int)
	if(read(sock,&socket_int,sizeof(int))!=sizeof(int)) {
		cerr<<"ERROR: Unable to read server's response to directory string."<<endl;
		cerr<<"ERROR: "<<strerror(errno)<<endl;
		exit(EXIT_FAILURE);
	}
	//Server reads the data and checks if it's OK.
	//		If data not OK, Server sends negative or 0 response indicating an error (int), an error string and closes the connection.
	//		The negative response indicates the size of the string containing the error description.
	if(socket_int<=0) {
		cerr	<<"ERROR: Received error response "<<socket_int
				<<" from server for directory string \""<<directory<<"\""<<endl;
		if(socket_int<0) {
			char* tempCharArray = new char[-socket_int];
			if(read(sock,tempCharArray,-socket_int)!=-socket_int) {
				cerr<<"ERROR: Unable to read error string from server."<<endl;
				cerr<<"ERROR: "<<strerror(errno)<<endl;
			}
			else {
				string tempString;
				tempString.assign(tempCharArray,-socket_int);
				cerr<<"ERROR: Error response string from server follows:"<<endl;
				cerr<<"SERVER: "<<tempString<<endl;
				cerr<<"ERROR: End of string from server."<<endl;
			}
			delete[] tempCharArray;
		}
		exit(EXIT_FAILURE);
	}
	//Else, if returned data was positive
	//If data OK, the server sends positive response which is PAGESIZE (int).
	int pageSize=socket_int;
	tempData = new char[pageSize];
	atexit(deleteTempArray);
	//Keep looping until the exit signal is received from the server
	while(true) {
		//Client now waits for status indicator (int).
		if(read(sock,&socket_int,sizeof(int))!=sizeof(int)) {
			cerr<<"ERROR: Unable to read status indicator from server."<<endl;
			cerr<<"ERROR: "<<strerror(errno)<<endl;
			exit(EXIT_FAILURE);
		}
		//		If Client receives 1, then it knows there are no more data and terminates succesfully.
		if(socket_int==1) {
			clog<<"DEBUG: Received end of transmission signal from server. Exiting..."<<endl;
			break;
		}
		//		If client receives negative, then it knows there is an error and it waits for an error string with a size of -status.
		else if(socket_int<0) {
			char* tempCharArray = new char[-socket_int];
			if(read(sock,tempCharArray,-socket_int)!=-socket_int) {
				cerr<<"ERROR: Unable to read error string from server."<<endl;
				cerr<<"ERROR: "<<strerror(errno)<<endl;
			}
			else {
				string tempString;
				tempString.assign(tempCharArray,-socket_int);
				cerr<<"ERROR: Error response string from server follows:"<<endl;
				cerr<<"SERVER: "<<tempString<<endl;
				cerr<<"ERROR: End of string from server."<<endl;
			}
			delete[] tempCharArray;
			exit(EXIT_FAILURE);
		}
		//		If Client receives 0, then it knows there are more data coming and gets ready to process a new file
		else if(socket_int!=0) {
			cerr<<"ERROR: Received unknown status signal: "<<socket_int<<endl;
			exit(EXIT_FAILURE);
		}
		//Server gathers data for the pool. If there is a problem, sends negative to Client.
		//Server puts data in pool. Reader unlocks socket mutex.
		//Reader thread exits now.
		//Writer thread takes data from the pool and prepares to send it. Locks socket mutex. If all OK, sends 0 to indicate data incoming. Else sends -1 to indicate error.
		//Server sends file string length (size_t) or 0 for error
		if(read(sock,&socket_size,sizeof(size_t))!=sizeof(size_t)) {
			cerr<<"ERROR: Unable to read incoming file path string size"<<endl;
			cerr<<"ERROR: "<<strerror(errno)<<endl;
			exit(EXIT_FAILURE);
		}
		//Server sends file string (char array with the above length)
		char* tempCharArray = new char[socket_size];
		if(read(sock,tempCharArray,socket_size)!=socket_size) {
			cerr<<"ERROR: Unable to read incoming file path string"<<endl;
			cerr<<"ERROR: "<<strerror(errno)<<endl;
			delete[] tempCharArray;
			exit(EXIT_FAILURE);
		}
		string filePathString;
		filePathString.assign(tempCharArray,socket_size);
		delete[] tempCharArray;
		clog<<"DEBUG: Received file path: "<<filePathString<<endl;
		//Try to open the file, create dir if necessary
		int file_fd=createDirIfNecessaryAndOpen(filePathString);
		if(file_fd<0) {
			cerr<<"ERROR: Unable to create/open file."<<endl;
			exit(EXIT_FAILURE);
		}
		//Client starts reading data segments for this file and writing them to socket until it receives 0 or negative.
		while(true) {
			if(read(sock,&socket_int,sizeof(int))!=sizeof(int)) {
				cerr<<"ERROR: Unable to read incoming data segment size"<<endl;
				cerr<<"ERROR: "<<strerror(errno)<<endl;
				close(file_fd);
				exit(EXIT_FAILURE);
			}
			//When there are no more data in this file to transmit
			//		Server sends 0 (int). Writer unlocks mutex.
			if(socket_int==0) {
				clog<<"DEBUG: File "<<filePathString<<" written successfully."<<endl;
				break;
			}
			//While there are more data in this file to transmit
			//		Server sends positive dataSize (int) or negative string size for error.
			else if(socket_int<0) {
				char* tempCharArray = new char[-socket_int];
				if(read(sock,tempCharArray,-socket_int)!=-socket_int) {
					cerr<<"ERROR: Unable to read error string from server."<<endl;
					cerr<<"ERROR: "<<strerror(errno)<<endl;
				}
				else {
					string tempString;
					tempString.assign(tempCharArray,-socket_int);
					cerr<<"ERROR: Error response string from server follows:"<<endl;
					cerr<<"SERVER: "<<tempString<<endl;
					cerr<<"ERROR: End of string from server."<<endl;
				}
				delete[] tempCharArray;
				close(file_fd);
				exit(EXIT_FAILURE);
			}
			else if(socket_int>pageSize) {
				cerr<<"ERROR: Received data segment size larger than the given pagesize."<<endl;
				cerr<<"ERROR: Pagesize: "<<pageSize<<" Received segment size: "<<socket_int<<endl;
				close(file_fd);
				exit(EXIT_FAILURE);
			}
			//		Server sends data (void* data of the above dataSize)
			if(read(sock,tempData,socket_int)!=socket_int) {
				cerr<<"ERROR: Unable to read data segment from server."<<endl;
				cerr<<"ERROR: "<<strerror(errno)<<endl;
				close(file_fd);
				exit(EXIT_FAILURE);
			}
			if(write(file_fd,tempData,socket_int)!=socket_int) {
				cerr<<"ERROR: Unable to write data segment to file."<<endl;
				cerr<<"ERROR: "<<strerror(errno)<<endl;
				close(file_fd);
				exit(EXIT_FAILURE);
			}
			clog<<"DEBUG: "<<socket_int<<" bytes written to file."<<endl;
		}
		//Writer reads from pool again while Client waits for response again
		if(close(file_fd)<0) {
			cerr<<"ERROR: Unable to close file "<<filePathString<<endl;
			cerr<<"ERROR: "<<strerror(errno)<<endl;
		}
	}
	exit(EXIT_SUCCESS);
}

void printUsage() {
	cerr	<<"./remoteClient -i <server_ip> -p <server_port> -d <directory>\n"
			<<"\tserver_ip\tThe server's ip adress or name.\n"
			<<"\tserver_port\tThe server's listen port.\n"
			<<"\tdirectory\tThe directory to be copied from the server.\n"
			<<"Alternatively, you can give the flag -e to request the server to terminate:\n"
			<<"./remoteClient -e -i <server_ip> -p <server_port>\n"
			<<endl;
}

int getArgs(int argc, char **argv, string& ip, int& port, string& directory, bool& exitCond) {
	ip.clear();
	port=-1;
	directory.clear();
	exitCond=false;
	
	char* pIp=NULL;
	char* pPort=NULL;
	char* pDir=NULL;
	
	//First check for exit command
	if(argc==6) {
		for(int i=1;i<6;++i) {
			if(string("-i")==argv[i]) {
				if(pIp!=NULL) {
					cerr<<"ERROR: Duplicate -i flag."<<endl;
					return -1;
				}
				++i;
				pIp=argv[i];
			}
			else if(string("-p")==argv[i]) {
				if(pPort!=NULL) {
					cerr<<"ERROR: Duplicate -p flag."<<endl;
					return -1;
				}
				++i;
				pPort=argv[i];
			}
			else if(string("-e")==argv[i]) {
				if(exitCond==true) {
					cerr<<"ERROR: Duplicate -e flag."<<endl;
					return -1;
				}
				exitCond=true;
			}
			else {
				cerr<<"ERROR: Unknown flag for 5 arguments: "<<argv[i]<<endl;
				printUsage();
				return -1;
			}
		}
		//Convert
		ip.assign(pIp);
		port=stoi(string(pPort));
		
		//Check
		if(port<=0||port>65535) {
			cerr<<"ERROR: Port number must be greater than zero and less than or equal to 65535."<<endl;
			return -1;
		}
		
		return 0;
	}
	
	//Else check for normal operation
	if(argc!=7) {
		cerr<<"ERROR: Need either 6 or 5 arguements arguments."<<endl;
		printUsage();
		return -1;
	}
	
	//Get flags and args
	for(int i=1;i<7;i+=2) {
		if(string("-i")==argv[i]) {
			if(pIp!=NULL) {
				cerr<<"ERROR: Duplicate -i flag."<<endl;
				return -1;
			}
			pIp=argv[i+1];
		}
		else if(string("-p")==argv[i]) {
			if(pPort!=NULL) {
				cerr<<"ERROR: Duplicate -p flag."<<endl;
				return -1;
			}
			pPort=argv[i+1];
		}
		else if(string("-d")==argv[i]) {
			if(pDir!=NULL) {
				cerr<<"ERROR: Duplicate -d flag."<<endl;
				return -1;
			}
			pDir=argv[i+1];
		}
		else {
			cerr<<"ERROR: Unknown flag for 6 arguments: "<<argv[i]<<endl;
			printUsage();
			return -1;
		}
	}
	
	//Convert
	ip.assign(pIp);
	port=stoi(string(pPort));
	directory.assign(pDir);
	
	//Check
	if(port<=0||port>65535) {
		cerr<<"ERROR: Port number must be greater than zero and less than or equal to 65535."<<endl;
		return -1;
	}
	
	return 0;
}

void closeSock() {
	close(sock);
}

void deleteTempArray() {
	delete[] tempData;
}

int createDirIfNecessaryAndOpen(string filePathString) {
	//Keep looping for each folder
	size_t pos=0;
	struct stat fileStat;
	while(true) {
		pos=filePathString.find('/',pos);
		//Break if there are no more folders
		if(pos==string::npos) {
			break;
		}
		++pos;
		string tempFolder=filePathString.substr(0,pos);
		//Stat folder
		if(stat(tempFolder.c_str(),&fileStat)<0) {
			//If folder does not exist
			if(errno==ENOENT) {
				//Create it
				if(mkdir(tempFolder.c_str(),S_IRWXU|S_IRWXG|S_IRWXO)<0) {
					cerr<<"ERROR: Unable to create folder "<<tempFolder<<endl;
					cerr<<"ERROR: "<<strerror(errno)<<endl;
					return -1;
				}
			}
			else {
				cerr<<"ERROR: Unable to stat folder "<<tempFolder<<endl;
				cerr<<"ERROR: "<<strerror(errno)<<endl;
				return -1;
			}
		}
		//If this is no a folder, complain
		if(!S_ISDIR(fileStat.st_mode)) {
			cerr<<"ERROR: "<<tempFolder<<"is not a folder"<<endl;
			return -1;
		}
	}
	//Stat file, if it exists
	errno=0;//Set errno to zero so that we can ignore previous errors
	if(stat(filePathString.c_str(),&fileStat)<0&&errno!=ENOENT) {
		cerr<<"ERROR: Unable to stat file "<<filePathString<<endl;
		cerr<<"ERROR: "<<strerror(errno)<<endl;
		return -1;
	}
	if(errno==0) {//That means that the file exists (errno was not set to ENOENT) and fileStat now holds its stats
		if(S_ISDIR(fileStat.st_mode)) {
			cerr<<"ERROR: "<<filePathString<<"is a folder"<<endl;
			return -1;
		}
		if(unlink(filePathString.c_str())<0) {
			cerr<<"ERROR: Unable to delete old file "<<filePathString<<endl;
			cerr<<"ERROR: "<<strerror(errno)<<endl;
			return -1;
		}
	}
	//Open file, delete its contents and return the file descriptor
	int retFD=open(filePathString.c_str(),O_CREAT|O_WRONLY|O_TRUNC,S_IRWXU|S_IRWXG|S_IRWXO);
	if(retFD<0) {
		cerr<<"ERROR: Unable to create and open file "<<filePathString<<endl;
		cerr<<"ERROR: "<<strerror(errno)<<endl;
		return -1;
	}
	return retFD;
}