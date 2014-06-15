#include <iostream>
#include <cstdlib>
#include <string>


#include <sys/types.h>
#include <sys/wait.h>
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

/* Just so that I don't have to copypaste the same code everywhere,
 * this one checks concurrency and the number of running jobs and
 * starts executing the next one if neccessary.
 */
inline void executeNextJobIfNecessary(JobList& jobRunning, JobList& jobQueued, int& concurrency);

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
	if(unlink("comm_pipe.temp")<0) {
		if(errno!=ENOENT) {
			cerr<<"ERROR: Unable to delete old named pipe."<<endl;
			perror("jobExecutor");
			exit(EXIT_FAILURE);
		}
	}
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
	sigaddset(&mask,SIGUSR2);
	sigaddset(&mask,SIGCHLD);
	
	//Notify the shared memory that you are ready
	bool_arr[EXEC_EXITING_BOOL]=false;
	bool_arr[EXEC_NO_LONGER_RUNNING_BOOL]=false;
	bool_arr[SMS_READY_BOOL]=true;
	
	//Start looping, waiting for signals and handling them
	//Exit condition is EXEC_EXITING_BOOL is true and there are no jobs left in the queue
	while(!(bool_arr[EXEC_EXITING_BOOL]&&jobAll.inList==0)) {
		//Wait for signal
		if(sigwaitinfo(&mask,&signalInfo)<0) {
			if(errno==EINTR) {
				cerr<<"WARNING: Interrupted while waiting for signal by unhandled signal."<<endl;
				continue;
			}
			cerr<<"ERROR: Error while waiting for signal."<<endl;
			perror("jobExecutor");
			exit(EXIT_FAILURE);
		}
		//Check signal type and handle it (either child or read from pipe signal)
		if(signalInfo.si_signo==SIGCHLD) {
			//Remove job from lists
			tempJob=jobAll.popJobWithPID(signalInfo.si_pid);
			if(tempJob==NULL) {
				cerr<<"ERROR: Received a signal from a child that is not in the job list."<<endl;
				clog<<"DEBUG: PID: "<<signalInfo.si_pid<<endl;
				waitpid(signalInfo.si_pid,NULL,WUNTRACED);
				continue;
			}
			if((tempJob=jobRunning.popJobWithPID(signalInfo.si_pid))==NULL) {
				cerr<<"ERROR: Received a signal from a child that is not in the running job list."<<endl;
				clog<<"DEBUG: PID: "<<signalInfo.si_pid<<endl;
				waitpid(signalInfo.si_pid,NULL,WUNTRACED);
				continue;
			}
			waitpid(signalInfo.si_pid,NULL,WUNTRACED);
			delete tempJob;
			
			//Start executing the next job if necessary.
			executeNextJobIfNecessary(jobRunning,jobQueued,concurrency);
			
			continue;
		}
		//If this is a signal to indicate the server should read
		else if(signalInfo.si_signo==SIGUSR1) {
			//Read the order
			if(read(pipe_fd,&exec_int,sizeof(int))<0) {
				cerr<<"ERROR: Unable to read from pipe."<<endl;
				perror("jobExecutor");
				exit(EXIT_FAILURE);
			}
			rString=new char[exec_int];
			read(pipe_fd,rString,exec_int);
			exec_string.assign(rString,exec_int);
			delete[] rString;
			//Respond to the order acdcordingly
			if(!exec_string.compare("issuejob")) {
				/* Communication protocol is as follows (no newlines written):
				 * 	<order_size(int)><order_name(char array of size <order_size>)>
				 * 	<argument_number(int)>
				 * 	<first_argument_size(int)><first_argument(char array of size <first_argument_size>)>
				 * 		...
				 * 	<<argument_number>_argument_size(int)>
				 * 	<<argument_number>_argument(char array of size <<argument_number>_argument_size>)>
				 * Communication ends
				 */
				//Get number of arguements
				read(pipe_fd,&exec_int,sizeof(int));
				char** tempArgv= new char*[exec_int+1]; //+1 for the NULL
				tempArgv[exec_int]=NULL;
				
				//Read all arguements
				for(int i=0,l=exec_int;i<l;++i) {
					read(pipe_fd,&exec_int,sizeof(int));
					tempArgv[i]=new char[exec_int+1]; //+1 for the '\0'
					read(pipe_fd,tempArgv[i],exec_int);
					tempArgv[i][exec_int]='\0';
				}
				exec_string.assign(tempArgv[0]);
				
				//Close pipe
				close(pipe_fd);
				
				//Create job
				tempJob=new Job(freeID,-1,exec_string,false,tempArgv);
				++freeID;
				
				if(!bool_arr[EXEC_EXITING_BOOL]) {
					//Put job in the appropriate queues
					jobAll.insertAtStart(tempJob);
					jobQueued.insertAtStart(tempJob);
				
					//Start running the job if necessary
					executeNextJobIfNecessary(jobRunning,jobQueued,concurrency);
				}
					
				/* Communication protocol is as follows (no newlines written):
				 * 	<returned_id(int)> less than 0 for error.
				 * 	if there was no error:
				 * 	<job_name_size(int)>
				 * 	<job_name(char array of size <job_name_size>)>
				 * 	<running(bool)>
				 * Communication ends
				 */
				//Open the pipe for writing (other has opened with block)
				if((pipe_fd=open("comm_pipe.temp",O_WRONLY))<0) {
					cerr<<"ERROR: Unable to open pipe for writing."<<endl;
					perror("jobExecutor");
					exit(EXIT_FAILURE);
				}
				if(bool_arr[EXEC_EXITING_BOOL]) {
					exec_int=-1;
					write(pipe_fd,&exec_int,sizeof(int));
				}
				else {
					write(pipe_fd,&(tempJob->jobID),sizeof(int));//ID
					exec_int=(tempJob->name).length();
					write(pipe_fd,&exec_int,sizeof(int));//name size
					write(pipe_fd,(tempJob->name).c_str(),exec_int);//name
					rBool=tempJob->running;
					write(pipe_fd,&rBool,sizeof(bool));//running
				}
			}
			else if(!exec_string.compare("setConcurrency")) {
				/* Communication protocol is as follows (no newlines written):
				 * 	<order_size(int)><order_name(char array of size <order_size>)>
				 * 	<concurrency(int)>
				 * Communication ends
				 */
				read(pipe_fd,&concurrency,sizeof(int));
				
				//Close the pipe
				close(pipe_fd);
				 
				//Execute jobs if necessary as the new concurrency dictates
				executeNextJobIfNecessary(jobRunning,jobQueued,concurrency);
				 
				/* Communication protocol is as follows (no newlines written):
				 * 	<returned_concurrency(int)> less than 0 for error.
				 * Communication ends
				 */ 
				 
				//Open pipe for writing (other opens with block)
				if((pipe_fd=open("comm_pipe.temp",O_WRONLY))<0) {
					cerr<<"ERROR: Unable to open pipe for writing."<<endl;
					perror("jobExecutor");
					exit(EXIT_FAILURE);
				}
				//Write back concurrency to acknowledge it was received
				write(pipe_fd,&concurrency,sizeof(int));
			}
			else if(!exec_string.compare("stop")) {
				/* Communication protocol is as follows (no newlines written):
				 * 	<order_size(int)><order_name(char array of size <order_size>)>
				 * 	<jobID(int)>
				 * Communication ends
				 */
				read(pipe_fd,&exec_int,sizeof(int));//jobID
				
				close(pipe_fd);
				
				//Open pipe for writing (other opens with block)
				if((pipe_fd=open("comm_pipe.temp",O_WRONLY))<0) {
					cerr<<"ERROR: Unable to open pipe for writing."<<endl;
					perror("jobExecutor");
					exit(EXIT_FAILURE);
				}

				/* Communication protocol is as follows (no newlines written):
				 * 	<returned_state(int)> less than 0 for error.
				 * 	0 means it doesn't exist
				 * 	1 means it was stopped while in queue
				 * 	2 means it was stopped during execution
				 * Communication ends
				 */
				if((tempJob=jobAll.popJobWithID(exec_int))==NULL) {
					exec_int=0;
					write(pipe_fd,&exec_int,sizeof(int));
				}
				else {
					if(tempJob->running) {
						//Put it back in so that when this receives sigchld
						//it will recognize it
						jobAll.insertAtStart(tempJob);
						if(kill(tempJob->PID,SIGKILL)<0) {
							cerr<<"ERROR: Error while terminating process "<<tempJob->PID<<endl;
							perror("jobExecutor");
							//Inform the commander that there was a problem
							exec_int=-1;
						}
						else {
							//Inform the commander that it was succesfully given
							//the kill signal. This will make it terminate no matter
							//what
							exec_int=2;
						}
						write(pipe_fd,&exec_int,sizeof(int));
					}
					//If it is not running
					else {
						//Also pop it from the queue to prevent it from
						//being excuted and then delete to prevent memory loss
						jobQueued.popJobWithID(exec_int);
						delete tempJob;
						//Inform the commander that it was removed from queue
						exec_int=1;
						write(pipe_fd,&exec_int,sizeof(int));
					}
				}
			}
			else if(!exec_string.compare("poll")) {
				/* Communication protocol is as follows (no newlines written):
				 * 	<order_size(int)><order_name(char array of size <order_size>)>
				 * 	<arg_size(int)><arg_name(char array of size <arg_size>)>
				 * Communication ends
				 */
				read(pipe_fd,&exec_int,sizeof(int));//arg size
				rString=new char[exec_int];
				read(pipe_fd,rString,exec_int);//arg
				exec_string.assign(rString,exec_int);
				delete[] rString;
				
				close(pipe_fd);
				
				//Open pipe for writing (other opens with block)
				if((pipe_fd=open("comm_pipe.temp",O_WRONLY))<0) {
					cerr<<"ERROR: Unable to open pipe for writing."<<endl;
					perror("jobExecutor");
					exit(EXIT_FAILURE);
				}
				
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
				
				JobListItem* iter;
				if(exec_string.compare("running")&&exec_string.compare("queued")) {
					exec_int=-1;
					write(pipe_fd,&exec_int,sizeof(int));
				}
				else {
					if(!exec_string.compare("running")) {
						exec_int=jobRunning.inList;
						iter=jobRunning.start;
					}
					else if(!exec_string.compare("queued")) {
						exec_int=jobQueued.inList;
						iter=jobQueued.start;
					}
					write(pipe_fd,&exec_int,sizeof(int)); //argument number
					//For all jobs
					for(int i=0,l=exec_int;i<l;++i,iter=iter->nextJob) {
						write(pipe_fd,&(iter->job->jobID),sizeof(int)); //id
						exec_int=(iter->job->name).length();
						write(pipe_fd,&exec_int,sizeof(int)); //name size
						write(pipe_fd,(iter->job->name).c_str(),exec_int); //name
					}
				}
			}
			else if(!exec_string.compare("exit")) {
				/* Communication protocol is as follows (no newlines written):
				 * 	<order_size(int)><order_name(char array of size <order_size>)>
				 * Communication ends
				 */
				//Nothing else to read
				close(pipe_fd);
				
				//Set exiting status to true
				bool_arr[EXEC_EXITING_BOOL]=true;
				
				//Open pipe for writing (other opens with block)
				if((pipe_fd=open("comm_pipe.temp",O_WRONLY))<0) {
					cerr<<"ERROR: Unable to open pipe for writing."<<endl;
					perror("jobExecutor");
					exit(EXIT_FAILURE);
				}
				
				/* Communication protocol is as follows (no newlines written):
				 * 	<response(int)> less than 0 for error.
				 * Communication ends
				 */
				//Send response to acknowledge order
				exec_int=0;
				write(pipe_fd,&exec_int,sizeof(int));
			}
			else {
				cerr<<"ERROR: Received unknown order: "<<exec_string<<endl;
				exit(EXIT_FAILURE);
			}
			//Close the output pipe
			if(close(pipe_fd)<0) {
				cerr<<"ERROR: Unable to close pipe"<<endl;
				perror("jobExecutor");
				exit(EXIT_FAILURE);
			}
			//Open the pipe for reading again
			if((pipe_fd=open("comm_pipe.temp",O_RDONLY|O_NONBLOCK))<0) {
				cerr<<"ERROR: Unable to open pipe for reading."<<endl;
				perror("jobExecutor");
				exit(EXIT_FAILURE);
			}
			//Let others communicate
			sem_post(&(sema_arr[EXEC_ACCESS_SEMA]));
			
			continue;
		}
		else {
			cerr<<"ERROR: Received unknown signal."<<endl;
			continue;
		}
	}
	//Once the exit command has been given, destroy any data left and exit
	
	//Remove named pipe
	//The pipe will remain open until the last proccess closes it
	unlink("comm_pipe.temp");
	
	clog<<"DEBUG: Exited normally."<<endl;
	exit(EXIT_SUCCESS);
}

void destroyJobAll() {
	jobAll.deleteAll();
}

void setMemoryToIndicateExit() {
	bool_arr[SMS_READY_BOOL]=true;
	bool_arr[EXEC_NO_LONGER_RUNNING_BOOL]=true;
	bool_arr[EXEC_EXITING_BOOL]=true;
}

inline void executeNextJobIfNecessary(JobList& jobRunning, JobList& jobQueued, int& concurrency) {
	//Keep forking until we either reach the limit or run out of jobs
	while(jobRunning.inList<concurrency&&jobQueued.inList>0) {
		Job* tempJob=jobQueued.removeFromEnd();
		pid_t pid=fork();
		if(pid<0) {
			cerr<<"ERROR: Unable to fork."<<endl;
			perror("jobExecuor");
			exit(EXIT_FAILURE);
		}
		//If this is the child
		if(pid==0) {
			//Remove the signal mask 
			//(because it carries over when we do execv and we don't want that)
			sigset_t mask;
			sigemptyset(&mask);
			sigprocmask(SIG_SETMASK,&mask,NULL);
			execv((tempJob->name).c_str(),tempJob->argv);
		}
		//Else if this is the father
		tempJob->PID=pid;
		tempJob->running=true;
		jobRunning.insertAtStart(tempJob);
		clog<<"DEBUG: Starting job: "<<tempJob->name<<" PID: "<<pid<<endl;
	}
}