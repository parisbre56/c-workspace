#include <iostream>
#include <cstdlib>
#include <string>


#include <sys/mman.h>
#include <sys/stat.h>			/* For mode constants */
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>				/* For O_* constants */

#include "jobList.hpp"
#include "shared_data.hpp"		//NOTE: THIS INCLUDES THE GLOBAL VARIABLE bool_arr

using namespace std;

#define DEFAULT_CONCURRENCY 2

JobList jobAll;

/* Registered with the atexit command
 * to delete all job objects at exit and prevent memory leaks.
 * Will only be useful when an error occurs.
 */
void destroyJobAll();

/* Registered with the atexit command
 * to notify all other processes that this one has stopped.
 */
void setMemoryToIndicateExit();

int main(int argc, char **argv)
{
	int shm_fd=-1; //Shared memory file descriptor
	bool_arr=NULL; //Shared memory boolean array !THIS ALWAYS POINTS TO THE START OF THE SMS EVEN IF IT IS EMPTY!
	int* int_arr=NULL; //Shared memory integer array
	sem_t* sema_arr=NULL; //Shared memory semaphore array
	
	//file descriptor for named pipe
	int pipe_fd=-1;
	//Used to send things to the pipe
	string exec_string;
	int exec_int=0;
	char* rString=NULL;
	bool rBool=false;
	
	//Holds data for the received signal
	siginfo_t signalInfo;

	
	//jobAll declared as global to be used atexit.
	atexit(destroyJobAll);
	JobList jobRunning;
	JobList jobQueued;
	Job* tempJob;
	
	int concurrency=DEFAULT_CONCURRENCY;
	//holds the next free ID to be given to the next job
	int freeID=0;
	
	
	
	//Connect to shared memory. If it doesn't exist, notify the user and exit
	if((shm_fd=shm_open("/jobCommander",O_RDWR,S_IRWXU|S_IRWXG|S_IRWXO))<0) {
		cerr	<<"ERROR: Unable to open shared memory.\n"
				<<"ERROR: Are you sure you are using the jobCommander?"<<endl;
		perror("jobExecutor");
		exit(EXIT_FAILURE);
	}
	
	//Get the memory mappings
	if(getSMSAdress(shm_fd,bool_arr,int_arr,sema_arr)<0) {
		cerr	<<"ERROR: An error occured while mapping the shared memory segment."<<endl;
		perror("jobExecutor");
		shm_unlink("/jobCommander");
		exit(EXIT_FAILURE);
	}
	
	//Clear memory at exit.
	atexit(detachSharedMemory);
	//We are certain that the functions will be called in the reverse order they were inserted
	//so now we can be certain that when the process stops due to an error, all others will know.
	atexit(setMemoryToIndicateExit);
	
	//Set up data
	//Pipes
	if(mkfifo("comm_pipe.temp",S_IRWXU|S_IRWXG|S_IRWXO)<0) {
		cerr	<<"ERROR: Unable to create named pipe."<<endl;
		perror("jobExecutor");
		exit(EXIT_FAILURE);
	}
	if((pipe_fd=open("comm_pipe.temp",O_RDONLY|O_NONBLOCK))<0) {
		cerr<<"ERROR: Unable to open pipe for reading."<<endl;
		perror("jobExecutor");
		exit(EXIT_FAILURE);
	}
	//Pid
	int_arr[EXEC_PID_INT]=getpid();
	int_arr[READER_PID_INT]=-1;
	
	//Signals to process
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask,SIGUSR1);
	//sigaddset(&mask,SIGUSR2);
	sigaddset(&mask,SIGCHLD);
	
	//Notify the shared memory that you are ready
	bool_arr[SMS_READY_BOOL]=true;
	
	//Start looping, waiting for signals and handling them
	//Exit condition is EXEC_EXITING_BOOL is true and there are no jobs left in the queue
	while(!bool_arr[EXEC_EXITING_BOOL]&&jobAll.inList>0) {
		//Wait for signal
		if(sigwaitinfo(&mask,&signalInfo)<0) {
			cerr<<"ERROR: Error while waiting for signal."<<endl;
			perror("jobExecutor");
			exit(EXIT_FAILURE);
		}
		//Check signal type and handle it (either child or read from pipe signal)
		if(signalInfo.si_signo==SIGCHLD) {
			
		}
		else if() {
			
		}
		else {
			
		}

	}
	//Once the exit command has been given, destroy any data left and exit
	//Remove named pipe with unlink
	
	exit(EXIT_SUCCESS);
}

void destroyJobAll() {
	jobAll.deleteAll();
}

void setMemoryToIndicateExit() {
	bool_arr[EXEC_NO_LONGER_RUNNING_BOOL]=true;
	bool_arr[EXEC_EXITING_BOOL]=true;
}