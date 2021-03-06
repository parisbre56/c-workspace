#ifndef __EXTERNDATA__INCLUDED__
#define __EXTERNDATA__INCLUDED__

#include "Pool.hpp"
#include "ThreadList.hpp"
#include <pthread.h>

//Holds the definition of shared extern variables

//Keeps all the data that must be retrieved. Accesed atomically.
extern Pool* dataPool;
//True if the server is exiting
extern bool exitCond;
//List of all active threads except for main. Accessed atomically.
extern ThreadList threadList;
extern ThreadList readerThreadList;
//Used for concurrent writing
extern pthread_mutex_t wMtx;
//Number of worker threads
extern int thread_pool_size;

#endif //__EXTERNDATA__INCLUDED__