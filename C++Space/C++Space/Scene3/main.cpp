/* TODO:
 * maybe add position prompt for next frame and a reprint command
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <climits>
#include "lake.hpp"

using namespace std;

int main(int argc, char **argv)
{
	unsigned int i;
	if (argc!=5)
	{
		cerr<<"ERROR: Need exactly 4 arguements."<<endl;
		return EXIT_FAILURE;
	}
	try
	{
		srand(time(NULL));
		
		unsigned int M = atoi(argv[1]);
		unsigned int L = atoi(argv[2]);
		if(L<2||M<2)
		{
			cerr<<"ERROR: Lake's side size must be greater than 1."<<endl;
			return EXIT_FAILURE;
		}
		if(L>=UINT_MAX-1||M>=UINT_MAX-1)
		{
			cerr<<"ERROR: Lake's side size exceeded UINT_MAX-2. Use a smaller number."<<endl;
			return EXIT_FAILURE;
		}
		unsigned int N = atoi(argv[3]);
		unsigned int T = atoi(argv[4]);
		if((L+1)*(M+1)<N)
		{
			cerr<<"ERROR: More IceSkaters than positions."<<endl;
			return EXIT_FAILURE;
		}
		Lake lake(M,L,N);
		
		//goto end;
		if(T==UINT_MAX)
		{
			cerr<<"WARNING: Simulation will continue until EOF (Ctrl+D) is given."<<endl;
		}
		for (i=1;i<=T;++i)
		{
			lake.output();
			cout<<"Frame:"<<i-1<<" Give a newline (ENTER) for the next frame."<<endl;
			while(cin.get()!='\n'&&!cin.fail())
			{
				
			}
			if(cin.fail())
			{
				cerr<<"WARNING: Standard input failbit set. Terminating..."<<endl;
				break;
			}
			lake.advance();
		}
		if (i>T)
		{
			lake.output();
			cout<<"Frame:"<<i-1<<" Simulation ended normally after time limit expired."<<endl;
		}
		else if (cin.eof())
		{
			cout<<"Simulation was aborted by user."<<endl;
		}
		else
		{
			cout<<"Simulation was terminated by abnormal input."<<endl;
		}
		
	}
	catch(const exception &e)
	{
		cerr<<"ERROR: Unhandled Exception Caught: "<<e.what()<<endl;
		return EXIT_FAILURE;
	}
	catch(...)
	{
		cerr<<"ERROR: Unknown Exception Caught. Check your code."<<endl;
		return EXIT_FAILURE;
	}
	//end:
	return EXIT_SUCCESS;
}
