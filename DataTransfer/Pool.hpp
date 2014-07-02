#ifndef __POOL__INCLUDED__
#define __POOL__INCLUDED__

#include <pthread.h>
#include <semaphore.h>

#include "DataObject.hpp"

/* Returned when the mutex lock timeouts
 */
#define __POOL__TIMEOUT__ 1
/* Returned when the pool is empty
 */
#define __POOL__EMPTY__ 2

/* Thread safe pool
 */
class Pool {
private:
	const int poolSize;
	DataObject** data;
	int start;
	int end;
	int count;
	pthread_mutex_t mtx;
	pthread_cond_t cond_nonempty;
	pthread_cond_t cond_nonfull;
	struct timespec timeout;
public:
	/* Creates a pool with size <size>.
	 */
	Pool(int size,unsigned short timeout_secs);
	/* Stores <toPlace> in the last place of the pool. Blocks until the object is stored. 
	 * Returns 0 for success and -1 for failure
	 */
	int place(DataObject*& toPlace);
	/* Removes the first object from the pool and places it in <store>. Blocks until the object is returned. 
	 * Returns 0 for success, -1 for failure and __POOL__TIMEOUT__ if the time limit for the non-empty signal expires
	 */
	int obtain(DataObject*& store);
	/* Removes the first object from the pool and places it in <store>. Blocks until the object is returned. 
	 * Returns 0 for success, -1 for failure, __POOL__TIMEOUT__ if the time limit for the non-empty signal expires
	 * and __POOL__EMPTY__ if the pool is empty.
	 */
	int obtainIfNotEmpty(DataObject*& store);
	~Pool();
};

#endif // __POOL__INCLUDED__
