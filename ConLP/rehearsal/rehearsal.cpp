#include <naxos.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "strtok.h"

using namespace std;
using namespace naxos;



	int
main (int argc, char *argv[])
{
    try  {

	//  Checking input arguments.

	if ( argc  !=  2 )   {

		cerr << "Correct syntax is:\t" << argv[0]
			<< " <input_filename>\n";
		return  1;
	}


	//  Opening input filename, e.g. `rehearsal_data.pl'.

	ifstream  file(argv[1]);

	if ( ! file )   {

		cerr << "Could not open `" << argv[1] << "'\n";
		return  1;
	}


	string  input, tok;

	NsDeque< NsDeque<NsInt> >  musicians;
	NsDeque<NsInt>             durations;


	//  Reading input file line by line.

	while ( getline(file,input) )    {

		StrTokenizer  tokens(input, "([,]).");

		tok  =  tokens.next();

		if ( tok == "musicians"  ||  tok == "           " )    {

			musicians.push_back( NsDeque<NsInt>() );

			while ( ! (tok = tokens.next()).empty() )
				musicians.back().push_back(atoi(tok.c_str()));

		}  else if ( tok  ==  "durations" )    {

			while ( ! (tok = tokens.next()).empty() )
				durations.push_back(atoi(tok.c_str()));

		}  else if ( tok  !=  "" )    {

			cerr << "Unknown predicate `" << tok
				<< "'\n";
			return  1;
		}
	}

	file.close();



	/**   ADD YOUR CODE HERE   **/



    }  catch (exception& exc)  {

	cerr << exc.what() << "\n";

    }  catch (...)  {

	cerr << "Unknown exception" << "\n";
    }
}
