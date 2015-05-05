#include <iostream>
#include <iomanip>

#include <cstdlib>
#include <cmath>

#include <mpi.h>

using namespace std;

#define ITER_CHECK 50 //The program will check if we have converged every ITER_CHECK iterations 

#define sqrtN 2000 //The squre root of the number of elements in the grid

//When the total change occuring in the entire array is less than epsilon, the program stops solving
//Because we compare without the square root, the desired epsilon should be squared and written below
#define epsilonSq 1e-10 //Means (10^-5)^2

#define qDef sqrt(nthreads)/4
//#define qDef sqrt(nthreads)/2
//#define qDef sqrt(nthreads)
//#define qDef sqrt(nthreads)*2
//#define qDef sqrt(nthreads)*4

#define omegaRed 0.1
//#define omegaRed 0.2
//...
//#define omegaRed 1.8
//#define omegaRed 1.9

//#define MPI_BOOL MPI_CHAR

#define omegaBlack 0.1
//#define omegaBlack 0.2
//...
//#define omegaBlack 1.8
//#define omegaBlack 1.9

/**
 * @brief Computes the value of elem[i][j] in the next iteration
 * @param omega the relaxation value
 * @param elems the array that holds the elements
 * @param i the position along the x axis (right is positive)
 * @param j the position along the y axis (down is positive)
 * @return the resulting value from the computation
 */
inline double equation(double omega, double** elems, int i, int j);

int main(int argc, char **argv)
{
	MPI_Comm cartComm; //Cartesian communicator
	
	int tid; //ID of the current thread
	
	int nthreads; //Number of threads
	
	double time_initial;
	double time_end;
	
	int verZones; //Threads per row
	int horZones; //Threads per column
	
	int horElements; //Number of horizontal elements in this zone
	int verElements; //Number of vertical elements in this zone
	int horDeficit=0; //How many more horizontal elements this has compared to other zones
	int verDeficit=0; //How mant more vertical elements this has compared to other zones
	int verTotal; //Total number of vertical elements, including edges
	int horTotal; //Total number of vertical edges, including edges
	
	bool isFirstElemRed; //True if the first (upper left) element (NOT edge) of this block is red, false otherwise
	
	int threads[2]; //Threads per dimension
	int wrapAround[2]; //Does the cartesian grid wrap around?
	//No, it doesn't
	wrapAround[0]=0;
	wrapAround[1]=0;
	
	int destL; //Left neighbour
	int destR; //Right neighbour
	int destU; //Up neighbour
	int destD; //Down neighbour
	
	//Send and receive buffers
	double* bufSL;
	double* bufRL;
	double* bufSR;
	double* bufRR;
	double* bufSD;
	double* bufRD;
	double* bufSU;
	double* bufRU;
	
	//Send and receive requests for asynchronous communication. Initializing as null lets us wait on them
	//even when no request has been made (useful for the first iteration).
	MPI_Request reqSL=MPI_REQUEST_NULL;
	MPI_Request reqRL=MPI_REQUEST_NULL;
	MPI_Request reqSU=MPI_REQUEST_NULL;
	MPI_Request reqRU=MPI_REQUEST_NULL;
	MPI_Request reqSD=MPI_REQUEST_NULL;
	MPI_Request reqRD=MPI_REQUEST_NULL;
	MPI_Request reqSR=MPI_REQUEST_NULL;
	MPI_Request reqRR=MPI_REQUEST_NULL;
	
	//Initialize the MPI environment
	if(MPI_Init(NULL,NULL)!=MPI_SUCCESS) {
		cerr<<"ERROR"<<endl;
	}
	
	//Get number of threads
	if(MPI_Comm_size(MPI_COMM_WORLD, &nthreads)!=MPI_SUCCESS) {
		cerr<<"ERROR"<<endl;
	}
	
	//Number of collumns
	verZones = qDef;
	if(verZones==0) {
		verZones=1;
	}
	horZones=nthreads/verZones;
	
	threads[0]=horZones;
	threads[1]=verZones;
	
	//Create two dimensional cartesian grouping that doesn't wrap around
	if(MPI_Cart_create(MPI_COMM_WORLD,2,threads,wrapAround,1,&cartComm)!=MPI_SUCCESS) {
		cerr<<"ERROR"<<endl;
	}
	//Get new number of threads
	if(MPI_Comm_size(cartComm, &nthreads)!=MPI_SUCCESS) {
		cerr<<"ERROR"<<endl;
	}
	//Get new id of thread
	if(MPI_Comm_rank(cartComm, &tid)!=MPI_SUCCESS) {
		cerr<<"ERROR"<<endl;
	}
	//Get neighbours
	MPI_Cart_shift(cartComm,0,1,&destL,&destR);
	MPI_Cart_shift(cartComm,1,1,&destU,&destD);
	
	//Find out how many elements this zone should have
	horElements = sqrtN/horZones;
	verElements = sqrtN/verZones;
	
	//If this is in the right/bottom edge (which we can tell since we have no right/bottom neighbour)
	//Find how many more elements we need and add them to the number of elements of this zone
	if(destR==MPI_PROC_NULL) {
		horDeficit=sqrtN%horZones;
		horElements+=horDeficit;
	}
	if(destD==MPI_PROC_NULL) {
		verDeficit=sqrtN%verZones;
		verElements+=verDeficit;
	}
	
	verTotal=verElements+2;
	horTotal=horElements+2;
	
	//Allocate the local grid plus the necessary border buffers
	//and initialize everything to zero
	double ** elems = new double*[horTotal];
	for(int i=1;i<horTotal;++i) {
		elems[i]=new double[verTotal](); //() means initialize to 0
	}
	
	//If this is on the right edge initialise the right edge to 1
	if(destR==MPI_PROC_NULL) {
		for(int i=0;i<verTotal;++i) {
			elems[horTotal-1][i]=1.0;
		}
	}
	
	//Allocate communication buffers if necessary
	if(destR!=MPI_PROC_NULL) {
		bufSR=new double[verTotal](); //() means initialize to 0
		bufRR=new double[verTotal](); //() means initialize to 0
	}
	if(destL!=MPI_PROC_NULL) {
		bufSL=new double[verTotal](); //() means initialize to 0
		bufRL=new double[verTotal](); //() means initialize to 0
	}
	if(destU!=MPI_PROC_NULL) {
		bufSU=new double[horTotal](); //() means initialize to 0
		bufRU=new double[horTotal](); //() means initialize to 0
	}
	if(destD!=MPI_PROC_NULL) {
		bufSD=new double[horTotal](); //() means initialize to 0
		bufRD=new double[horTotal](); //() means initialize to 0
	}
	
	//Compute if the upper left element (element, NOT edge) of this block is red or black
	//then use that to tell the next block
	//First we receive data from the previous block, if it exists
	if(destU!=MPI_PROC_NULL) {
		MPI_Recv(&isFirstElemRed,1,MPI_BOOL,destU,MPI_ANY_TAG,cartComm,MPI_STATUS_IGNORE);
	}
	if(destL!=MPI_PROC_NULL) {
		MPI_Recv(&isFirstElemRed,1,MPI_BOOL,destL,MPI_ANY_TAG,cartComm,MPI_STATUS_IGNORE);
	}
	//If the previous block doesn't exist, then we are at the upper left block and we
	//know the upper left block starts red
	if(destU==MPI_PROC_NULL && destL==MPI_PROC_NULL) {
		isFirstElemRed=true;
	}
	//Send data to the next block if it exists
	if(destD!=MPI_PROC_NULL) {
		//verElements%2==0 when the first element is red and the next block's first element is also red
		//So if isFirstElemRed==true and (verElements%2==0)==true then isNextFirstElemRed==true
		//if isFirstElemRed==true and (verElements%2==0)==false then isNextFirstElemRed==false
		//and so on
		bool isNextFirstElemRed=(((verElements%2)==0)==isFirstElemRed);
		MPI_Send(&isNextFirstElemRed,1,MPI_BOOL,destD,MPI_ANY_TAG,cartComm);
	}
	if(destR!=MPI_PROC_NULL) {
		bool isNextFirstElemRed=(((horElements%2)==0)==isFirstElemRed);
		MPI_Send(&isNextFirstElemRed,1,MPI_BOOL,destR,MPI_ANY_TAG,cartComm);
	}
	
	//Get start time
	time_initial = MPI_Wtime();
	
	//Start solving, keep looping until the difference becomes small enough
	//Start from the left collumn (position 1) and start going down by two. 
	//Anything computed gets stored in a temp var, where it is used to be copied into the send buffer 
	//When done, send/rcv, but do not actually check if we got the data until it is needed 
	//(ie, until we reach this part of the loop again)
	//Remember, omega1 for red, omega2 for black
	
	unsigned int iteration=0;
	
	do {
		++iteration;
		double diffSum; //Holds the sum for the end condition
		bool checkIter=((iteration%ITER_CHECK)==0); //Tells us if we should check for convergance in this iteration
		if(checkIter) {
			diffSum=0.0;
		}
		
		//wait for the data from the appropriate receive buffer each time the looop starts 
		//and copy them to the edge. Then send a request for the next batch of edges 
		//(that will complete on the next loop)
		if(destR!=MPI_PROC_NULL) {
			MPI_Wait(&reqRR,MPI_STATUS_IGNORE);
			for(int j=1;j<verTotal-2;++j) {
				elems[horTotal-1][j]=bufRR[j];
			}
			MPI_Irecv(bufRR,verElements,MPI_DOUBLE,destR,MPI_ANY_TAG,cartComm,&reqRR);
		}
		if(destL!=MPI_PROC_NULL) {
			MPI_Wait(&reqRL,MPI_STATUS_IGNORE);
			for(int j=1;j<verTotal-2;++j) {
				elems[0][j]=bufRL[j];
			}
			MPI_Irecv(bufRL,verElements,MPI_DOUBLE,destL,MPI_ANY_TAG,cartComm,&reqRL);
		}
		if(destU!=MPI_PROC_NULL) {
			MPI_Wait(&reqRU,MPI_STATUS_IGNORE);
			for(int i=1;i<horTotal-2;++i) {
				elems[i][0]=bufRU[i];
			}
			MPI_Irecv(bufRU,horElements,MPI_DOUBLE,destU,MPI_ANY_TAG,cartComm,&reqRU);
		}
		if(destD!=MPI_PROC_NULL) {
			MPI_Wait(&reqRD,MPI_STATUS_IGNORE);
			for(int i=1;i<horTotal-2;++i) {
				elems[i][verTotal-1]=bufRD[i];
			}
			MPI_Irecv(bufRD,horElements,MPI_DOUBLE,destD,MPI_ANY_TAG,cartComm,&reqRD);
		}
		
		//Compute all red
		for(int i=1;i<horTotal-1;++i) {
			for(int j=(isFirstElemRed)?(i%2):((i%2)+1);j<verTotal-1;j+=2) {
				double newVal=equation(omegaRed,elems,i,j);
				if(checkIter) {
					diffSum=diffSum+((newVal-elems[i][j])*(newVal-elems[i][j]));
				}
				elems[i][j]=newVal;
			}
		}
		//Do the same as above but for black
		for(int i=1;i<horTotal-1;++i) {
			for(int j=(isFirstElemRed)?((i%2)+1):(i%2);j<verTotal-1;j+=2) {
				double newVal=equation(omegaBlack,elems,i,j);
				if(checkIter) {
					diffSum=diffSum+((newVal-elems[i][j])*(newVal-elems[i][j]));
				}
				elems[i][j]=newVal;
			}
		}
		
		//Finally, wait on the send buffers, copy new data there and 
		//make the buffers availiable to the other processes 
		if(destR!=MPI_PROC_NULL) {
			MPI_Wait(&reqSR,MPI_STATUS_IGNORE);
			for(int j=1;j<verTotal-2;++j) {
				bufSR[j]=elems[horTotal-1][j];
			}
			MPI_Isend(bufSR,verElements,MPI_DOUBLE,destR,MPI_ANY_TAG,cartComm,&reqSR);
		}
		if(destL!=MPI_PROC_NULL) {
			MPI_Wait(&reqSL,MPI_STATUS_IGNORE);
			for(int j=1;j<verTotal-2;++j) {
				bufSL[j]=elems[0][j];
			}
			MPI_Isend(bufSL,verElements,MPI_DOUBLE,destL,MPI_ANY_TAG,cartComm,&reqSL);
		}
		if(destU!=MPI_PROC_NULL) {
			MPI_Wait(&reqSU,MPI_STATUS_IGNORE);
			for(int i=1;i<horTotal-2;++i) {
				bufSU[i]=elems[i][0];
			}
			MPI_Irecv(bufSU,horElements,MPI_DOUBLE,destU,MPI_ANY_TAG,cartComm,&reqSU);
		}
		if(destD!=MPI_PROC_NULL) {
			MPI_Wait(&reqSD,MPI_STATUS_IGNORE);
			for(int i=1;i<horTotal-2;++i) {
				bufSD[i]=elems[i][verTotal-1];
			}
			MPI_Isend(bufSD,horElements,MPI_DOUBLE,destD,MPI_ANY_TAG,cartComm,&reqSD);
		}
		
		//Each ITER_CHECK times, check for the sum and exit if we have reached the required level of convergence
		if(checkIter) {
			if(diffSum<epsilonSq) {
				break;
			}
		}
	} while(true);
	
	time_end = MPI_Wtime();
	
	if(tid==0) {
		cout<<fixed<<setprecision(20)<<(time_end-time_initial)<<endl;
		//cerr<<fixed<<setprecision(20)<<(time_end-time_initial)<<endl;
	}
	
	//Finalize the MPI environment
	if(MPI_Finalize()!=MPI_SUCCESS) {
		cerr<<tid<<" ERROR"<<endl;
	}
	
	//Exit
	return EXIT_SUCCESS;
}


inline double equation(double omega, double** elems, int i, int j) {
	return (1.0-omega)*(elems[i][j])+(omega/4.0)*(elems[i-1][j]+elems[i][j-1]+elems[i][j+1]+elems[i+1][j]);
}