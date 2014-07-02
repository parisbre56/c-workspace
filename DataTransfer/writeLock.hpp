#ifndef __WRITELOCK__INCLUDED__
#define __WRITELOCK__INCLUDED__

#define __WRITELOCK__TIMEOUTSECS__ 5

/* Tries to acquire the mutex for the output stream
 */
void writeTimedLock();

/* Tries to release the mutex for the output stream
 */
void writeTimedUnlock();

#endif //__WRITELOCK__INCLUDED__