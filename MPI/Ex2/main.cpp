#include <iostream>
#include <iomanip>
#include <chrono>

#include <cstdlib>
#include <cmath>

#ifndef SINGLE_PROC
	#include <mpi.h>
#endif

//#define CUDA_ENABLED

#ifdef CUDA_ENABLED
	#include <cuda.h>
#endif

using namespace std;
using namespace chrono;

#define ITER_CHECK 50 //The program will check if we have converged every ITER_CHECK iterations 

//#define sqrtN 50 //The squre root of the number of elements in the grid

//When the total change occuring in the entire array is less than epsilon, the program stops solving
#define epsilon 1e-5

//#define qDef sqrt(nthreads)/4
//#define qDef sqrt(nthreads)/2
//#define qDef sqrt(nthreads)
//#define qDef sqrt(nthreads)*2
//#define qDef sqrt(nthreads)*4

//#define omegaRed 0.1
//#define omegaRed 0.2
//...
//#define omegaRed 1.8
//#define omegaRed 1.9

//#define MPI::BOOL MPI_CHAR

//#define omegaBlack 0.1
//#define omegaBlack 0.2
//...
//#define omegaBlack 1.8
//#define omegaBlack 1.9

//#define SINGLE_PROC //Used to testing with a single process

#define INIT_TAG 1 //Used to send initialisation data
#define BUFF_TAG 2 //Used to send data from block to block

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
	#ifndef SINGLE_PROC
		MPI::Cartcomm cartComm; //Cartesian communicator
	#endif
	
	int tid; //ID of the current thread
	
	int nthreads; //Number of threads
	
	#ifndef SINGLE_PROC
		double time_initial;
		double time_end;
	#else
		high_resolution_clock::time_point time_initial;
		high_resolution_clock::time_point time_end;
	#endif
	
	int verZones; //Threads per row
	int horZones; //Threads per column
	
	int horElements; //Number of horizontal elements in this zone
	int verElements; //Number of vertical elements in this zone
	int horDeficit=0; //How many more horizontal elements this has compared to other zones
	int verDeficit=0; //How mant more vertical elements this has compared to other zones
	int verTotal; //Total number of vertical elements, including edges
	int horTotal; //Total number of vertical edges, including edges
	
	bool isFirstElemRed; //True if the first (upper left) element (NOT edge) of this block is red, false otherwise
	
	#ifndef SINGLE_PROC
		int threads[2]; //Threads per dimension
		bool wrapAround[2]; //Does the cartesian grid wrap around?
		//No, it doesn't
		wrapAround[0]=false;
		wrapAround[1]=false;

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
		MPI::Request reqSL=MPI::REQUEST_NULL;
		MPI::Request reqRL=MPI::REQUEST_NULL;
		MPI::Request reqSU=MPI::REQUEST_NULL;
		MPI::Request reqRU=MPI::REQUEST_NULL;
		MPI::Request reqSD=MPI::REQUEST_NULL;
		MPI::Request reqRD=MPI::REQUEST_NULL;
		MPI::Request reqSR=MPI::REQUEST_NULL;
		MPI::Request reqRR=MPI::REQUEST_NULL;
	
	//Used for the asynchrous sum allreduce
		MPI_Request reqSum;
	#endif //End ifndef SINGLE_PROC
	
	#ifdef CUDA_ENABLED
		//Number of cuda devices
		bool hasCuda;
		int devCount;
	#endif
	
	#ifndef SINGLE_PROC
		//Initialize the MPI environment
		MPI::Init();
		
		//Get number of threads
		nthreads=MPI::COMM_WORLD.Get_size();
		
		//Number of collumns
		horZones = qDef;
		if(horZones==0) {
			horZones=1;
		}
		else if(horZones>nthreads) {
			horZones=nthreads;
		}
		verZones=nthreads/horZones;
		
		threads[0]=horZones;
		threads[1]=verZones;
		
		//Create two dimensional cartesian grouping that doesn't wrap around
		cartComm=MPI::COMM_WORLD.Create_cart(2,threads,wrapAround,true);
		
		//If this thread is not needed, then finalize and exit
		if(cartComm==MPI::COMM_NULL) {
			MPI::Finalize();
			return EXIT_SUCCESS;
		}

		//Get new number of threads
		nthreads=cartComm.Get_size();
		
		//Get new id of thread
		tid=cartComm.Get_rank();
		
		//Get neighbours
		cartComm.Shift(0,1,destL,destR);
		cartComm.Shift(1,1,destU,destD);
	#endif //End ifndef SINGLE_PROC
	
	#ifndef SINGLE_PROC
		//Find out how many elements this zone should have
		horElements = sqrtN/horZones;
		verElements = sqrtN/verZones;
	#else
		horElements=sqrtN;
		verElements=sqrtN;
	#endif
	
	#ifndef SINGLE_PROC
		//If this is in the right/bottom edge (which we can tell since we have no right/bottom neighbour)
		//Find how many more elements we need and add them to the number of elements of this zone
		if(destR==MPI::PROC_NULL) {
			horDeficit=sqrtN%horZones;
			horElements+=horDeficit;
		}
		if(destD==MPI::PROC_NULL) {
			verDeficit=sqrtN%verZones;
			verElements+=verDeficit;
		}
	#endif
	
	verTotal=verElements+2;
	horTotal=horElements+2;
	
	//Allocate the local grid plus the necessary border buffers
	//and initialize everything to zero
	double ** elems = new double*[horTotal];
	for(int i=0;i<horTotal;++i) {
		elems[i]=new double[verTotal](); //() means initialize to 0
	}
	
	#ifndef SINGLE_PROC
		//If this is on the right edge initialise the right edge to 1
		if(destR==MPI::PROC_NULL) {
			for(int i=0;i<verTotal;++i) {
				elems[horTotal-1][i]=1.0;
			}
		}
	#else
		for(int i=0;i<verTotal;++i) {
			elems[horTotal-1][i]=1.0;
		}
	#endif
	
	#ifndef SINGLE_PROC
		//Allocate communication buffers if necessary
		if(destR!=MPI::PROC_NULL) {
			bufSR=new double[verElements](); //() means initialize to 0
			bufRR=new double[verElements](); //() means initialize to 0
		}
		if(destL!=MPI::PROC_NULL) {
			bufSL=new double[verElements](); //() means initialize to 0
			bufRL=new double[verElements](); //() means initialize to 0
		}
		if(destU!=MPI::PROC_NULL) {
			bufSU=new double[horElements](); //() means initialize to 0
			bufRU=new double[horElements](); //() means initialize to 0
		}
		if(destD!=MPI::PROC_NULL) {
			bufSD=new double[horElements](); //() means initialize to 0
			bufRD=new double[horElements](); //() means initialize to 0
		}
		
		//Compute if the upper left element (element, NOT edge) of this block is red or black
		//then use that to tell the next block
		//First we receive data from the previous block, if it exists
		if(destU!=MPI::PROC_NULL) {
			cartComm.Recv(&isFirstElemRed,1,MPI::BOOL,destU,INIT_TAG);
		}
		if(destL!=MPI::PROC_NULL) {
			cartComm.Recv(&isFirstElemRed,1,MPI::BOOL,destL,INIT_TAG);
		}
		//If the previous block doesn't exist, then we are at the upper left block and we
		//know the upper left block starts red
		if(destU==MPI::PROC_NULL && destL==MPI::PROC_NULL) {
			isFirstElemRed=true;
		}
		//Send data to the next block if it exists
		if(destD!=MPI::PROC_NULL) {
			//verElements%2==0 when the first element is red and the next block's first element is also red
			//So if isFirstElemRed==true and (verElements%2==0)==true then isNextFirstElemRed==true
			//if isFirstElemRed==true and (verElements%2==0)==false then isNextFirstElemRed==false
			//and so on
			bool isNextFirstElemRed=(((verElements%2)==0)==isFirstElemRed);
			cartComm.Send(&isNextFirstElemRed,1,MPI::BOOL,destD,INIT_TAG);
		}
		if(destR!=MPI::PROC_NULL) {
			bool isNextFirstElemRed=(((horElements%2)==0)==isFirstElemRed);
			cartComm.Send(&isNextFirstElemRed,1,MPI::BOOL,destR,INIT_TAG);
		}
	#else
		isFirstElemRed=true;
	#endif
	
	#ifdef CUDA_ENABLED
		//Set up cuda
		cudaGetDeviceCount(&devCount);
		if(devCount>0) {
			hasCuda=true;
		}
		dim3 threadsPerBlock(1);
		dim3 cudaBlocks(horElements,verElements);
	#endif
	
	#ifndef SINGLE_PROC
		//Wait for all processes to get here
		cartComm.Barrier();
		
		//Get start time
		if(tid==0) {
			time_initial = MPI::Wtime();
		}
	#else
		time_initial = high_resolution_clock::now();
	#endif
	
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
		
		#ifndef SINGLE_PROC
			//wait for the data from the appropriate receive buffer each time the looop starts 
			//and copy them to the edge. Then send a request for the next batch of edges 
			//(that will complete on the next loop)
			if(destR!=MPI::PROC_NULL) {
				reqRR.Wait();
				for(int j=1;j<verTotal-2;++j) {
					elems[horTotal-1][j]=bufRR[j-1];
				}
				reqRR=cartComm.Irecv(bufRR,verElements,MPI::DOUBLE,destR,BUFF_TAG);
			}
			if(destL!=MPI::PROC_NULL) {
				reqRL.Wait();
				for(int j=1;j<verTotal-2;++j) {
					elems[0][j]=bufRL[j-1];
				}
				reqRL=cartComm.Irecv(bufRL,verElements,MPI::DOUBLE,destL,BUFF_TAG);
			}
			if(destU!=MPI::PROC_NULL) {
				reqRU.Wait();
				for(int i=1;i<horTotal-2;++i) {
					elems[i][0]=bufRU[i-1];
				}
				reqRU=cartComm.Irecv(bufRU,horElements,MPI::DOUBLE,destU,BUFF_TAG);
			}
			if(destD!=MPI::PROC_NULL) {
				reqRD.Wait();
				for(int i=1;i<horTotal-2;++i) {
					elems[i][verTotal-1]=bufRD[i-1];
				}
				reqRD=cartComm.Irecv(bufRD,horElements,MPI::DOUBLE,destD,BUFF_TAG);
			}
		#endif
		
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
		
		#ifndef SINGLE_PROC
			//Make a reduce request, now that we have all the data
			double allSum;
			if(checkIter) {
				MPI_Iallreduce(&diffSum,&allSum,1,MPI::DOUBLE,MPI::SUM,cartComm,&reqSum);
			}
		
			//Finally, wait on the send buffers, copy new data there and 
			//make the buffers availiable to the other processes 
			if(destR!=MPI::PROC_NULL) {
				reqSR.Wait();
				for(int j=1;j<verTotal-2;++j) {
					bufSR[j-1]=elems[horTotal-1][j];
				}
				reqSR=cartComm.Isend(bufSR,verElements,MPI::DOUBLE,destR,BUFF_TAG);
			}
			if(destL!=MPI::PROC_NULL) {
				reqSL.Wait();
				for(int j=1;j<verTotal-2;++j) {
					bufSL[j-1]=elems[0][j];
				}
				reqSL=cartComm.Isend(bufSL,verElements,MPI::DOUBLE,destL,BUFF_TAG);
			}
			if(destU!=MPI::PROC_NULL) {
				reqSU.Wait();
				for(int i=1;i<horTotal-2;++i) {
					bufSU[i-1]=elems[i][0];
				}
				reqSU=cartComm.Isend(bufSU,horElements,MPI::DOUBLE,destU,BUFF_TAG);
			}
			if(destD!=MPI::PROC_NULL) {
				reqSD.Wait();
				for(int i=1;i<horTotal-2;++i) {
					bufSD[i-1]=elems[i][verTotal-1];
				}
				reqSD=cartComm.Isend(bufSD,horElements,MPI::DOUBLE,destD,BUFF_TAG);
			}
		#endif
		
		#ifndef SINGLE_PROC
			//Each ITER_CHECK times, check for the sum and exit if we have reached the required level of convergence
			if(checkIter) {
				MPI_Wait(&reqSum,MPI_STATUS_IGNORE);
				if(sqrt(allSum)<epsilon) {
					break;
				}
			}
		#else
			if(checkIter && sqrt(diffSum)<epsilon) {
				break;
			}
		#endif
		
	} while(true);
	
	#ifndef SINGLE_PROC
		if(tid==0) {
			//Get end time
			time_end = MPI::Wtime();
			//Output time taken to complete
			cout<<fixed<<setprecision(20)<<(time_end-time_initial)<<endl;
			//cerr<<fixed<<setprecision(20)<<(time_end-time_initial)<<endl;
		}
	#else
		//Get end time
		time_end = high_resolution_clock::now();
		duration<double> dur= duration_cast<duration<double>>(time_end-time_initial);
		//Output time taken to complete
		cout<<fixed<<setprecision(20)<<dur.count()<<endl;
		//cerr<<fixed<<setprecision(20)<<(time_end-time_initial)<<endl;
	#endif
	
	//free memory
	for(int i=0;i<horTotal;++i) {
		delete[] elems[i];
	}
	delete[] elems;
	
	#ifndef SINGLE_PROC
		//Allocate communication buffers if necessary
		if(destR!=MPI::PROC_NULL) {
			delete[] bufSR;
			delete[] bufRR;
		}
		if(destL!=MPI::PROC_NULL) {
			delete[] bufSL;
			delete[] bufRL;
		}
		if(destU!=MPI::PROC_NULL) {
			delete[] bufSU;
			delete[] bufRU;
		}
		if(destD!=MPI::PROC_NULL) {
			delete[] bufSD;
			delete[] bufRD;
		}
		
		//Finalize the MPI environment
		MPI::Finalize();
	#endif
	
	//Exit
	return EXIT_SUCCESS;
}


inline double equation(double omega, double** elems, int i, int j) {
	return (1.0-omega)*(elems[i][j])+(omega/4.0)*(elems[i-1][j]+elems[i][j-1]+elems[i][j+1]+elems[i+1][j]);
}
