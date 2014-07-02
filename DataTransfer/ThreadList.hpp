#ifndef __THREADLIST__INCLUDED__
#define __THREADLIST__INCLUDED__

#include <pthread.h>

class ThreadListObject {
public:
	pthread_t * thread;
	ThreadListObject * next;
	/* Constructs a node that contains <thread> and points to NULL
	 */
	ThreadListObject(pthread_t * cThread);
};

/* Thread safe list of threads.
 */
class ThreadList {
private:
	pthread_mutex_t mtx;
	ThreadListObject * root;
public:
	ThreadList();
	/* Inserts data in the beginning of the list.
	 */
	void push(pthread_t * data);
	/* Removes a thread from the beginning of the list and returns it.
	 * Returns NULL for empty.
	 */
	pthread_t * pop();
	/* Atempts to remove the first occurence of <data> in the list
	 * if it exists.
	 */
	void remove(pthread_t data);
	/* Deletes all ThreadListObjects and thread_t objects in the list.
	 */
	~ThreadList();
};

#endif // __THREADLIST__INCLUDED__
