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
	return 0;
}

void detachSharedMemory() {
	munmap(bool_arr,SMS_SIZE);
	shm_unlink("/jobCommander");
}