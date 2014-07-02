#ifndef __CONNECTIONDATA__INCLUDED__
#define __CONNECTIONDATA__INCLUDED__

#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

class ConnectionData
{
private:
	/* Holds the number of pointers to this connection data object.
	 * When this hits 0, the connection data object will be deleted.
	 */
	int links;
	/* Mutex to ensure only one can access this at the same time.
	 */
	
	~ConnectionData();
public:
	pthread_mutex_t mtx;
	int sock;
	struct sockaddr_in client;
	socklen_t clientlen;

	/* Creates a connection data object and sets links to 1.
	 */
	ConnectionData(int Csock, struct sockaddr_in Cclient, socklen_t Cclientlen);
	/* Atempts to lock the socket mutex.
	 * Returns negative for error.
	 */
	int lock_mutex();
	/* Attempts to lock the socket mutex. Timeouts after <timeout_secs> seconds.
	 * Returns negative for error.
	 */
	int timedLock_mutex(unsigned int timeout_secs);
	/* Atempts to unlock the socket mutex.
	 * Returns negative for error.
	 */
	int unlock_mutex();
	/* Call this when creating a pointer to this object.
	 */
	void linksIncrement();
	/* Call this when removing a pointer to this object.
	 * The object will be deleted when the last link is removed.
	 * Deleting the object closes the socket.
	 */
	void linksDecrement();
};

#endif // __CONNECTIONDATA__INCLUDED__
