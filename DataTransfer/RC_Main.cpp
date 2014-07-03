/* Communication protocol is as follows:
 * 
 * 	Server listens, client initiates connection
 * 	Server creates new reader thread. Reader locks socket mutex.
 * 	Client sends length of string (size_t) or 0 for error
 * 	Client sends string (char array with the above length)
 * 		Client sends only the special character '\0' (null) if it wants the server to terminate
 * 	Client waits for response (int)
 * 	Server reads the data and checks if it's OK.
 * 		If data not OK, Server sends negative or 0 response indicating an error (int), an error string and closes the connection.
 * 		The negative response indicates the size of the string containing the error description.
 * 	If data OK, the server sends positive response which is PAGESIZE (int).
 * 	Client now waits for status indicator (int). 
 * 		If Client receives 1, then it knows there are no more data and terminates succesfully.
 * 		If Client receives 0, then it knows there are more data coming and gets ready to process a new file
 * 		If client receives negative, then it knows there is an error and it waits for an error string with a size of -status.
 * 	Server gathers data for the pool. If there is a problem, sends negative to Client.
 * 	Server puts data in pool. Reader unlocks socket mutex.
 * 	Reader thread exits now.
 * 	Writer thread takes data from the pool and prepares to send it. Locks socket mutex. If all OK, sends 0 to indicate data incoming. Else sends -1 to indicate error.
 * 	Server sends file string length (size_t) or 0 for error
 * 	Server sends file string (char array with the above length)
 * 	Client starts reading data segments for this file and writing them to socket until it receives 0 or negative.
 * 	While there are more data in this file to transmit
 * 		Server sends positive dataSize (int) or negative for error.
 * 		Server sends data (void* data of the above dataSize)
 * 	When there are no more data in this file to transmit
 * 		Server sends 0 (int). Writer unlocks mutex.
 * 	Writer reads from pool again while Client waits for response again
 */

#include <iostream>

#include <cstdlib>

/* Prints help about the command line arguments for this program in stderr
 */
void printUsage();

/* Check the arguements and puts them in the appropriate variable.
 * Returns -1 for error, 0 for successs.
 */
 
 //Change thisw!!!!
int getArgs(int argc, char **argv, int& port, int& thread_pool_size, int& queue_size);

int main(int argc, char **argv)
{
	if(argc!=2) {
		cerr<<"ERROR: Need exactly one argument."<<endl;
		exit(EXIT_FAILURE);
	}
	
	exit(EXIT_SUCCESS);
}
