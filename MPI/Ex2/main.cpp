#include <iostream>
#include <iomanip>

#include <cstdlib>
#include <cmath>

//#define SINGLE_PROC //Used for testing with a single process

#ifndef SINGLE_PROC
	#include <mpi.h>
#else
	#include <chrono>
#endif

//#define CUDA_ENABLED

#ifdef CUDA_ENABLED
	#include <cuda.h>
#endif

using namespace std;
#ifdef SINGLE_PROC
	using namespace chrono;
#endif

#ifndef ITER_CHECK
	#define ITER_CHECK 50 //The program will check if we have converged every ITER_CHECK iterations 
#endif

#ifndef sqrtN
	#define sqrtN 150 //The squre root of the number of elements in the grid
#endif

#ifndef epsilon
	#define epsilon 1e-5 //When the total change occuring in the entire array is less than epsilon, the program stops solving
#endif

#ifndef qDef
	//#define qDef sqrt(nthreads)/4
	//#define qDef sqrt(nthreads)/2
	#define qDef sqrt(nthreads)
	//#define qDef sqrt(nthreads)*2
	//#define qDef sqrt(nthreads)*4
#endif

#ifndef omegaRed
	#define omegaRed 0.1
	//#define omegaRed 0.2
	//...
	//#define omegaRed 1.8
	//#define omegaRed 1.9
#endif

#ifndef omegaBlack
	#define omegaBlack 0.1
	//#define omegaBlack 0.2
	//...
	//#define omegaBlack 1.8
	//#define omegaBlack 1.9
#endif

#define INIT_TAG 1 //Used to send initialisation data
#define BUFF_TAG 2 //Used to send data from block to block. In multibuffer mode, BUFF_TAG+<buffer number> is sent used as the tag

//#define MULTI_BUFFER

#ifdef MULTI_BUFFER
	#ifndef BUFFERS
		#define BUFFERS 4
	#endif
#endif

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
		//Initialize the MPI environment
		MPI::Init();
	
		MPI::Cartcomm cartComm; //Cartesian communicator
		
		int tid; //ID of the current thread
		
		int nthreads; //Number of threads
	#endif
		
	#ifndef SINGLE_PROC
		double time_initial;
		double time_end;
	#else //Instead of MPI Wtime we use the high resolution clock for single process mode
		high_resolution_clock::time_point time_initial;
		high_resolution_clock::time_point time_end;
	#endif
	
	#ifndef SINGLE_PROC
		int verZones; //Threads per row
		int horZones; //Threads per column
	#endif
	
	int horElements; //Number of horizontal elements in this zone
	int verElements; //Number of vertical elements in this zone
	#ifndef SINGLE_PROC
		int horDeficit=0; //How many more horizontal elements this has compared to other zones
		int verDeficit=0; //How mant more vertical elements this has compared to other zones
	#endif
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
		#ifndef MULTI_BUFFER
			double* bufSL;
			double* bufRL;
			double* bufSR;
			double* bufRR;
			double* bufSD;
			double* bufRD;
			double* bufSU;
			double* bufRU;
		#else //If we're using multiple buffers
			double** bufSL;
			double** bufRL;
			double** bufSR;
			double** bufRR;
			double** bufSD;
			double** bufRD;
			double** bufSU;
			double** bufRU;
		#endif //End ifndef MULTI_BUFFER
	
		//Send and receive requests for asynchronous communication. Initializing as null lets us wait on them
		//even when no request has been made (useful for the first iteration).
		#ifndef MULTI_BUFFER
			MPI::Request reqSL=MPI::REQUEST_NULL;
			MPI::Request reqRL=MPI::REQUEST_NULL;
			MPI::Request reqSU=MPI::REQUEST_NULL;
			MPI::Request reqRU=MPI::REQUEST_NULL;
			MPI::Request reqSD=MPI::REQUEST_NULL;
			MPI::Request reqRD=MPI::REQUEST_NULL;
			MPI::Request reqSR=MPI::REQUEST_NULL;
			MPI::Request reqRR=MPI::REQUEST_NULL;
		#else //If we're using multiple buffers
			short sBuf=0; //Holds the currently selected buffer
		
			MPI::Request reqSL[BUFFERS];
			MPI::Request reqRL[BUFFERS];
			MPI::Request reqSU[BUFFERS];
			MPI::Request reqRU[BUFFERS];
			MPI::Request reqSD[BUFFERS];
			MPI::Request reqRD[BUFFERS];
			MPI::Request reqSR[BUFFERS];
			MPI::Request reqRR[BUFFERS];
			for(int i=0;i<BUFFERS;++i) {
				reqSL[i]=MPI::REQUEST_NULL;
				reqRL[i]=MPI::REQUEST_NULL;
				reqSU[i]=MPI::REQUEST_NULL;
				reqRU[i]=MPI::REQUEST_NULL;
				reqSD[i]=MPI::REQUEST_NULL;
				reqRD[i]=MPI::REQUEST_NULL;
				reqSR[i]=MPI::REQUEST_NULL;
				reqRR[i]=MPI::REQUEST_NULL;
			}
		#endif //End ifndef MULTI_BUFFER
	
	//Used for the asynchrous sum allreduce
		MPI_Request reqSum;
	#endif //End ifndef SINGLE_PROC
	
	#ifdef CUDA_ENABLED
		//Number of cuda devices
		bool hasCuda;
		int devCount;
	#endif
	
	#ifndef SINGLE_PROC
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
		#ifndef MULTI_BUFFER
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
		#else
			//Allocate communication buffers if necessary
			if(destR!=MPI::PROC_NULL) {
				bufSR=new double*[BUFFERS];
				bufRR=new double*[BUFFERS];
			}
			if(destL!=MPI::PROC_NULL) {
				bufSL=new double*[BUFFERS];
				bufRL=new double*[BUFFERS];
			}
			if(destU!=MPI::PROC_NULL) {
				bufSU=new double*[BUFFERS];
				bufRU=new double*[BUFFERS];
			}
			if(destD!=MPI::PROC_NULL) {
				bufSD=new double*[BUFFERS];
				bufRD=new double*[BUFFERS];
			}
			for(int i = 0; i<BUFFERS; ++i) {
				if(destR!=MPI::PROC_NULL) {
					bufSR[i]=new double[verElements](); //() means initialize to 0
					bufRR[i]=new double[verElements](); //() means initialize to 0
				}
				if(destL!=MPI::PROC_NULL) {
					bufSL[i]=new double[verElements](); //() means initialize to 0
					bufRL[i]=new double[verElements](); //() means initialize to 0
				}
				if(destU!=MPI::PROC_NULL) {
					bufSU[i]=new double[horElements](); //() means initialize to 0
					bufRU[i]=new double[horElements](); //() means initialize to 0
				}
				if(destD!=MPI::PROC_NULL) {
					bufSD[i]=new double[horElements](); //() means initialize to 0
					bufRD[i]=new double[horElements](); //() means initialize to 0
				}
			}
			
			//For all receive buffers other than the first, make a recv request
			//Imagine we have another processes. When the other process sends something with its sBuf==0, we'll receive it in our sBuf==1 (because the 0th request is NULL)
			//When the other process sends with its sBuf==1, we'll receive it in our sBuf==2, and so on
			//That means that we will always get the correct data and not the previous or next data. The BUFF_TAG+i helps us further ensure that
			for(int i=1;i<BUFFERS;++i) {
				if(destR!=MPI::PROC_NULL) {
					reqRR[i]=cartComm.Irecv(bufRR[i],verElements,MPI::DOUBLE,destR,BUFF_TAG+i);
				}
				if(destL!=MPI::PROC_NULL) {
					reqRL[i]=cartComm.Irecv(bufRL[i],verElements,MPI::DOUBLE,destL,BUFF_TAG+i);
				}
				if(destU!=MPI::PROC_NULL) {
					reqRU[i]=cartComm.Irecv(bufRU[i],horElements,MPI::DOUBLE,destU,BUFF_TAG+i);
				}
				if(destD!=MPI::PROC_NULL) {
					reqRD[i]=cartComm.Irecv(bufRD[i],horElements,MPI::DOUBLE,destD,BUFF_TAG+i);
				}
			}
		#endif
		
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
			#ifndef MULTI_BUFFER
				//wait for the data from the appropriate receive buffer each time the looop starts 
				//and copy them to the edge. Then send a request for the next batch of edges 
				//(that will complete on the next loop)
				if(destR!=MPI::PROC_NULL) {
					reqRR.Wait();
					for(int j=1;j<verTotal-1;++j) {
						elems[horTotal-1][j]=bufRR[j-1];
					}
					reqRR=cartComm.Irecv(bufRR,verElements,MPI::DOUBLE,destR,BUFF_TAG);
				}
				if(destL!=MPI::PROC_NULL) {
					reqRL.Wait();
					for(int j=1;j<verTotal-1;++j) {
						elems[0][j]=bufRL[j-1];
					}
					reqRL=cartComm.Irecv(bufRL,verElements,MPI::DOUBLE,destL,BUFF_TAG);
				}
				if(destU!=MPI::PROC_NULL) {
					reqRU.Wait();
					for(int i=1;i<horTotal-1;++i) {
						elems[i][0]=bufRU[i-1];
					}
					reqRU=cartComm.Irecv(bufRU,horElements,MPI::DOUBLE,destU,BUFF_TAG);
				}
				if(destD!=MPI::PROC_NULL) {
					reqRD.Wait();
					for(int i=1;i<horTotal-1;++i) {
						elems[i][verTotal-1]=bufRD[i-1];
					}
					reqRD=cartComm.Irecv(bufRD,horElements,MPI::DOUBLE,destD,BUFF_TAG);
				}
			#else //If we are not a single process and are using multiple buffers
				//Save the previous buffer and select the next buffer
				short prevBuf=sBuf;
				sBuf=(sBuf+1)%BUFFERS;
				
				//Receive the data. Copying is unnecessary, since they'll be copied during the main loop
				//Make sure send buffers have been sent and we can safely write on them
				if(destR!=MPI::PROC_NULL) {
					reqRR[prevBuf].Wait();
					reqSR[sBuf].Wait();
				}
				if(destL!=MPI::PROC_NULL) {
					reqRL[prevBuf].Wait();
					reqSL[sBuf].Wait();
				}
				if(destU!=MPI::PROC_NULL) {
					reqRU[prevBuf].Wait();
					reqSU[sBuf].Wait();
				}
				if(destD!=MPI::PROC_NULL) {
					reqRD[prevBuf].Wait();
					reqSU[sBuf].Wait();
				}
			#endif
		#endif
		
		#if defined(SINGLE_PROC) || !defined(MULTI_BUFFER) //If we are a single process or are not using multiple buffers
			//Compute all red
			for(int i=1;i<horTotal-1;++i) {
				for(int j=((isFirstElemRed)?((i+1)%2):(i%2))+1;j<verTotal-1;j+=2) {
					double newVal=equation(omegaRed,elems,i,j);
					if(checkIter) {
						diffSum=diffSum+((newVal-elems[i][j])*(newVal-elems[i][j]));
					}
					elems[i][j]=newVal;
				}
			}
			
			//Exchange data and get the new red elements
			#ifndef SINGLE_PROC
				//Finally, wait on the send buffers, copy new data there and 
				//make the buffers availiable to the other processes 
				if(destR!=MPI::PROC_NULL) {
					reqSR.Wait();
					for(int j=1;j<verTotal-1;++j) {
						bufSR[j-1]=elems[horTotal-2][j];
					}
					reqSR=cartComm.Isend(bufSR,verElements,MPI::DOUBLE,destR,BUFF_TAG);
				}
				if(destL!=MPI::PROC_NULL) {
					reqSL.Wait();
					for(int j=1;j<verTotal-1;++j) {
						bufSL[j-1]=elems[1][j];
					}
					reqSL=cartComm.Isend(bufSL,verElements,MPI::DOUBLE,destL,BUFF_TAG);
				}
				if(destU!=MPI::PROC_NULL) {
					reqSU.Wait();
					for(int i=1;i<horTotal-1;++i) {
						bufSU[i-1]=elems[i][1];
					}
					reqSU=cartComm.Isend(bufSU,horElements,MPI::DOUBLE,destU,BUFF_TAG);
				}
				if(destD!=MPI::PROC_NULL) {
					reqSD.Wait();
					for(int i=1;i<horTotal-1;++i) {
						bufSD[i-1]=elems[i][verTotal-2];
					}
					reqSD=cartComm.Isend(bufSD,horElements,MPI::DOUBLE,destD,BUFF_TAG);
				}
				//wait for the data from the appropriate receive buffer
				//and copy them to the edge. Then send a request for the next batch of edges 
				//(that will complete on the next loop)
				if(destR!=MPI::PROC_NULL) {
					reqRR.Wait();
					for(int j=1;j<verTotal-1;++j) {
						elems[horTotal-1][j]=bufRR[j-1];
					}
					reqRR=cartComm.Irecv(bufRR,verElements,MPI::DOUBLE,destR,BUFF_TAG);
				}
				if(destL!=MPI::PROC_NULL) {
					reqRL.Wait();
					for(int j=1;j<verTotal-1;++j) {
						elems[0][j]=bufRL[j-1];
					}
					reqRL=cartComm.Irecv(bufRL,verElements,MPI::DOUBLE,destL,BUFF_TAG);
				}
				if(destU!=MPI::PROC_NULL) {
					reqRU.Wait();
					for(int i=1;i<horTotal-1;++i) {
						elems[i][0]=bufRU[i-1];
					}
					reqRU=cartComm.Irecv(bufRU,horElements,MPI::DOUBLE,destU,BUFF_TAG);
				}
				if(destD!=MPI::PROC_NULL) {
					reqRD.Wait();
					for(int i=1;i<horTotal-1;++i) {
						elems[i][verTotal-1]=bufRD[i-1];
					}
					reqRD=cartComm.Irecv(bufRD,horElements,MPI::DOUBLE,destD,BUFF_TAG);
				}
			#endif
			
			//Do the same as above but for black
			for(int i=1;i<horTotal-1;++i) {
				for(int j=((isFirstElemRed)?(i%2):((i+1)%2))+1;j<verTotal-1;j+=2) {
					double newVal=equation(omegaBlack,elems,i,j);
					if(checkIter) {
						diffSum=diffSum+((newVal-elems[i][j])*(newVal-elems[i][j]));
					}
					elems[i][j]=newVal;
				}
			}
		#else //If we are not a single process and are using multiple buffers
			//If the first element is red
			if(isFirstElemRed) {
				/////////////RED STARTS HERE//////////////
				
				//We start with the red elements
					//For the first element
						//Get the data from the receive buffers for the first element
				if(destL!=MPI::PROC_NULL) {
					elems[0][1]=bufRL[prevBuf][0];
				}
				if(destU!=MPI::PROC_NULL) {
					elems[1][0]=bufRU[prevBuf][0];
				}
						//Compute the new value for the first element
				double newVal=equation(omegaRed,elems,1,1);
				if(checkIter) {
					diffSum=diffSum+((newVal-elems[1][1])*(newVal-elems[1][1]));
				}
				elems[1][1]=newVal;
						//Put that value to the necessary send buffers
				if(destL!=MPI::PROC_NULL) {
					bufSL[sBuf][0]=elems[1][1];
				}
				if(destU!=MPI::PROC_NULL) {
					bufSU[sBuf][0]=elems[1][1];
				}
					//For the rest of the first collumn
				int j;
				for(j=3;j<verTotal-2;j+=2) {
					//Get the data from the receive buffers
					if(destL!=MPI::PROC_NULL) {
						elems[0][j]=bufRL[prevBuf][j-1];
					}
					//Compute the new value
					newVal=equation(omegaRed,elems,1,j);
					if(checkIter) {
						diffSum=diffSum+((newVal-elems[1][j])*(newVal-elems[1][j]));
					}
					elems[1][j]=newVal;
					//Put that value in the necessary send buffers
					if(destL!=MPI::PROC_NULL) {
						bufSL[sBuf][j-1]=elems[1][j];
					}
				}
					//For the last element of the first collumn, if it is red
				if(j==verTotal-2) {
					//Get the data from the receive buffers
					if(destL!=MPI::PROC_NULL) {
						elems[0][j]=bufRL[prevBuf][j-1];
					}
					if(destD!=MPI::PROC_NULL) {
						elems[1][j+1]=bufRD[prevBuf][0];
					}
					//Compute the new value
					newVal=equation(omegaRed,elems,1,j);
					if(checkIter) {
						diffSum=diffSum+((newVal-elems[1][j])*(newVal-elems[1][j]));
					}
					elems[1][j]=newVal;
					//Put the value in the necessary send buffers
					if(destL!=MPI::PROC_NULL) {
						bufSL[sBuf][j-1]=elems[1][j];
					}
					if(destD!=MPI::PROC_NULL) {
						bufSD[sBuf][0]=elems[1][j];
					}
				}
					//For all collumns, except the end collumn
					//((A loop for the first black, a check for end condition with a break, then a loop for the first red))
				int i;
				for(i=2;i<horTotal-2;i+=2) {
					//We know the first (elem[i][1]) is black, so we start with the red and compute them normally (no need to retrieve data)
					for(j=2;j<verTotal-2;j+=2) {
						//Compute the new value
						newVal=equation(omegaRed,elems,i,j);
						if(checkIter) {
							diffSum=diffSum+((newVal-elems[i][j])*(newVal-elems[i][j]));
						}
						elems[i][j]=newVal;
					}
					//If the last collumn element is red, check for bottom process
					if(j==verTotal-2) {
						if(destD!=MPI::PROC_NULL) {
							elems[i][j+1]=bufRD[prevBuf][i-1];
						}
						//Compute the new value
						newVal=equation(omegaRed,elems,i,j);
						if(checkIter) {
							diffSum=diffSum+((newVal-elems[i][j])*(newVal-elems[i][j]));
						}
						elems[i][j]=newVal;
						if(destD!=MPI::PROC_NULL) {
							bufSD[sBuf][i-1]=elems[i][j];
						}
					}
					//Check if we have reached the end 
					//(horTotal-1 is the edge collumn, so we don't want to compute that or we get segfaults when trying to access elements beyond the edge)
					//(horTotal-2 is the end collumn, and we don't want to compute that in the current loop because we want to send its results to the right)
					int newI=i+1;
					if(newI>=horTotal-2) {
						break;
					}
					
					//We know the next collumn is going to have the first element be red, so we need to send/receive for that
					if(destU!=MPI::PROC_NULL) {
						elems[newI][0]=bufRU[prevBuf][newI-1];
					}
							//Compute the new value for the first element
					double newVal=equation(omegaRed,elems,newI,1);
					if(checkIter) {
						diffSum=diffSum+((newVal-elems[newI][1])*(newVal-elems[newI][1]));
					}
					elems[newI][1]=newVal;
							//Put that value to the necessary send buffers
					if(destU!=MPI::PROC_NULL) {
						bufSU[sBuf][newI-1]=elems[newI][1];
					}
					//Then we loop through all the others normally
					for(j=3;j<verTotal-2;j+=2) {
						//Compute the new value
						newVal=equation(omegaRed,elems,newI,j);
						if(checkIter) {
							diffSum=diffSum+((newVal-elems[newI][j])*(newVal-elems[newI][j]));
						}
						elems[newI][j]=newVal;
					}
					//If the last collumn element is red, check for bottom process
					if(j==verTotal-2) {
						if(destD!=MPI::PROC_NULL) {
							elems[newI][j+1]=bufRD[prevBuf][newI-1];
						}
						//Compute the new value
						newVal=equation(omegaRed,elems,newI,j);
						if(checkIter) {
							diffSum=diffSum+((newVal-elems[newI][j])*(newVal-elems[newI][j]));
						}
						elems[newI][j]=newVal;
						if(destD!=MPI::PROC_NULL) {
							bufSD[sBuf][newI-1]=elems[newI][j];
						}
					}
				}
				//Now we check if the loop ended during the red or during the black part
				//If i==horTotal-2, then it ended during the black part and we know the first collumn element is black
				if(i==horTotal-2) {
					//So we simply compute all the elements and send them right
					for(j=2;j<verTotal-2;j+=2) {
						//Get the data from the receive buffers
						if(destR!=MPI::PROC_NULL) {
							elems[i+1][j]=bufRR[prevBuf][j-1];
						}
						//Compute the new value
						newVal=equation(omegaRed,elems,i,j);
						if(checkIter) {
							diffSum=diffSum+((newVal-elems[i][j])*(newVal-elems[i][j]));
						}
						elems[i][j]=newVal;
						//Put that value in the necessary send buffers
						if(destR!=MPI::PROC_NULL) {
							bufSR[sBuf][j-1]=elems[i][j];
						}
					}
					//And down if necessary
					if(j==verTotal-2) {
						//Get the data from the receive buffers
						if(destR!=MPI::PROC_NULL) {
							elems[i+1][j]=bufRR[prevBuf][j-1];
						}
						if(destD!=MPI::PROC_NULL) {
							elems[i][j+1]=bufRD[prevBuf][i-1];
						}
						//Compute the new value
						newVal=equation(omegaRed,elems,i,j);
						if(checkIter) {
							diffSum=diffSum+((newVal-elems[i][j])*(newVal-elems[i][j]));
						}
						elems[i][j]=newVal;
						//Put that value in the necessary send buffers
						if(destR!=MPI::PROC_NULL) {
							bufSR[sBuf][j-1]=elems[i][j];
						}
						if(destD!=MPI::PROC_NULL) {
							bufSD[sBuf][i-1]=elems[i][j];
						}
					}
				}
				//Else if i+1==horTotal-2, then it ended during the red part amd we know the first collumn element is red
				else {
					//So first we send the first element up and right
					int newI=i+1;
					if(destR!=MPI::PROC_NULL) {
						elems[newI+1][1]=bufRR[prevBuf][0];
					}
					if(destU!=MPI::PROC_NULL) {
						elems[newI][0]=bufRU[prevBuf][newI-1];
					}
					//Compute the new value
					newVal=equation(omegaRed,elems,newI,1);
					if(checkIter) {
						diffSum=diffSum+((newVal-elems[newI][1])*(newVal-elems[newI][1]));
					}
					elems[newI][1]=newVal;
					//Put that value in the necessary send buffers
					if(destR!=MPI::PROC_NULL) {
						bufSR[sBuf][0]=elems[newI][1];
					}
					if(destU!=MPI::PROC_NULL) {
						bufSU[sBuf][newI-1]=elems[newI][1];
					}
					//Then we compute the rest of the elements normally and send them right
					for(j=3;j<verTotal-2;j+=2) {
						//Get the data from the receive buffers
						if(destR!=MPI::PROC_NULL) {
							elems[newI+1][j]=bufRR[prevBuf][j-1];
						}
						//Compute the new value
						newVal=equation(omegaRed,elems,newI,j);
						if(checkIter) {
							diffSum=diffSum+((newVal-elems[newI][j])*(newVal-elems[newI][j]));
						}
						elems[newI][j]=newVal;
						//Put that value in the necessary send buffers
						if(destR!=MPI::PROC_NULL) {
							bufSR[sBuf][j-1]=elems[newI][j];
						}
					}
					//And send the final element down if necessary
					if(j==verTotal-2) {
						//Get the data from the receive buffers
						if(destR!=MPI::PROC_NULL) {
							elems[newI+1][j]=bufRR[prevBuf][j-1];
						}
						if(destD!=MPI::PROC_NULL) {
							elems[newI][j+1]=bufRD[prevBuf][newI-1];
						}
						//Compute the new value
						newVal=equation(omegaRed,elems,newI,j);
						if(checkIter) {
							diffSum=diffSum+((newVal-elems[newI][j])*(newVal-elems[newI][j]));
						}
						elems[newI][j]=newVal;
						//Put that value in the necessary send buffers
						if(destR!=MPI::PROC_NULL) {
							bufSR[sBuf][j-1]=elems[newI][j];
						}
						if(destD!=MPI::PROC_NULL) {
							bufSD[sBuf][newI-1]=elems[newI][j];
						}
					}
				}
				
				/////////////MID-POINT-DATA-TRANSFER////////////////
				
				//Send buffers should already be filled, so we just send them over. 
				//Same with receive buffers. Their contents have been transferred so they are ready to receive
				if(destR!=MPI::PROC_NULL) {
					reqSR[sBuf]=cartComm.Isend(bufSR[sBuf],verElements,MPI::DOUBLE,destR,BUFF_TAG+sBuf);
					reqRR[prevBuf]=cartComm.Irecv(bufRR[prevBuf],verElements,MPI::DOUBLE,destR,BUFF_TAG+prevBuf);
				}
				if(destL!=MPI::PROC_NULL) {
					reqSL[sBuf]=cartComm.Isend(bufSL[sBuf],verElements,MPI::DOUBLE,destL,BUFF_TAG+sBuf);
					reqRL[prevBuf]=cartComm.Irecv(bufRL[prevBuf],verElements,MPI::DOUBLE,destL,BUFF_TAG+prevBuf);
				}
				if(destU!=MPI::PROC_NULL) {
					reqSU[sBuf]=cartComm.Isend(bufSU[sBuf],horElements,MPI::DOUBLE,destU,BUFF_TAG+sBuf);
					reqRU[prevBuf]=cartComm.Irecv(bufRU[prevBuf],horElements,MPI::DOUBLE,destU,BUFF_TAG+prevBuf);
				}
				if(destD!=MPI::PROC_NULL) {
					reqSD[sBuf]=cartComm.Isend(bufSD[sBuf],horElements,MPI::DOUBLE,destD,BUFF_TAG+sBuf);
					reqRD[prevBuf]=cartComm.Irecv(bufRD[prevBuf],horElements,MPI::DOUBLE,destD,BUFF_TAG+prevBuf);
				}
				//Save the previous buffer and select the next buffer
				prevBuf=sBuf;
				sBuf=(sBuf+1)%BUFFERS;
				
				//Receive the data. Copying is unnecessary, since they'll be copied during the main loop
				//Make sure send buffers have been sent and we can safely write on them
				if(destR!=MPI::PROC_NULL) {
					reqRR[prevBuf].Wait();
					reqSR[sBuf].Wait();
				}
				if(destL!=MPI::PROC_NULL) {
					reqRL[prevBuf].Wait();
					reqSL[sBuf].Wait();
				}
				if(destU!=MPI::PROC_NULL) {
					reqRU[prevBuf].Wait();
					reqSU[sBuf].Wait();
				}
				if(destD!=MPI::PROC_NULL) {
					reqRD[prevBuf].Wait();
					reqSU[sBuf].Wait();
				}
				
				
				/////////////BLACK STARTS HERE//////////////
					
				//We continue with the black elements
					//For the black elements of the first collumn, we know elem[1][1] is red so we start with the black, elem[1][2]
				for(j=2;j<verTotal-2;j+=2) {
					//Get the data from the receive buffers
					if(destL!=MPI::PROC_NULL) {
						elems[0][j]=bufRL[prevBuf][j-1];
					}
					//Compute the new value
					newVal=equation(omegaRed,elems,1,j);
					if(checkIter) {
						diffSum=diffSum+((newVal-elems[1][j])*(newVal-elems[1][j]));
					}
					elems[1][j]=newVal;
					//Put that value in the necessary send buffers
					if(destL!=MPI::PROC_NULL) {
						bufSL[sBuf][j-1]=elems[1][j];
					}
				}
					//For the last element of the first collumn, if it is black
				if(j==verTotal-2) {
					//Get the data from the receive buffers
					if(destL!=MPI::PROC_NULL) {
						elems[0][j]=bufRL[prevBuf][j-1];
					}
					if(destD!=MPI::PROC_NULL) {
						elems[1][j+1]=bufRD[prevBuf][0];
					}
					//Compute the new value
					newVal=equation(omegaRed,elems,1,j);
					if(checkIter) {
						diffSum=diffSum+((newVal-elems[1][j])*(newVal-elems[1][j]));
					}
					elems[1][j]=newVal;
					//Put the value in the necessary send buffers
					if(destL!=MPI::PROC_NULL) {
						bufSL[sBuf][j-1]=elems[1][j];
					}
					if(destD!=MPI::PROC_NULL) {
						bufSD[sBuf][0]=elems[1][j];
					}
				}
				
				//For all the collumns between first and last
				for(i=2;i<horTotal-2;i+=2) {
					//We know the first (elem[2][1]) is black, so we start with the black and compute them normally
					//First we send/rcv for the first black element
					if(destU!=MPI::PROC_NULL) {
						elems[i][0]=bufRU[prevBuf][i-1];
					}
							//Compute the new value for the first element
					double newVal=equation(omegaRed,elems,i,1);
					if(checkIter) {
						diffSum=diffSum+((newVal-elems[i][1])*(newVal-elems[i][1]));
					}
					elems[i][1]=newVal;
							//Put that value to the necessary send buffers
					if(destU!=MPI::PROC_NULL) {
						bufSU[sBuf][i-1]=elems[i][1];
					}
					//Then we loop through all the others normally
					for(j=3;j<verTotal-2;j+=2) {
						//Compute the new value
						newVal=equation(omegaRed,elems,i,j);
						if(checkIter) {
							diffSum=diffSum+((newVal-elems[i][j])*(newVal-elems[i][j]));
						}
						elems[i][j]=newVal;
					}
					//If the last collumn element is black, check for bottom process
					if(j==verTotal-2) {
						if(destD!=MPI::PROC_NULL) {
							elems[i][j+1]=bufRD[prevBuf][i-1];
						}
						//Compute the new value
						newVal=equation(omegaRed,elems,i,j);
						if(checkIter) {
							diffSum=diffSum+((newVal-elems[i][j])*(newVal-elems[i][j]));
						}
						elems[i][j]=newVal;
						if(destD!=MPI::PROC_NULL) {
							bufSD[sBuf][i-1]=elems[i][j];
						}
					}
					
					
					//Check if we have reached the end 
					//(horTotal-1 is the edge collumn, so we don't want to compute that or we get segfaults when trying to access elements beyond the edge)
					//(horTotal-2 is the end collumn, and we don't want to compute that in the current loop because we want to send its results to the right)
					int newI=i+1;
					if(newI>=horTotal-2) {
						break;
					}
					
					//Now we know the first element of the collumn is red, so we start with the black
					for(j=2;j<verTotal-2;j+=2) {
						//Compute the new value
						newVal=equation(omegaRed,elems,newI,j);
						if(checkIter) {
							diffSum=diffSum+((newVal-elems[newI][j])*(newVal-elems[newI][j]));
						}
						elems[newI][j]=newVal;
					}
					//If the last collumn element is black, check for bottom process
					if(j==verTotal-2) {
						if(destD!=MPI::PROC_NULL) {
							elems[newI][j+1]=bufRD[prevBuf][newI-1];
						}
						//Compute the new value
						newVal=equation(omegaRed,elems,newI,j);
						if(checkIter) {
							diffSum=diffSum+((newVal-elems[newI][j])*(newVal-elems[newI][j]));
						}
						elems[newI][j]=newVal;
						if(destD!=MPI::PROC_NULL) {
							bufSD[sBuf][newI-1]=elems[newI][j];
						}
					}
				}
				//Now we check if the loop ended during the red or during the black part
				//If i==horTotal-2, then it ended during the black part and we know the first collumn element is black
				if(i==horTotal-2) {
					//So first we send the first element up and right
					if(destR!=MPI::PROC_NULL) {
						elems[i+1][1]=bufRR[prevBuf][0];
					}
					if(destU!=MPI::PROC_NULL) {
						elems[i][0]=bufRU[prevBuf][i-1];
					}
					//Compute the new value
					newVal=equation(omegaRed,elems,i,1);
					if(checkIter) {
						diffSum=diffSum+((newVal-elems[i][1])*(newVal-elems[i][1]));
					}
					elems[i][1]=newVal;
					//Put that value in the necessary send buffers
					if(destR!=MPI::PROC_NULL) {
						bufSR[sBuf][0]=elems[i][1];
					}
					if(destU!=MPI::PROC_NULL) {
						bufSU[sBuf][i-1]=elems[i][1];
					}
					//Then we compute the rest of the elements normally and send them right
					for(j=3;j<verTotal-2;j+=2) {
						//Get the data from the receive buffers
						if(destR!=MPI::PROC_NULL) {
							elems[i+1][j]=bufRR[prevBuf][j-1];
						}
						//Compute the new value
						newVal=equation(omegaRed,elems,i,j);
						if(checkIter) {
							diffSum=diffSum+((newVal-elems[i][j])*(newVal-elems[i][j]));
						}
						elems[i][j]=newVal;
						//Put that value in the necessary send buffers
						if(destR!=MPI::PROC_NULL) {
							bufSR[sBuf][j-1]=elems[i][j];
						}
					}
					//And send the final element down if necessary
					if(j==verTotal-2) {
						//Get the data from the receive buffers
						if(destR!=MPI::PROC_NULL) {
							elems[i+1][j]=bufRR[prevBuf][j-1];
						}
						if(destD!=MPI::PROC_NULL) {
							elems[i][j+1]=bufRD[prevBuf][i-1];
						}
						//Compute the new value
						newVal=equation(omegaRed,elems,i,j);
						if(checkIter) {
							diffSum=diffSum+((newVal-elems[i][j])*(newVal-elems[i][j]));
						}
						elems[i][j]=newVal;
						//Put that value in the necessary send buffers
						if(destR!=MPI::PROC_NULL) {
							bufSR[sBuf][j-1]=elems[i][j];
						}
						if(destD!=MPI::PROC_NULL) {
							bufSD[sBuf][i-1]=elems[i][j];
						}
					}
				}
				//Else if i+1==horTotal-2, then it ended during the red part amd we know the first collumn element is red
				else {
					int newI=i+1;
					//So we simply compute all the black elements and send them right
					for(j=2;j<verTotal-2;j+=2) {
						//Get the data from the receive buffers
						if(destR!=MPI::PROC_NULL) {
							elems[newI+1][j]=bufRR[prevBuf][j-1];
						}
						//Compute the new value
						newVal=equation(omegaRed,elems,newI,j);
						if(checkIter) {
							diffSum=diffSum+((newVal-elems[newI][j])*(newVal-elems[newI][j]));
						}
						elems[newI][j]=newVal;
						//Put that value in the necessary send buffers
						if(destR!=MPI::PROC_NULL) {
							bufSR[sBuf][j-1]=elems[newI][j];
						}
					}
					//And down if necessary
					if(j==verTotal-2) {
						//Get the data from the receive buffers
						if(destR!=MPI::PROC_NULL) {
							elems[newI+1][j]=bufRR[prevBuf][j-1];
						}
						if(destD!=MPI::PROC_NULL) {
							elems[newI][j+1]=bufRD[prevBuf][newI-1];
						}
						//Compute the new value
						newVal=equation(omegaRed,elems,newI,j);
						if(checkIter) {
							diffSum=diffSum+((newVal-elems[newI][j])*(newVal-elems[newI][j]));
						}
						elems[newI][j]=newVal;
						//Put that value in the necessary send buffers
						if(destR!=MPI::PROC_NULL) {
							bufSR[sBuf][j-1]=elems[newI][j];
						}
						if(destD!=MPI::PROC_NULL) {
							bufSD[sBuf][newI-1]=elems[newI][j];
						}
					}
				}
			}
			//If the first element is black
			else {
				/////////////BLACK STARTS HERE//////////////
					
				//We continue with the black elements
					//For the black elements of the first collumn, we know elem[1][1] is red so we start with the black, elem[1][2]
				int j;
				for(j=2;j<verTotal-2;j+=2) {
					//Get the data from the receive buffers
					if(destL!=MPI::PROC_NULL) {
						elems[0][j]=bufRL[prevBuf][j-1];
					}
					//Compute the new value
					double newVal=equation(omegaRed,elems,1,j);
					if(checkIter) {
						diffSum=diffSum+((newVal-elems[1][j])*(newVal-elems[1][j]));
					}
					elems[1][j]=newVal;
					//Put that value in the necessary send buffers
					if(destL!=MPI::PROC_NULL) {
						bufSL[sBuf][j-1]=elems[1][j];
					}
				}
					//For the last element of the first collumn, if it is black
				if(j==verTotal-2) {
					//Get the data from the receive buffers
					if(destL!=MPI::PROC_NULL) {
						elems[0][j]=bufRL[prevBuf][j-1];
					}
					if(destD!=MPI::PROC_NULL) {
						elems[1][j+1]=bufRD[prevBuf][0];
					}
					//Compute the new value
					double newVal=equation(omegaRed,elems,1,j);
					if(checkIter) {
						diffSum=diffSum+((newVal-elems[1][j])*(newVal-elems[1][j]));
					}
					elems[1][j]=newVal;
					//Put the value in the necessary send buffers
					if(destL!=MPI::PROC_NULL) {
						bufSL[sBuf][j-1]=elems[1][j];
					}
					if(destD!=MPI::PROC_NULL) {
						bufSD[sBuf][0]=elems[1][j];
					}
				}
				
				//For all the collumns between first and last
				int i;
				for(i=2;i<horTotal-2;i+=2) {
					//We know the first (elem[2][1]) is black, so we start with the black and compute them normally
					//First we send/rcv for the first black element
					if(destU!=MPI::PROC_NULL) {
						elems[i][0]=bufRU[prevBuf][i-1];
					}
							//Compute the new value for the first element
					double newVal=equation(omegaRed,elems,i,1);
					if(checkIter) {
						diffSum=diffSum+((newVal-elems[i][1])*(newVal-elems[i][1]));
					}
					elems[i][1]=newVal;
							//Put that value to the necessary send buffers
					if(destU!=MPI::PROC_NULL) {
						bufSU[sBuf][i-1]=elems[i][1];
					}
					//Then we loop through all the others normally
					for(j=3;j<verTotal-2;j+=2) {
						//Compute the new value
						newVal=equation(omegaRed,elems,i,j);
						if(checkIter) {
							diffSum=diffSum+((newVal-elems[i][j])*(newVal-elems[i][j]));
						}
						elems[i][j]=newVal;
					}
					//If the last collumn element is black, check for bottom process
					if(j==verTotal-2) {
						if(destD!=MPI::PROC_NULL) {
							elems[i][j+1]=bufRD[prevBuf][i-1];
						}
						//Compute the new value
						newVal=equation(omegaRed,elems,i,j);
						if(checkIter) {
							diffSum=diffSum+((newVal-elems[i][j])*(newVal-elems[i][j]));
						}
						elems[i][j]=newVal;
						if(destD!=MPI::PROC_NULL) {
							bufSD[sBuf][i-1]=elems[i][j];
						}
					}
					
					
					//Check if we have reached the end 
					//(horTotal-1 is the edge collumn, so we don't want to compute that or we get segfaults when trying to access elements beyond the edge)
					//(horTotal-2 is the end collumn, and we don't want to compute that in the current loop because we want to send its results to the right)
					int newI=i+1;
					if(newI>=horTotal-2) {
						break;
					}
					
					//Now we know the first element of the collumn is red, so we start with the black
					for(j=2;j<verTotal-2;j+=2) {
						//Compute the new value
						newVal=equation(omegaRed,elems,newI,j);
						if(checkIter) {
							diffSum=diffSum+((newVal-elems[newI][j])*(newVal-elems[newI][j]));
						}
						elems[newI][j]=newVal;
					}
					//If the last collumn element is black, check for bottom process
					if(j==verTotal-2) {
						if(destD!=MPI::PROC_NULL) {
							elems[newI][j+1]=bufRD[prevBuf][newI-1];
						}
						//Compute the new value
						newVal=equation(omegaRed,elems,newI,j);
						if(checkIter) {
							diffSum=diffSum+((newVal-elems[newI][j])*(newVal-elems[newI][j]));
						}
						elems[newI][j]=newVal;
						if(destD!=MPI::PROC_NULL) {
							bufSD[sBuf][newI-1]=elems[newI][j];
						}
					}
				}
				//Now we check if the loop ended during the red or during the black part
				//If i==horTotal-2, then it ended during the black part and we know the first collumn element is black
				if(i==horTotal-2) {
					//So first we send the first element up and right
					if(destR!=MPI::PROC_NULL) {
						elems[i+1][1]=bufRR[prevBuf][0];
					}
					if(destU!=MPI::PROC_NULL) {
						elems[i][0]=bufRU[prevBuf][i-1];
					}
					//Compute the new value
					double newVal=equation(omegaRed,elems,i,1);
					if(checkIter) {
						diffSum=diffSum+((newVal-elems[i][1])*(newVal-elems[i][1]));
					}
					elems[i][1]=newVal;
					//Put that value in the necessary send buffers
					if(destR!=MPI::PROC_NULL) {
						bufSR[sBuf][0]=elems[i][1];
					}
					if(destU!=MPI::PROC_NULL) {
						bufSU[sBuf][i-1]=elems[i][1];
					}
					//Then we compute the rest of the elements normally and send them right
					for(j=3;j<verTotal-2;j+=2) {
						//Get the data from the receive buffers
						if(destR!=MPI::PROC_NULL) {
							elems[i+1][j]=bufRR[prevBuf][j-1];
						}
						//Compute the new value
						newVal=equation(omegaRed,elems,i,j);
						if(checkIter) {
							diffSum=diffSum+((newVal-elems[i][j])*(newVal-elems[i][j]));
						}
						elems[i][j]=newVal;
						//Put that value in the necessary send buffers
						if(destR!=MPI::PROC_NULL) {
							bufSR[sBuf][j-1]=elems[i][j];
						}
					}
					//And send the final element down if necessary
					if(j==verTotal-2) {
						//Get the data from the receive buffers
						if(destR!=MPI::PROC_NULL) {
							elems[i+1][j]=bufRR[prevBuf][j-1];
						}
						if(destD!=MPI::PROC_NULL) {
							elems[i][j+1]=bufRD[prevBuf][i-1];
						}
						//Compute the new value
						newVal=equation(omegaRed,elems,i,j);
						if(checkIter) {
							diffSum=diffSum+((newVal-elems[i][j])*(newVal-elems[i][j]));
						}
						elems[i][j]=newVal;
						//Put that value in the necessary send buffers
						if(destR!=MPI::PROC_NULL) {
							bufSR[sBuf][j-1]=elems[i][j];
						}
						if(destD!=MPI::PROC_NULL) {
							bufSD[sBuf][i-1]=elems[i][j];
						}
					}
				}
				//Else if i+1==horTotal-2, then it ended during the red part amd we know the first collumn element is red
				else {
					int newI=i+1;
					//So we simply compute all the black elements and send them right
					for(j=2;j<verTotal-2;j+=2) {
						//Get the data from the receive buffers
						if(destR!=MPI::PROC_NULL) {
							elems[newI+1][j]=bufRR[prevBuf][j-1];
						}
						//Compute the new value
						double newVal=equation(omegaRed,elems,newI,j);
						if(checkIter) {
							diffSum=diffSum+((newVal-elems[newI][j])*(newVal-elems[newI][j]));
						}
						elems[newI][j]=newVal;
						//Put that value in the necessary send buffers
						if(destR!=MPI::PROC_NULL) {
							bufSR[sBuf][j-1]=elems[newI][j];
						}
					}
					//And down if necessary
					if(j==verTotal-2) {
						//Get the data from the receive buffers
						if(destR!=MPI::PROC_NULL) {
							elems[newI+1][j]=bufRR[prevBuf][j-1];
						}
						if(destD!=MPI::PROC_NULL) {
							elems[newI][j+1]=bufRD[prevBuf][newI-1];
						}
						//Compute the new value
						double newVal=equation(omegaRed,elems,newI,j);
						if(checkIter) {
							diffSum=diffSum+((newVal-elems[newI][j])*(newVal-elems[newI][j]));
						}
						elems[newI][j]=newVal;
						//Put that value in the necessary send buffers
						if(destR!=MPI::PROC_NULL) {
							bufSR[sBuf][j-1]=elems[newI][j];
						}
						if(destD!=MPI::PROC_NULL) {
							bufSD[sBuf][newI-1]=elems[newI][j];
						}
					}
				}
				
				/////////////MID-POINT-DATA-TRANSFER////////////////
				
				//TODO: Can I further improve this by making the send/receive buffers half their size?
				//Here we send and receive data so that we get the updated red and black edges
				//Send buffers should already be filled, so we just send them over. Same with receive buffers
				if(destR!=MPI::PROC_NULL) {
					reqSR[sBuf]=cartComm.Isend(bufSR[sBuf],verElements,MPI::DOUBLE,destR,BUFF_TAG+sBuf);
					reqRR[prevBuf]=cartComm.Irecv(bufRR[prevBuf],verElements,MPI::DOUBLE,destR,BUFF_TAG+prevBuf);
				}
				if(destL!=MPI::PROC_NULL) {
					reqSL[sBuf]=cartComm.Isend(bufSL[sBuf],verElements,MPI::DOUBLE,destL,BUFF_TAG+sBuf);
					reqRL[prevBuf]=cartComm.Irecv(bufRL[prevBuf],verElements,MPI::DOUBLE,destL,BUFF_TAG+prevBuf);
				}
				if(destU!=MPI::PROC_NULL) {
					reqSU[sBuf]=cartComm.Isend(bufSU[sBuf],horElements,MPI::DOUBLE,destU,BUFF_TAG+sBuf);
					reqRU[prevBuf]=cartComm.Irecv(bufRU[prevBuf],horElements,MPI::DOUBLE,destU,BUFF_TAG+prevBuf);
				}
				if(destD!=MPI::PROC_NULL) {
					reqSD[sBuf]=cartComm.Isend(bufSD[sBuf],horElements,MPI::DOUBLE,destD,BUFF_TAG+sBuf);
					reqRD[prevBuf]=cartComm.Irecv(bufRD[prevBuf],horElements,MPI::DOUBLE,destD,BUFF_TAG+prevBuf);
				}
				//Save the previous buffer and select the next buffer
				prevBuf=sBuf;
				sBuf=(sBuf+1)%BUFFERS;
				
				//Receive the data. Copying is unnecessary, since they'll be copied during the main loop
				//Make sure send buffers have been sent and we can safely write on them
				if(destR!=MPI::PROC_NULL) {
					reqRR[prevBuf].Wait();
					reqSR[sBuf].Wait();
				}
				if(destL!=MPI::PROC_NULL) {
					reqRL[prevBuf].Wait();
					reqSL[sBuf].Wait();
				}
				if(destU!=MPI::PROC_NULL) {
					reqRU[prevBuf].Wait();
					reqSU[sBuf].Wait();
				}
				if(destD!=MPI::PROC_NULL) {
					reqRD[prevBuf].Wait();
					reqSU[sBuf].Wait();
				}
				
				/////////////RED STARTS HERE//////////////
				
				//We start with the red elements
					//For the first element
						//Get the data from the receive buffers for the first element
				if(destL!=MPI::PROC_NULL) {
					elems[0][1]=bufRL[prevBuf][0];
				}
				if(destU!=MPI::PROC_NULL) {
					elems[1][0]=bufRU[prevBuf][0];
				}
						//Compute the new value for the first element
				double newVal=equation(omegaRed,elems,1,1);
				if(checkIter) {
					diffSum=diffSum+((newVal-elems[1][1])*(newVal-elems[1][1]));
				}
				elems[1][1]=newVal;
						//Put that value to the necessary send buffers
				if(destL!=MPI::PROC_NULL) {
					bufSL[sBuf][0]=elems[1][1];
				}
				if(destU!=MPI::PROC_NULL) {
					bufSU[sBuf][0]=elems[1][1];
				}
					//For the rest of the first collumn
				for(j=3;j<verTotal-2;j+=2) {
					//Get the data from the receive buffers
					if(destL!=MPI::PROC_NULL) {
						elems[0][j]=bufRL[prevBuf][j-1];
					}
					//Compute the new value
					newVal=equation(omegaRed,elems,1,j);
					if(checkIter) {
						diffSum=diffSum+((newVal-elems[1][j])*(newVal-elems[1][j]));
					}
					elems[1][j]=newVal;
					//Put that value in the necessary send buffers
					if(destL!=MPI::PROC_NULL) {
						bufSL[sBuf][j-1]=elems[1][j];
					}
				}
					//For the last element of the first collumn, if it is red
				if(j==verTotal-2) {
					//Get the data from the receive buffers
					if(destL!=MPI::PROC_NULL) {
						elems[0][j]=bufRL[prevBuf][j-1];
					}
					if(destD!=MPI::PROC_NULL) {
						elems[1][j+1]=bufRD[prevBuf][0];
					}
					//Compute the new value
					newVal=equation(omegaRed,elems,1,j);
					if(checkIter) {
						diffSum=diffSum+((newVal-elems[1][j])*(newVal-elems[1][j]));
					}
					elems[1][j]=newVal;
					//Put the value in the necessary send buffers
					if(destL!=MPI::PROC_NULL) {
						bufSL[sBuf][j-1]=elems[1][j];
					}
					if(destD!=MPI::PROC_NULL) {
						bufSD[sBuf][0]=elems[1][j];
					}
				}
					//For all collumns, except the end collumn
					//((A loop for the first black, a check for end condition with a break, then a loop for the first red))
				for(i=2;i<horTotal-2;i+=2) {
					//We know the first (elem[i][1]) is black, so we start with the red and compute them normally (no need to retrieve data)
					for(j=2;j<verTotal-2;j+=2) {
						//Compute the new value
						newVal=equation(omegaRed,elems,i,j);
						if(checkIter) {
							diffSum=diffSum+((newVal-elems[i][j])*(newVal-elems[i][j]));
						}
						elems[i][j]=newVal;
					}
					//If the last collumn element is red, check for bottom process
					if(j==verTotal-2) {
						if(destD!=MPI::PROC_NULL) {
							elems[i][j+1]=bufRD[prevBuf][i-1];
						}
						//Compute the new value
						newVal=equation(omegaRed,elems,i,j);
						if(checkIter) {
							diffSum=diffSum+((newVal-elems[i][j])*(newVal-elems[i][j]));
						}
						elems[i][j]=newVal;
						if(destD!=MPI::PROC_NULL) {
							bufSD[sBuf][i-1]=elems[i][j];
						}
					}
					//Check if we have reached the end 
					//(horTotal-1 is the edge collumn, so we don't want to compute that or we get segfaults when trying to access elements beyond the edge)
					//(horTotal-2 is the end collumn, and we don't want to compute that in the current loop because we want to send its results to the right)
					int newI=i+1;
					if(newI>=horTotal-2) {
						break;
					}
					
					//We know the next collumn is going to have the first element be red, so we need to send/receive for that
					if(destU!=MPI::PROC_NULL) {
						elems[newI][0]=bufRU[prevBuf][newI-1];
					}
							//Compute the new value for the first element
					double newVal=equation(omegaRed,elems,newI,1);
					if(checkIter) {
						diffSum=diffSum+((newVal-elems[newI][1])*(newVal-elems[newI][1]));
					}
					elems[newI][1]=newVal;
							//Put that value to the necessary send buffers
					if(destU!=MPI::PROC_NULL) {
						bufSU[sBuf][newI-1]=elems[newI][1];
					}
					//Then we loop through all the others normally
					for(j=3;j<verTotal-2;j+=2) {
						//Compute the new value
						newVal=equation(omegaRed,elems,newI,j);
						if(checkIter) {
							diffSum=diffSum+((newVal-elems[newI][j])*(newVal-elems[newI][j]));
						}
						elems[newI][j]=newVal;
					}
					//If the last collumn element is red, check for bottom process
					if(j==verTotal-2) {
						if(destD!=MPI::PROC_NULL) {
							elems[newI][j+1]=bufRD[prevBuf][newI-1];
						}
						//Compute the new value
						newVal=equation(omegaRed,elems,newI,j);
						if(checkIter) {
							diffSum=diffSum+((newVal-elems[newI][j])*(newVal-elems[newI][j]));
						}
						elems[newI][j]=newVal;
						if(destD!=MPI::PROC_NULL) {
							bufSD[sBuf][newI-1]=elems[newI][j];
						}
					}
				}
				//Now we check if the loop ended during the red or during the black part
				//If i==horTotal-2, then it ended during the black part and we know the first collumn element is black
				if(i==horTotal-2) {
					//So we simply compute all the elements and send them right
					for(j=2;j<verTotal-2;j+=2) {
						//Get the data from the receive buffers
						if(destR!=MPI::PROC_NULL) {
							elems[i+1][j]=bufRR[prevBuf][j-1];
						}
						//Compute the new value
						newVal=equation(omegaRed,elems,i,j);
						if(checkIter) {
							diffSum=diffSum+((newVal-elems[i][j])*(newVal-elems[i][j]));
						}
						elems[i][j]=newVal;
						//Put that value in the necessary send buffers
						if(destR!=MPI::PROC_NULL) {
							bufSR[sBuf][j-1]=elems[i][j];
						}
					}
					//And down if necessary
					if(j==verTotal-2) {
						//Get the data from the receive buffers
						if(destR!=MPI::PROC_NULL) {
							elems[i+1][j]=bufRR[prevBuf][j-1];
						}
						if(destD!=MPI::PROC_NULL) {
							elems[i][j+1]=bufRD[prevBuf][i-1];
						}
						//Compute the new value
						newVal=equation(omegaRed,elems,i,j);
						if(checkIter) {
							diffSum=diffSum+((newVal-elems[i][j])*(newVal-elems[i][j]));
						}
						elems[i][j]=newVal;
						//Put that value in the necessary send buffers
						if(destR!=MPI::PROC_NULL) {
							bufSR[sBuf][j-1]=elems[i][j];
						}
						if(destD!=MPI::PROC_NULL) {
							bufSD[sBuf][i-1]=elems[i][j];
						}
					}
				}
				//Else if i+1==horTotal-2, then it ended during the red part amd we know the first collumn element is red
				else {
					//So first we send the first element up and right
					int newI=i+1;
					if(destR!=MPI::PROC_NULL) {
						elems[newI+1][1]=bufRR[prevBuf][0];
					}
					if(destU!=MPI::PROC_NULL) {
						elems[newI][0]=bufRU[prevBuf][newI-1];
					}
					//Compute the new value
					newVal=equation(omegaRed,elems,newI,1);
					if(checkIter) {
						diffSum=diffSum+((newVal-elems[newI][1])*(newVal-elems[newI][1]));
					}
					elems[newI][1]=newVal;
					//Put that value in the necessary send buffers
					if(destR!=MPI::PROC_NULL) {
						bufSR[sBuf][0]=elems[newI][1];
					}
					if(destU!=MPI::PROC_NULL) {
						bufSU[sBuf][newI-1]=elems[newI][1];
					}
					//Then we compute the rest of the elements normally and send them right
					for(j=3;j<verTotal-2;j+=2) {
						//Get the data from the receive buffers
						if(destR!=MPI::PROC_NULL) {
							elems[newI+1][j]=bufRR[prevBuf][j-1];
						}
						//Compute the new value
						newVal=equation(omegaRed,elems,newI,j);
						if(checkIter) {
							diffSum=diffSum+((newVal-elems[newI][j])*(newVal-elems[newI][j]));
						}
						elems[newI][j]=newVal;
						//Put that value in the necessary send buffers
						if(destR!=MPI::PROC_NULL) {
							bufSR[sBuf][j-1]=elems[newI][j];
						}
					}
					//And send the final element down if necessary
					if(j==verTotal-2) {
						//Get the data from the receive buffers
						if(destR!=MPI::PROC_NULL) {
							elems[newI+1][j]=bufRR[prevBuf][j-1];
						}
						if(destD!=MPI::PROC_NULL) {
							elems[newI][j+1]=bufRD[prevBuf][newI-1];
						}
						//Compute the new value
						newVal=equation(omegaRed,elems,newI,j);
						if(checkIter) {
							diffSum=diffSum+((newVal-elems[newI][j])*(newVal-elems[newI][j]));
						}
						elems[newI][j]=newVal;
						//Put that value in the necessary send buffers
						if(destR!=MPI::PROC_NULL) {
							bufSR[sBuf][j-1]=elems[newI][j];
						}
						if(destD!=MPI::PROC_NULL) {
							bufSD[sBuf][newI-1]=elems[newI][j];
						}
					}
				}
			}
		#endif
		
		#ifndef SINGLE_PROC
			//Make a reduce request, now that we have all the data
			double allSum;
			if(checkIter) {
				MPI_Iallreduce(&diffSum,&allSum,1,MPI::DOUBLE,MPI::SUM,cartComm,&reqSum);
			}
		#endif
		
		#ifndef SINGLE_PROC
			#ifndef MULTI_BUFFER
				//Finally, wait on the send buffers, copy new data there and 
				//make the buffers availiable to the other processes 
				if(destR!=MPI::PROC_NULL) {
					reqSR.Wait();
					for(int j=1;j<verTotal-1;++j) {
						bufSR[j-1]=elems[horTotal-2][j];
					}
					reqSR=cartComm.Isend(bufSR,verElements,MPI::DOUBLE,destR,BUFF_TAG);
				}
				if(destL!=MPI::PROC_NULL) {
					reqSL.Wait();
					for(int j=1;j<verTotal-1;++j) {
						bufSL[j-1]=elems[1][j];
					}
					reqSL=cartComm.Isend(bufSL,verElements,MPI::DOUBLE,destL,BUFF_TAG);
				}
				if(destU!=MPI::PROC_NULL) {
					reqSU.Wait();
					for(int i=1;i<horTotal-1;++i) {
						bufSU[i-1]=elems[i][1];
					}
					reqSU=cartComm.Isend(bufSU,horElements,MPI::DOUBLE,destU,BUFF_TAG);
				}
				if(destD!=MPI::PROC_NULL) {
					reqSD.Wait();
					for(int i=1;i<horTotal-1;++i) {
						bufSD[i-1]=elems[i][verTotal-2];
					}
					reqSD=cartComm.Isend(bufSD,horElements,MPI::DOUBLE,destD,BUFF_TAG);
				}
			#else //If we are using multiple buffers
				//Send buffers should already be filled, so we just send them over. Same with receive buffers
				if(destR!=MPI::PROC_NULL) {
					reqSR[sBuf]=cartComm.Isend(bufSR[sBuf],verElements,MPI::DOUBLE,destR,BUFF_TAG+sBuf);
					reqRR[prevBuf]=cartComm.Irecv(bufRR[prevBuf],verElements,MPI::DOUBLE,destR,BUFF_TAG+prevBuf);
				}
				if(destL!=MPI::PROC_NULL) {
					reqSL[sBuf]=cartComm.Isend(bufSL[sBuf],verElements,MPI::DOUBLE,destL,BUFF_TAG+sBuf);
					reqRL[prevBuf]=cartComm.Irecv(bufRL[prevBuf],verElements,MPI::DOUBLE,destL,BUFF_TAG+prevBuf);
				}
				if(destU!=MPI::PROC_NULL) {
					reqSU[sBuf]=cartComm.Isend(bufSU[sBuf],horElements,MPI::DOUBLE,destU,BUFF_TAG+sBuf);
					reqRU[prevBuf]=cartComm.Irecv(bufRU[prevBuf],horElements,MPI::DOUBLE,destU,BUFF_TAG+prevBuf);
				}
				if(destD!=MPI::PROC_NULL) {
					reqSD[sBuf]=cartComm.Isend(bufSD[sBuf],horElements,MPI::DOUBLE,destD,BUFF_TAG+sBuf);
					reqRD[prevBuf]=cartComm.Irecv(bufRD[prevBuf],horElements,MPI::DOUBLE,destD,BUFF_TAG+prevBuf);
				}
			#endif
		#endif
		
		#ifndef SINGLE_PROC
			//Each ITER_CHECK times, check for the sum and exit if we have reached the required level of convergence
			if(checkIter) {
				MPI_Wait(&reqSum,MPI_STATUS_IGNORE);
				if(sqrt(allSum)<epsilon) {
					break;
				}
				#ifndef NDEBUG
					if(tid==0) {
						cout<<"|"<<diffSum<<"| !"<<allSum<<"!"<<endl;
					}
					else {
						cout<<"|"<<diffSum<<"|"<<endl;
					}
				#endif
			}
		#else
			if(checkIter && sqrt(diffSum)<epsilon) {
				break;
			}
			#ifndef NDEBUG
				if(checkIter) {
					cout<<diffSum<<endl;
				}
			#endif
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
		#ifndef MULTI_BUFFER
			//Destroy communication buffers if necessary
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
		#else //If we are using multiple buffers
			for(int i=0;i<BUFFERS;++i) {
				//Destroy communication buffers if necessary
				if(destR!=MPI::PROC_NULL) {
					delete[] bufSR[i];
					delete[] bufRR[i];
				}
				if(destL!=MPI::PROC_NULL) {
					delete[] bufSL[i];
					delete[] bufRL[i];
				}
				if(destU!=MPI::PROC_NULL) {
					delete[] bufSU[i];
					delete[] bufRU[i];
				}
				if(destD!=MPI::PROC_NULL) {
					delete[] bufSD[i];
					delete[] bufRD[i];
				}
			}
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
		#endif
		
		//Finalize the MPI environment
		MPI::Finalize();
	#endif
	
	//Exit
	return EXIT_SUCCESS;
}


inline double equation(double omega, double** elems, int i, int j) {
	#ifndef NDEBUG
		//cout<<i<<','<<j<<endl;
		//cin.ignore();
	#endif
	return (1.0-omega)*(elems[i][j])+(omega/4.0)*(elems[i-1][j]+elems[i][j-1]+elems[i][j+1]+elems[i+1][j]);
}

//TODO: Make inline functions for writing/reading to/from send/receive buffers during multi-buffer mode. Something like SendU,SendD,etc with (elems,buff,sBuf/prevBuf,i,j). Or make a single inline function and use defines to make the left/right/up/down/etc.

