#include "shared_data.hpp"

#include <cstdlib>

#include <sys/mman.h>

bool* bool_arr;

int getSMSAdress(int fd, bool*& bool_arr, int*& int_arr, sem_t*& sema_arr) {
	bool_arr=(bool*)mmap(NULL,SMS_SIZE,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
	if(bool_arr==MAP_FAILED) {
		return -1;
	}
	if(NUM_OF_INT==0) {
		int_arr=NULL;
	}
	else {
		int_arr=(int*)(bool_arr+NUM_OF_BOOL);
	}
	if(NUM_OF_SEMA==0) {
		sema_arr=NULL;
	} 
	else {
		sema_arr=(sem_t*)((int*)(bool_arr+NUM_OF_BOOL)+NUM_OF_INT);
	}
	#ifdef ACCESSING_MEMORY_INT
		#ifdef ACCESSING_INT_MUTEX
			sem_wait(&(sema_arr[ACCESSING_INT_MUTEX]));
		#endif
		
		int_arr[ACCESSING_MEMORY_INT]=int_arr[ACCESSING_MEMORY_INT]+1;
		
		#ifdef ACCESSING_INT_MUTEX
			sem_post(&(sema_arr[ACCESSING_INT_MUTEX]));
		#endif
	#endif
	return 0;
}

void detachSharedMemory() {
	#ifdef ACCESSING_MEMORY_INT
		
		int* int_arr=(int*)(bool_arr+NUM_OF_BOOL);
		
		#ifdef ACCESSING_INT_MUTEX
			
			sem_t* sema_arr=(sem_t*)((int*)(bool_arr+NUM_OF_BOOL)+NUM_OF_INT);
			
			sem_wait(&(sema_arr[ACCESSING_INT_MUTEX]));
			
		#endif
		
		int_arr[ACCESSING_MEMORY_INT]=int_arr[ACCESSING_MEMORY_INT]-1;
		
		if(int_arr[ACCESSING_MEMORY_INT]<=0) {
			munmap(bool_arr,SMS_SIZE);
			shm_unlink("/jobCommander");
		}
		else {
			#ifdef ACCESSING_INT_MUTEX
			
				sem_post(&(sema_arr[ACCESSING_INT_MUTEX]));

			#endif
			munmap(bool_arr,SMS_SIZE);
		}
	#else
		munmap(bool_arr,SMS_SIZE);
		shm_unlink("/jobCommander");
	#endif
}