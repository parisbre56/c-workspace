//GENERAL COMM PROTOCOL: 
//		ALWAYS SEND <ORDER SIZE> (INT) 
//		FOLLOWED BY 
//		<ORDER NAME (CHAR ARRAY OF SIZE <ORDER SIZE>)> 
//		what follows depends on the order
//		
//		ALWAYS RECEIVE AN INT WHICH SIGNALS ERROR IF LESS THAN 0

#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include <cerrno>
#include <cstdlib>
#include <cstdio>

#include <fcntl.h>				/* For O_* constants */
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>			/* For mode constants */

#include "shared_data.hpp"		//NOTE: THIS INCLUDES THE GLOBAL VARIABLE bool_arr

using namespace std;

/* Prints a small message detailing the usage of this 
 * command.
 */
void printUsage (ostream& out = cout);

/* Exactly what it says on the tin.
 * If the shared memory segment is created, it also initialises the semaphores.
 * Returns negative for failure or the file descriptor of the shared memory
 * segment (positive) on success. It is up to the calling function to check if the
 * job commander is running and ready and hence if the data here is valid. If 
 * successful, registers detachSharedMemory to be called atexit.
 */
int createJobExecutorIfNecessaryAndReturnSharedMemory(bool*& bool_arr, int*& int_arr, sem_t*& sema_arr);

/* Exactly what it says on the tin.
 * Returns negative for failure (failure includes JobExecutor does not exist)
 * or the file descriptor of the shared memory segment (positive) on success.
 * It is up to the calling function to check if the job commander is running 
 * and ready and hence if the data here is valid. If successful, registers
 * detachSharedMemory to be called atexit.
 */
int checkIfJobExecutorExistsAndIfYesReturnSharedMemory(bool*& bool_arr, int*& int_arr, sem_t*& sema_arr);

int main(int argc, char **argv)
{
	
	int shm_fd=-1; //Shared memory file descriptor
	bool_arr=NULL; //Shared memory boolean array !THIS ALWAYS POINTS TO THE START OF THE SMS EVEN IF IT IS EMPTY!
	int* int_arr=NULL; //Shared memory integer array
	sem_t* sema_arr=NULL; //Shared memory semaphore array
	
	struct timespec ts; //Used for semaphore timed_wait
	
	int pipe_fd=-1; //file descriptor for named pipe
	string exec_string; //used to pass things to the executor via the named pipe
	int exec_int=0; //used to pass things to the executor via the named pipe
	char* rString=NULL; //Used as a buffer for reading from the pipe
	bool rBool=false; //Used to read from pipe
	
	//Check args
	if(argc<2) {
		cerr<<"ERROR: Need at least 1 arguement."<<endl;
		printUsage();
		exit(EXIT_FAILURE);
	}
	string firstArg (argv[1]);
	if(!firstArg.compare("issuejob")) {
		//Check for the existence of second argument
		if(argc<3) {
			cerr<<"ERROR: Missing argument. Need the job's name."<<endl;
			printUsage();
			exit(EXIT_FAILURE);
		}
		//Check for the existance of Job commander
		shm_fd=createJobExecutorIfNecessaryAndReturnSharedMemory(bool_arr,int_arr,sema_arr);
		if(shm_fd<0) {
			cerr<<"ERROR: Error while checking for existance of jobCommander and creating it if necessary."<<endl;
			perror("JobCommander");
			exit(EXIT_FAILURE);
		}
		//Wait until shared memory is ready
		while(!bool_arr[SMS_READY_BOOL]) {
			this_thread::sleep_for(chrono::seconds(1));
		}
		//Check if exiting or exited and display appropriate message
		if(bool_arr[EXEC_EXITING_BOOL]) {
			cerr<<"ERROR: Can't issue job because jobExecutor is in the process of exiting.\n"
					"ERROR: Wait for it to exit and try again."<<endl;
					exit(EXIT_FAILURE);
		}
		
		//Get time for timed semaphore wait.
		if(clock_gettime(CLOCK_REALTIME, &ts)<0) {
			cerr<<"ERROR: Can't get current time for timeout."<<endl;
			perror("jobCommander");
			exit(EXIT_FAILURE);
		}
		ts.tv_sec += TIMEOUT_SECS;
		
		//Wait until semaphore is open or until timeout (timeout should be long enough)
		if(sem_timedwait(&(sema_arr[EXEC_ACCESS_SEMA]),&ts)<0) {
				//Check for error or timeout
				cerr<<"ERROR: Error during semaphore time wait. Are you sure the executor is running?"<<endl;
				perror("jobCommander");
				exit(EXIT_FAILURE);
			}		
			//Check again to make sure the executor is fine
			if(bool_arr[EXEC_EXITING_BOOL]||bool_arr[EXEC_NO_LONGER_RUNNING_BOOL]) {
				cerr<<"ERROR: Can't issue job because jobExecutor is in the process of exiting.\n"
						"ERROR: Wait for it to exit and try again."<<endl;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}
			
			//Put PID in shared memory
			int_arr[READER_PID_INT]=getpid();
			
			//Open pipe for writing (other has opened with no block)
			if((pipe_fd=open("comm_pipe.temp",O_WRONLY|O_NONBLOCK))<0) {
				cerr<<"ERROR: Unable to open pipe for writing."<<endl;
				perror("jobCommander");
				int_arr[READER_PID_INT]=-1;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}
			
			//Write in pipe
			/* Communication protocol is as follows (no newlines written):
			 * 	<order_size(int)><order_name(char array of size <order_size>)>
			 * 	<argument_number(int)>
			 * 	<first_argument_size(int)><first_argument(char array of size <first_argument_size>)>
			 * 		...
			 * 	<<argument_number>_argument_size(int)>
			 * 	<<argument_number>_argument(char array of size <<argument_number>_argument_size>)>
			 * Communication ends
			 */
			
			
			exec_int=firstArg.length(); //Order size compute
			write(pipe_fd,&exec_int,sizeof(int)); //Order size write
			write(pipe_fd,firstArg.c_str(),firstArg.length()); //Order name write
			exec_int=argc-2; //Argument number compute
			write(pipe_fd,&exec_int,sizeof(int)); //Argument number write.
			for(int i=2;i<argc;++i) { //For all arguments
				exec_string.assign(argv[i]); //Compute argument
				exec_int=exec_string.length(); //Compute argument size
				write(pipe_fd,&exec_int,sizeof(int)); //Write argument size
				write(pipe_fd,exec_string.c_str(),exec_string.length()); //Write argument
			}
			
			//Close pipe
			if(close(pipe_fd)<0) {
				cerr<<"ERROR: Unable to close pipe"<<endl;
				perror("jobCommander");
				int_arr[READER_PID_INT]=-1;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}
			
			//Send signal1 for reading
			if(kill(int_arr[EXEC_PID_INT],SIGUSR1)<0) {
				cerr<<"ERROR: Unable to send signal"<<endl;
				perror("jobCommander");
				int_arr[READER_PID_INT]=-1;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}

			//Open pipe for reading (other has opened with block)
			if((pipe_fd=open("comm_pipe.temp",O_RDONLY))<0) {
				cerr<<"ERROR: Unable to open pipe for reading."<<endl;
				perror("jobCommander");
				int_arr[READER_PID_INT]=-1;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}
			
			//Read returned data (other process starts waiting for a signal again) and display it
			/* Communication protocol is as follows (no newlines written):
			 * 	<returned_id(int)> less than 0 for error.
			 * 	if there was no error:
			 * 	<job_name_size(int)>
			 * 	<job_name(char array of size <job_name_size>)>
			 * 	<running(bool)>
			 * Communication ends
			 */
			if(read(pipe_fd,&exec_int,sizeof(int))<0) { //Read pid
				cerr<<"ERROR: Unable to read returned data from pipe."<<endl;
				perror("jobCommander");
				int_arr[READER_PID_INT]=-1;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}
			//Check for error
			if(exec_int<0) {
				cerr<<"ERROR: Received error "<<exec_int<<" from jobExecutor."<<endl;
				int_arr[READER_PID_INT]=-1;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}
			
			cout<<"jobID: "<<exec_int<<endl;
			
			read(pipe_fd,&exec_int,sizeof(int)); //Read size of name
			rString=new char[exec_int];
			read(pipe_fd,rString,exec_int);
			exec_string.assign(rString,exec_int);
			delete[] rString;
			
			cout<<"Job name: "<<exec_string<<endl;
			
			read(pipe_fd,&rBool,sizeof(bool));
			
			cout<<"Running: "<<boolalpha<<rBool<<endl;

			//Close pipe
			if(close(pipe_fd)<0) {
				cerr<<"ERROR: Unable to close pipe"<<endl;
				perror("jobCommander");
				int_arr[READER_PID_INT]=-1;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}
			
			//Remove pid from shared memory
			int_arr[READER_PID_INT]=-1;
			
		//The executor will post the semaphore to allow others to communicate
		
	}
	else if(!firstArg.compare("setConcurrency")) {
		//Check for the existence of second argument
		if(argc<3) {
			cerr<<"ERROR: Missing argument. Need concurrency (positive integer)."<<endl;
			printUsage();
			exit(EXIT_FAILURE);
		}
		//Check second argument
		int arg_int=atoi(argv[2]);
		if(arg_int<0) {
			cerr<<"ERROR: Cuncurrency must be a positive integer (greater than or equal to zero)."<<endl;
			printUsage();
			exit(EXIT_FAILURE);
		}
		if(arg_int==0) {
			cerr	<<"WARNING: Setting concurrency to 0!\n"
					<<"WARNING: No new jobs will be executed until it is set to a number greater than zero!"<<endl;
		}
		
		//Check for the existance of Job commander
		shm_fd=createJobExecutorIfNecessaryAndReturnSharedMemory(bool_arr,int_arr,sema_arr);
		if(shm_fd<0) {
			cerr<<"ERROR: Error while checking for existance of jobCommander and creating it if necessary."<<endl;
			perror("JobCommander");
			exit(EXIT_FAILURE);
		}
		//Wait until shared memory is ready
		while(!bool_arr[SMS_READY_BOOL]) {
			this_thread::sleep_for(chrono::seconds(1));
		}
		//Check if exiting or exited and display appropriate message
		if(bool_arr[EXEC_NO_LONGER_RUNNING_BOOL]) {
			cerr	<<"ERROR: Can't issue order because jobExecutor has stopped\n"
					<<"ERROR: excuting jobs and is in the process of exiting.\n"
					<<"ERROR: Wait for it to exit and try again."<<endl;
			exit(EXIT_FAILURE);
		}
		
		//Get time for timed semaphore wait.
		if(clock_gettime(CLOCK_REALTIME, &ts)<0) {
			cerr<<"ERROR: Can't get current time for timeout."<<endl;
			perror("jobCommander");
			exit(EXIT_FAILURE);
		}
		ts.tv_sec += TIMEOUT_SECS;
		
		//Wait until semaphore is open or until timeout (timeout should be long enough)
		if(sem_timedwait(&(sema_arr[EXEC_ACCESS_SEMA]),&ts)<0) {
				//Check for error or timeout
				cerr<<"ERROR: Error during semaphore time wait. Are you sure the executor is running?"<<endl;
				perror("jobCommander");
				exit(EXIT_FAILURE);
			}		
			//Check again to make sure the executor is fine
			if(bool_arr[EXEC_NO_LONGER_RUNNING_BOOL]) {
				cerr	<<"ERROR: Can't issue order because jobExecutor has stopped\n"
						<<"ERROR: excuting jobs and is in the process of exiting.\n"
						<<"ERROR: Wait for it to exit and try again."<<endl;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}
			
			//Put PID in shared memory
			int_arr[READER_PID_INT]=getpid();
			
			//Open pipe for writing (other has opened with no block)
			if((pipe_fd=open("comm_pipe.temp",O_WRONLY|O_NONBLOCK))<0) {
				cerr<<"ERROR: Unable to open pipe for writing."<<endl;
				perror("jobCommander");
				int_arr[READER_PID_INT]=-1;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}
			
			//Write in pipe
			/* Communication protocol is as follows (no newlines written):
			 * 	<order_size(int)><order_name(char array of size <order_size>)>
			 * 	<concurrency(int)>
			 * Communication ends
			 */
			
			exec_int=firstArg.length(); //Order size compute
			write(pipe_fd,&exec_int,sizeof(int)); //Order size write
			write(pipe_fd,firstArg.c_str(),firstArg.length()); //Order name write
			write(pipe_fd,&arg_int,sizeof(int)); //Argument integer write
			
			//Close pipe
			if(close(pipe_fd)<0) {
				cerr<<"ERROR: Unable to close pipe"<<endl;
				perror("jobCommander");
				int_arr[READER_PID_INT]=-1;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}
			
			//Send signal1 for reading
			if(kill(int_arr[EXEC_PID_INT],SIGUSR1)<0) {
				cerr<<"ERROR: Unable to send signal"<<endl;
				perror("jobCommander");
				int_arr[READER_PID_INT]=-1;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}

			//Open pipe for reading (other has opened with block)
			if((pipe_fd=open("comm_pipe.temp",O_RDONLY))<0) {
				cerr<<"ERROR: Unable to open pipe for reading."<<endl;
				perror("jobCommander");
				int_arr[READER_PID_INT]=-1;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}
			
			//Read returned data (other process starts waiting for a signal again) and display it
			/* Communication protocol is as follows (no newlines written):
			 * 	<returned_concurrency(int)> less than 0 for error.
			 * Communication ends
			 */
			if(read(pipe_fd,&exec_int,sizeof(int))<0) { //Read state
				cerr<<"ERROR: Unable to read returned data from pipe."<<endl;
				perror("jobCommander");
				int_arr[READER_PID_INT]=-1;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}
			//Check for error
			if(exec_int<0) {
				cerr<<"ERROR: Received error "<<exec_int<<" from jobExecutor."<<endl;
				int_arr[READER_PID_INT]=-1;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}
			
			cout<<"Concurency set to: "<<exec_int<<endl;
			
			//Close pipe
			if(close(pipe_fd)<0) {
				cerr<<"ERROR: Unable to close pipe"<<endl;
				perror("jobCommander");
				int_arr[READER_PID_INT]=-1;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}
			
			//Remove pid from shared memory
			int_arr[READER_PID_INT]=-1;
			
		//The executor will post the semaphore to allow others to communicate
		
	}
	else if(!firstArg.compare("stop")) {
		//Check for the existence of second argument
		if(argc<3) {
			cerr<<"ERROR: Missing argument. Need jobID (positive integer)."<<endl;
			printUsage();
			exit(EXIT_FAILURE);
		}
		//Check second argument
		int arg_int=atoi(argv[2]);
		if(arg_int<0) {
			cerr<<"ERROR: jobID must be a positive integer (greater than or equal to zero)."<<endl;
			printUsage();
			exit(EXIT_FAILURE);
		}
		
		//Check for the existance of Job commander
		shm_fd=checkIfJobExecutorExistsAndIfYesReturnSharedMemory(bool_arr,int_arr,sema_arr);
		if(shm_fd<0) {
			cerr<<"ERROR: Error while checking for existance of jobCommander. Are you sure it is running?"<<endl;
			perror("JobCommander");
			exit(EXIT_FAILURE);
		}
		//Wait until shared memory is ready
		while(!bool_arr[SMS_READY_BOOL]) {
			this_thread::sleep_for(chrono::seconds(1));
		}
		//Check if exiting or exited and display appropriate message
		if(bool_arr[EXEC_NO_LONGER_RUNNING_BOOL]) {
			cerr	<<"ERROR: Can't issue order because jobExecutor has stopped\n"
					<<"ERROR: excuting jobs and is in the process of exiting.\n"
					<<"ERROR: Wait for it to exit and try again."<<endl;
					exit(EXIT_FAILURE);
		}
		
		//Get time for timed semaphore wait.
		if(clock_gettime(CLOCK_REALTIME, &ts)<0) {
			cerr<<"ERROR: Can't get current time for timeout."<<endl;
			perror("jobCommander");
			exit(EXIT_FAILURE);
		}
		ts.tv_sec += TIMEOUT_SECS;
		
		//Wait until semaphore is open or until timeout (timeout should be long enough)
		if(sem_timedwait(&(sema_arr[EXEC_ACCESS_SEMA]),&ts)<0) {
				//Check for error or timeout
				cerr<<"ERROR: Error during semaphore time wait. Are you sure the executor is running?"<<endl;
				perror("jobCommander");
				exit(EXIT_FAILURE);
			}		
			//Check again to make sure the executor is fine
			if(bool_arr[EXEC_NO_LONGER_RUNNING_BOOL]) {
				cerr	<<"ERROR: Can't issue order because jobExecutor has stopped\n"
						<<"ERROR: excuting jobs and is in the process of exiting.\n"
						<<"ERROR: Wait for it to exit and try again."<<endl;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}
			
			//Put PID in shared memory
			int_arr[READER_PID_INT]=getpid();
			
			//Open pipe for writing (other has opened with no block)
			if((pipe_fd=open("comm_pipe.temp",O_WRONLY|O_NONBLOCK))<0) {
				cerr<<"ERROR: Unable to open pipe for writing."<<endl;
				perror("jobCommander");
				int_arr[READER_PID_INT]=-1;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}
			
			//Write in pipe
			/* Communication protocol is as follows (no newlines written):
			 * 	<order_size(int)><order_name(char array of size <order_size>)>
			 * 	<jobID(int)>
			 * Communication ends
			 */
			
			exec_int=firstArg.length(); //Order size compute
			write(pipe_fd,&exec_int,sizeof(int)); //Order size write
			write(pipe_fd,firstArg.c_str(),firstArg.length()); //Order name write
			write(pipe_fd,&arg_int,sizeof(int)); //Argument integer write
			
			//Close pipe
			if(close(pipe_fd)<0) {
				cerr<<"ERROR: Unable to close pipe"<<endl;
				perror("jobCommander");
				int_arr[READER_PID_INT]=-1;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}
			
			//Send signal1 for reading
			if(kill(int_arr[EXEC_PID_INT],SIGUSR1)<0) {
				cerr<<"ERROR: Unable to send signal"<<endl;
				perror("jobCommander");
				int_arr[READER_PID_INT]=-1;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}

			//Open pipe for reading (other has opened with block)
			if((pipe_fd=open("comm_pipe.temp",O_RDONLY))<0) {
				cerr<<"ERROR: Unable to open pipe for reading."<<endl;
				perror("jobCommander");
				int_arr[READER_PID_INT]=-1;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}
			
			//Read returned data (other process starts waiting for a signal again) and display it
			/* Communication protocol is as follows (no newlines written):
			 * 	<returned_state(int)> less than 0 for error.
			 * 	0 means it doesn't exist
			 * 	1 means it was stopped while in queue
			 * 	2 means it was stopped during execution
			 * Communication ends
			 */
			if(read(pipe_fd,&exec_int,sizeof(int))<0) { //Read state
				cerr<<"ERROR: Unable to read returned data from pipe."<<endl;
				perror("jobCommander");
				int_arr[READER_PID_INT]=-1;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}
			//Check for error
			if(exec_int<0) {
				cerr<<"ERROR: Received error "<<exec_int<<" from jobExecutor."<<endl;
				int_arr[READER_PID_INT]=-1;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}
			if(exec_int==0) {
				cout<<"There is no job with ID "<<arg_int<<endl;
				cout<<"Maybe it has finished execution?"<<endl;
			}
			else if(exec_int==1) {
				cout<<"Job "<<arg_int<<" succesfully removed from queue."<<endl;
			}
			else if(exec_int==2) {
				cout	<<"Job "<<arg_int<<" was given the kill signal while running.\n"<<
						"It should stop soon, if it hasn't already."<<endl;
			}
			else {
				cerr<<"WARNING: Received unknown success indicator "<<exec_int<<" from jobExecutor."<<endl;
			}
			
			//Close pipe
			if(close(pipe_fd)<0) {
				cerr<<"ERROR: Unable to close pipe"<<endl;
				perror("jobCommander");
				int_arr[READER_PID_INT]=-1;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}
			
			//Remove pid from shared memory
			int_arr[READER_PID_INT]=-1;
			
		//The executor will post the semaphore to allow others to communicate
	}
	else if(!firstArg.compare("poll")) {
		//Check for the existence of second argument
		if(argc<3) {
			cerr<<"ERROR: Missing argument. Need either running or queued as second argument."<<endl;
			printUsage();
			exit(EXIT_FAILURE);
		}
		//Check second argument
		string arg_string (argv[2]);
		if(arg_string.compare("running")&&arg_string.compare("queued")) {
			cerr<<"ERROR: Wrong argument. Need either running or queued as second argument."<<endl;
			printUsage();
			exit(EXIT_FAILURE);
		}
		
		//Check for the existance of Job commander
		shm_fd=checkIfJobExecutorExistsAndIfYesReturnSharedMemory(bool_arr,int_arr,sema_arr);
		if(shm_fd<0) {
			cerr<<"ERROR: Error while checking for existance of jobCommander. Are you sure it is running?"<<endl;
			perror("JobCommander");
			exit(EXIT_FAILURE);
		}
		//Wait until shared memory is ready
		while(!bool_arr[SMS_READY_BOOL]) {
			this_thread::sleep_for(chrono::seconds(1));
		}
		//Check if exiting or exited and display appropriate message
		if(bool_arr[EXEC_NO_LONGER_RUNNING_BOOL]) {
			cerr	<<"ERROR: Can't issue order because jobExecutor has stopped\n"
					<<"ERROR: excuting jobs and is in the process of exiting.\n"
					<<"ERROR: Wait for it to exit and try again."<<endl;
					exit(EXIT_FAILURE);
		}
		
		//Get time for timed semaphore wait.
		if(clock_gettime(CLOCK_REALTIME, &ts)<0) {
			cerr<<"ERROR: Can't get current time for timeout."<<endl;
			perror("jobCommander");
			exit(EXIT_FAILURE);
		}
		ts.tv_sec += TIMEOUT_SECS;
		
		//Wait until semaphore is open or until timeout (timeout should be long enough)
		if(sem_timedwait(&(sema_arr[EXEC_ACCESS_SEMA]),&ts)<0) {
				//Check for error or timeout
				cerr<<"ERROR: Error during semaphore time wait. Are you sure the executor is running?"<<endl;
				perror("jobCommander");
				exit(EXIT_FAILURE);
			}		
			//Check again to make sure the executor is fine
			if(bool_arr[EXEC_NO_LONGER_RUNNING_BOOL]) {
				cerr	<<"ERROR: Can't issue order because jobExecutor has stopped\n"
						<<"ERROR: excuting jobs and is in the process of exiting.\n"
						<<"ERROR: Wait for it to exit and try again."<<endl;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}
			
			//Put PID in shared memory
			int_arr[READER_PID_INT]=getpid();
			
			//Open pipe for writing (other has opened with no block)
			if((pipe_fd=open("comm_pipe.temp",O_WRONLY|O_NONBLOCK))<0) {
				cerr<<"ERROR: Unable to open pipe for writing."<<endl;
				perror("jobCommander");
				int_arr[READER_PID_INT]=-1;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}
			
			//Write in pipe
			/* Communication protocol is as follows (no newlines written):
			 * 	<order_size(int)><order_name(char array of size <order_size>)>
			 * 	<arg_size(int)><arg_name(char array of size <arg_size>)>
			 * Communication ends
			 */
			
			exec_int=firstArg.length(); //Order size compute
			write(pipe_fd,&exec_int,sizeof(int)); //Order size write
			write(pipe_fd,firstArg.c_str(),firstArg.length()); //Order name write
			
			exec_int=arg_string.length(); //Argument size compute
			write(pipe_fd,&exec_int,sizeof(int)); //Argument size write
			write(pipe_fd,arg_string.c_str(),arg_string.length()); //Argument name write
			
			//Close pipe
			if(close(pipe_fd)<0) {
				cerr<<"ERROR: Unable to close pipe"<<endl;
				perror("jobCommander");
				int_arr[READER_PID_INT]=-1;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}
			
			//Send signal1 for reading
			if(kill(int_arr[EXEC_PID_INT],SIGUSR1)<0) {
				cerr<<"ERROR: Unable to send signal"<<endl;
				perror("jobCommander");
				int_arr[READER_PID_INT]=-1;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}

			//Open pipe for reading (other has opened with block)
			if((pipe_fd=open("comm_pipe.temp",O_RDONLY))<0) {
				cerr<<"ERROR: Unable to open pipe for reading."<<endl;
				perror("jobCommander");
				int_arr[READER_PID_INT]=-1;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}
			
			//Read returned data (other process starts waiting for a signal again) and display it
			/* Communication protocol is as follows (no newlines written):
			 * 	<argument_number(int)> less than 0 for error.
			 * 		<first_job_id(int)> less than 0 for error.
			 * 		<first_job_size(int)>
			 * 		<first_job_name(char array of size <job_size>)>
			 * 			...
			 * 		<<argument_number>_job_id(int)> less than 0 for error.
			 * 		<<argument_number>_job_size(int)>
			 * 		<<argument_number>_job_name(char array of size <job_size>)>
			 * Communication ends
			 */
			 
			if(read(pipe_fd,&exec_int,sizeof(int))<0) { //Read state
				cerr<<"ERROR: Unable to read returned data from pipe."<<endl;
				perror("jobCommander");
				int_arr[READER_PID_INT]=-1;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}
			//Check for error
			if(exec_int<0) {
				cerr<<"ERROR: Received error "<<exec_int<<" from jobExecutor."<<endl;
				int_arr[READER_PID_INT]=-1;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}
			cout<<exec_int<<" jobs "<<arg_string<<endl;
			
			for(int i=exec_int;i>0;--i) {
				if(read(pipe_fd,&exec_int,sizeof(int))<0) { //Read id
					cerr<<"ERROR: Unable to read returned data from pipe."<<endl;
					perror("jobCommander");
					int_arr[READER_PID_INT]=-1;
					sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
					exit(EXIT_FAILURE);
				}
				//Check for error
				if(exec_int<0) {
					cerr<<"ERROR: Received error "<<exec_int<<" from jobExecutor."<<endl;
					int_arr[READER_PID_INT]=-1;
					sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
					exit(EXIT_FAILURE);
				}
				
				cout<<"JobID: "<<exec_int;
				
				if(read(pipe_fd,&exec_int,sizeof(int))<0) { //Read size
					cerr<<"ERROR: Unable to read returned data from pipe."<<endl;
					perror("jobCommander");
					int_arr[READER_PID_INT]=-1;
					sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
					exit(EXIT_FAILURE);
				}
				rString=new char[exec_int];
				read(pipe_fd,rString,exec_int); //Read job name
				exec_string.assign(rString,exec_int);
				delete[] rString;
				
				cout<<" Job: "<<exec_string<<endl;
			}
			
			//Close pipe
			if(close(pipe_fd)<0) {
				cerr<<"ERROR: Unable to close pipe"<<endl;
				perror("jobCommander");
				int_arr[READER_PID_INT]=-1;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}
			
			//Remove pid from shared memory
			int_arr[READER_PID_INT]=-1;
			
		//The executor will post the semaphore to allow others to communicate
	}
	else if(!firstArg.compare("exit")) {
		//Check for the existance of Job commander
		shm_fd=checkIfJobExecutorExistsAndIfYesReturnSharedMemory(bool_arr,int_arr,sema_arr);
		if(shm_fd<0) {
			cerr<<"ERROR: Error while checking for existance of jobCommander. Are you sure it is running?"<<endl;
			perror("JobCommander");
			exit(EXIT_FAILURE);
		}
		//Wait until shared memory is ready
		while(!bool_arr[SMS_READY_BOOL]) {
			this_thread::sleep_for(chrono::seconds(1));
		}
		//Check if exiting or exited and display appropriate message
		if(bool_arr[EXEC_EXITING_BOOL]||bool_arr[EXEC_NO_LONGER_RUNNING_BOOL]) {
			cerr<<"ERROR: Can't issue order because jobExecutor is already in the process of exiting."<<endl;
					exit(EXIT_FAILURE);
		}
		
		//Get time for timed semaphore wait.
		if(clock_gettime(CLOCK_REALTIME, &ts)<0) {
			cerr<<"ERROR: Can't get current time for timeout."<<endl;
			perror("jobCommander");
			exit(EXIT_FAILURE);
		}
		ts.tv_sec += TIMEOUT_SECS;
		
		//Wait until semaphore is open or until timeout (timeout should be long enough)
		if(sem_timedwait(&(sema_arr[EXEC_ACCESS_SEMA]),&ts)<0) {
				//Check for error or timeout
				cerr<<"ERROR: Error during semaphore time wait. Are you sure the executor is running?"<<endl;
				perror("jobCommander");
				exit(EXIT_FAILURE);
			}		
			//Check again to make sure the executor is fine
			if(bool_arr[EXEC_EXITING_BOOL]||bool_arr[EXEC_NO_LONGER_RUNNING_BOOL]) {
				cerr<<"ERROR: Can't issue order because jobExecutor is already in the process of exiting."<<endl;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}
			
			//Put PID in shared memory
			int_arr[READER_PID_INT]=getpid();
			
			//Open pipe for writing (other has opened with no block)
			if((pipe_fd=open("comm_pipe.temp",O_WRONLY|O_NONBLOCK))<0) {
				cerr<<"ERROR: Unable to open pipe for writing."<<endl;
				perror("jobCommander");
				int_arr[READER_PID_INT]=-1;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}
			
			//Write in pipe
			/* Communication protocol is as follows (no newlines written):
			 * 	<order_size(int)><order_name(char array of size <order_size>)>
			 * Communication ends
			 */
			
			exec_int=firstArg.length(); //Order size compute
			write(pipe_fd,&exec_int,sizeof(int)); //Order size write
			write(pipe_fd,firstArg.c_str(),firstArg.length()); //Order name write
			
			//Close pipe
			if(close(pipe_fd)<0) {
				cerr<<"ERROR: Unable to close pipe"<<endl;
				perror("jobCommander");
				int_arr[READER_PID_INT]=-1;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}
			
			//Send signal1 for reading
			if(kill(int_arr[EXEC_PID_INT],SIGUSR1)<0) {
				cerr<<"ERROR: Unable to send signal"<<endl;
				perror("jobCommander");
				int_arr[READER_PID_INT]=-1;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}

			//Open pipe for reading (other has opened with block)
			if((pipe_fd=open("comm_pipe.temp",O_RDONLY))<0) {
				cerr<<"ERROR: Unable to open pipe for reading."<<endl;
				perror("jobCommander");
				int_arr[READER_PID_INT]=-1;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}
			
			//Read returned data (other process starts waiting for a signal again) and display it
			/* Communication protocol is as follows (no newlines written):
			 * 	<response(int)> less than 0 for error.
			 * Communication ends
			 */
			 
			if(read(pipe_fd,&exec_int,sizeof(int))<0) { //Read response
				cerr<<"ERROR: Unable to read returned data from pipe."<<endl;
				perror("jobCommander");
				int_arr[READER_PID_INT]=-1;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}
			//Check for error
			if(exec_int<0) {
				cerr<<"ERROR: Received error "<<exec_int<<" from jobExecutor."<<endl;
				int_arr[READER_PID_INT]=-1;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}
			
			//Close pipe
			if(close(pipe_fd)<0) {
				cerr<<"ERROR: Unable to close pipe"<<endl;
				perror("jobCommander");
				int_arr[READER_PID_INT]=-1;
				sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
				exit(EXIT_FAILURE);
			}
			
			//Remove pid from shared memory
			int_arr[READER_PID_INT]=-1;
			
		//The executor will post the semaphore to allow others to communicate
	}
	else {
		cerr<<"ERROR: Unkown argument \""<<firstArg<<"\""<<endl;
		printUsage();
		exit(EXIT_FAILURE);
	}
	
	clog<<"DEBUG: Exited succesfully."<<endl;
	exit(EXIT_SUCCESS);
}

void printUsage(ostream& out) {
	out
		<<"Usage:\n"
		<<"./jobCommander issuejob <job> [args]\n"
		<<"\tPuts job <job> with optional arguments [args] in the job queue.\n"
		<<"\tOutputs the jobID, the job and its status (running or waiting).\n"
		<<"./jobCommander setConcurrency <N>\n"
		<<"\tSets the number of jobs that can be executed at the same time.\n" 
		<<"\tIf it is set less than current number of jobs running, no jobs\n"
		<<"\twill be paused or stopped.\n"
		<<"./jobCommander stop <jobID>\n"
		<<"\tStops the execution of job <jobID> if it is running or removes\n"
		<<"\tit from the queue if it is not.\n"
		<<"./jobCommander poll [running,queued]\n"
		<<"\tOutputs the jobID and jobs that are either running or waiting\n"
		<<"\tdepending on which argument is given.\n"
		<<"./jobCommander exit\n"
		<<"\tStops the jobExecutor server.\n"
		<<endl;
}

//Because all bytes in the shared memory are made to be 0, 
//we are guaranteed that all bools will start as false
int createJobExecutorIfNecessaryAndReturnSharedMemory(bool*& bool_arr, int*& int_arr, sem_t*& sema_arr) {
	int retval=-1;
	//If shared memory object doesn't exist, create it
	retval=shm_open("/jobCommander",O_RDWR|O_CREAT|O_EXCL,S_IRWXU|S_IRWXG|S_IRWXO);
	//If there was an error
	if(retval<0) {
		if(errno!=EEXIST) {
			return retval;
		}
		//If the error was that it already exists, just get it
		retval=shm_open("/jobCommander",O_RDWR,S_IRWXU|S_IRWXG|S_IRWXO);
		if(retval<0) {
			return retval;
		}
		//If we got it succesfully, get the memory mappings
		if(getSMSAdress(retval,bool_arr,int_arr,sema_arr)<0) {
			perror("jobCommander: Mapping shared memmory segment");
			shm_unlink("/jobCommander");
			return -1;
		}
		return retval;
	}
	//Else if it was succesfully created, 
	clog<<"DEBUG: Starting executor..."<<endl;
	//resize it
	ftruncate(retval,SMS_SIZE);
	//Get the memmory mappings
	if(getSMSAdress(retval,bool_arr,int_arr,sema_arr)<0) {
		perror("jobCommander: Mapping shared memmory segment");
		shm_unlink("/jobCommander");
		return -1;
	}
	//Create semaphores
	for(int i=0;i<NUM_OF_SEMA;++i) {
		if(sem_init(&(sema_arr[i]),true,1)<0) {
			perror("jobCommander: initialising semaphores");
			munmap(bool_arr,SMS_SIZE);
			shm_unlink("/jobCommander");
			return -1;
		}
	}
	//Double check bools are set correctly, just in case
	bool_arr[EXEC_EXITING_BOOL]=false;
	bool_arr[EXEC_NO_LONGER_RUNNING_BOOL]=false;
	bool_arr[SMS_READY_BOOL]=false;
	//Make a mask of ignored signals (mask is inherited after fork and execv)
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask,SIGUSR1);
	sigaddset(&mask,SIGUSR2);
	sigaddset(&mask,SIGCHLD);
	//Mask now contains SIGUSR and SIGCHILD
	sigprocmask(SIG_SETMASK,&mask,NULL);
	
	//Fork and start the JobExecutor
	pid_t pid=fork();
	if(pid<0) {
		munmap(bool_arr,SMS_SIZE);
		shm_unlink("/jobCommander");
		return pid;
	}
	//If this is the child, start the executor
	if(pid==0) {
		char * const args[2]={"./jobExecutor",NULL};
		execv("./jobExecutor",args);
	}
	//Else this is the parent
	
	atexit(detachSharedMemory);
	return retval;
	
}

int checkIfJobExecutorExistsAndIfYesReturnSharedMemory(bool*& bool_arr, int*& int_arr, sem_t*& sema_arr) {
	int retval=-1;
	//Get shared memory file descriptor
	retval=shm_open("/jobCommander",O_RDWR,S_IRWXU|S_IRWXG|S_IRWXO);
	if(retval<0) {
		//This probably means that the job commander is not running.
		return retval;
	}
	//Get the memmory mappings
	if(getSMSAdress(retval,bool_arr,int_arr,sema_arr)<0) {
		perror("jobCommander: Mapping shared memmory segment");
		shm_unlink("/jobCommander");
		return -1;
	}
	
	atexit(detachSharedMemory);
	return retval;
}