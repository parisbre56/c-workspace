#include <cstdlib>

#include <unistd.h>
#include <sys/mman.h>

#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
	if(argc>1) {
		if(argv[1][0]=='u') {
			shm_unlink("/jobCommander");
		}
		unsigned int secs=atoi(argv[1]);
		cout<<"Process "<<getpid()<<" sleeping for "<<secs<<endl;
		sleep(secs);
		cout<<"Process "<<getpid()<<" slept for "<<secs<<endl;
	}
	else {
		cout<<"No args given. This process waits for no one."<<endl;
	}
	
	exit(EXIT_SUCCESS);
}
