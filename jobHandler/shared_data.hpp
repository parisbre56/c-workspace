#ifndef __SHARED_DATA_HPP__
#define __SHARED_DATA_HPP__

//NOTE: THIS INCLUDES THE GLOBAL VARIABLE bool_arr

#include <semaphore.h>

//Number of seconds to wait until the process times out
#define TIMEOUT_SECS 100

//Number of variables in shared memory
#define NUM_OF_BOOL 3
#define NUM_OF_INT 2
#define NUM_OF_SEMA 1

//Define bools here

//The shared memory is ready to be used
#define SMS_READY_BOOL 0
//The executor is exiting
#define EXEC_EXITING_BOOL 1
//The executor is no longer running
#define EXEC_NO_LONGER_RUNNING_BOOL 2

//Define ints here
#define EXEC_PID_INT 0
#define READER_PID_INT 1

//Define semas here
#define EXEC_ACCESS_SEMA 0

#define SMS_SIZE sizeof(bool)*NUM_OF_BOOL+sizeof(int)*NUM_OF_INT+sizeof(sem_t)*NUM_OF_SEMA

/* Gets the shared memory adresses according to defines. fd is the file descriptor
 * of the shared memory adress.
 */
int getSMSAdress(int fd, bool*& bool_arr, int*& int_arr, sem_t*& sema_arr);

/* Detaches the shared memory. Should be called atexit.
 */
void detachSharedMemory();

//Used to store the SMS Adress.
extern bool* bool_arr;

#endif