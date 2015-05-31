//#define NDEBUG

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
	#define ITER_CHECK 500 //The program will check if we have converged every ITER_CHECK iterations 
#endif

#ifndef sqrtN
	#define sqrtN 200 //The squre root of the number of elements in the grid
#endif

#ifndef epsilon
	#define epsilon 1e-10 //When the total change occuring in the entire array is less than epsilon, the program stops solving
#endif

#ifndef qDef
	//#define qDef sqrt(nthreads)/4
	//#define qDef sqrt(nthreads)/2
	#define qDef sqrt(nthreads)
	//#define qDef sqrt(nthreads)*2
	//#define qDef sqrt(nthreads)*4
#endif

#ifndef omegaRed
	//#define omegaRed 0.1
	//#define omegaRed 0.2
	//...
	//#define omegaRed 1.8
	#define omegaRed 1.9
#endif

#ifndef omegaBlack
	//#define omegaBlack 0.1
	//#define omegaBlack 0.2
	//...
	//#define omegaBlack 1.8
	#define omegaBlack 1.9
#endif

#define INIT_TAG 1 //Used to send initialisation data
#define BUFF_TAG 2 //Used to send data from block to block. In multibuffer mode, BUFF_TAG+<buffer number> is sent used as the tag

//#define MULTI_BUFFER

#ifdef MULTI_BUFFER
	#ifndef BUFFERS
		#define BUFFERS 4
	#endif
	#define BUFF_NUM (BUFFERS*2)
#endif

#ifdef __GNUC__
	#define likely(x)       __builtin_expect((x), true)
	#define unlikely(x)     __builtin_expect((x), false)
#else
	#define likely(x)       (x)
	#define unlikely(x)     (x)
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

#ifdef MULTI_BUFFER

	#define getFromRecRight(y,k) getFromRecLR(elems,bufRR,prevBuf,horTotal-2,y,+1,hasRight,k)
	#define getFromRecLeft(y,k) getFromRecLR(elems,bufRL,prevBuf,1,y,-1,hasLeft,k)
	#define getFromRecUp(x,k) getFromRecUD(elems,bufRU,prevBuf,x,1,-1,hasUp,k)
	#define getFromRecDown(x,k) getFromRecUD(elems,bufRD,prevBuf,x,verTotal-2,+1,hasDown,k)

	#define writeToSendRight(y,k) writeToSendLR(elems,bufSR,sBuf,horTotal-2,y,hasRight,k)
	#define writeToSendLeft(y,k) writeToSendLR(elems,bufSL,sBuf,1,y,hasLeft,k)
	#define writeToSendUp(x,k) writeToSendUD(elems,bufSU,sBuf,x,1,hasUp,k)
	#define writeToSendDown(x,k) writeToSendUD(elems,bufSD,sBuf,x,verTotal-2,hasDown,k)

	inline void writeToSendLR(double ** elems, double** buff, int sBuf, int i, int j, bool destExists, int k);

	inline void writeToSendUD(double ** elems, double** buff, int sBuf, int i, int j, bool destExists, int k);

	inline void getFromRecLR(double ** elems, double** buff, int prevBuf, int i, int j, int offsX, bool destExists, int k);

	inline void getFromRecUD(double ** elems, double** buff, int prevBuf, int i, int j, int offsY, bool destExists, int k);
#endif

inline void proccessData(double ** elems, double& diffSum, double omega, bool checkIter, int i, int j);

void deleteElements(double ** elems, int horTotal);

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
	/*#ifndef SINGLE_PROC
		int horDeficit=0; //How many more horizontal elements this has compared to other zones
		int verDeficit=0; //How mant more vertical elements this has compared to other zones
	#endif*/
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
			//Red buffers
			double* bufSLR;
			double* bufRLR;
			double* bufSRR;
			double* bufRRR;
			double* bufSDR;
			double* bufRDR;
			double* bufSUR;
			double* bufRUR;
			//Black buffers
			double* bufSLB;
			double* bufRLB;
			double* bufSRB;
			double* bufRRB;
			double* bufSDB;
			double* bufRDB;
			double* bufSUB;
			double* bufRUB;
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
			//Red buffers
			MPI::Request reqSLR=MPI::REQUEST_NULL;
			MPI::Request reqRLR=MPI::REQUEST_NULL;
			MPI::Request reqSUR=MPI::REQUEST_NULL;
			MPI::Request reqRUR=MPI::REQUEST_NULL;
			MPI::Request reqSDR=MPI::REQUEST_NULL;
			MPI::Request reqRDR=MPI::REQUEST_NULL;
			MPI::Request reqSRR=MPI::REQUEST_NULL;
			MPI::Request reqRRR=MPI::REQUEST_NULL;
			//Black buffers
			MPI::Request reqSLB=MPI::REQUEST_NULL;
			MPI::Request reqRLB=MPI::REQUEST_NULL;
			MPI::Request reqSUB=MPI::REQUEST_NULL;
			MPI::Request reqRUB=MPI::REQUEST_NULL;
			MPI::Request reqSDB=MPI::REQUEST_NULL;
			MPI::Request reqRDB=MPI::REQUEST_NULL;
			MPI::Request reqSRB=MPI::REQUEST_NULL;
			MPI::Request reqRRB=MPI::REQUEST_NULL;
		#else //If we're using multiple buffers
			short sBuf=0; //Holds the currently selected buffer. (odd for black, even for red)
		
			MPI::Request reqSL[BUFF_NUM];
			MPI::Request reqRL[BUFF_NUM];
			MPI::Request reqSU[BUFF_NUM];
			MPI::Request reqRU[BUFF_NUM];
			MPI::Request reqSD[BUFF_NUM];
			MPI::Request reqRD[BUFF_NUM];
			MPI::Request reqSR[BUFF_NUM];
			MPI::Request reqRR[BUFF_NUM];
			for(int i=0;i<BUFF_NUM;++i) {
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
		
		bool hasLeft=(destL!=MPI::PROC_NULL);
		bool hasRight=(destR!=MPI::PROC_NULL);
		bool hasUp=(destU!=MPI::PROC_NULL);
		bool hasDown=(destD!=MPI::PROC_NULL);
	#endif //End ifndef SINGLE_PROC
	
	#ifdef SINGLE_PROC
		horElements=sqrtN;
		verElements=sqrtN;
	#endif
	
	#ifndef SINGLE_PROC
		//Find out how many elements this zone should have
		if(sqrtN%horZones<=1 || ((sqrtN/horZones)-(horZones-1))<2) {
			horElements = sqrtN/horZones;
		} else {
			horElements = (sqrtN/horZones) + 1;
		}
		if(sqrtN%verZones<=1 || ((sqrtN/verZones)-(verZones-1))<2) {
			verElements = sqrtN/verZones;
		} else {
			verElements = (sqrtN/verZones) + 1;
		}
		
		
		//If this is in the right/bottom edge (which we can tell since we have no right/bottom neighbour)
		//Find how many more elements we need and add them to the number of elements of this zone
		if(!hasRight) {
			if(sqrtN%horZones>1 && ((sqrtN/horZones)-(horZones-1))>=2) {
				horElements=(sqrtN/horZones)-(horZones-1);
			} else {
				horElements+=sqrtN%horZones;
			}
		}
		if(!hasDown) {
			if(sqrtN%verZones>1 && ((sqrtN/verZones)-(verZones-1))>=2) {
				verElements=(sqrtN/verZones)-(verZones-1);
			} else {
				verElements+=sqrtN%verZones;
			}
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
		if(!hasRight) {
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
		//Compute if the upper left element (element, NOT edge) of this block is red or black
		//then use that to tell the next block
		//First we receive data from the previous block, if it exists
		if(hasUp) {
			cartComm.Recv(&isFirstElemRed,1,MPI::BOOL,destU,INIT_TAG);
		}
		if(hasLeft) {
			cartComm.Recv(&isFirstElemRed,1,MPI::BOOL,destL,INIT_TAG);
		}
		//If the previous block doesn't exist, then we are at the upper left block and we
		//know the upper left block starts red
		if(!hasUp && !hasLeft) {
			isFirstElemRed=true;
		}
		//Send data to the next block if it exists
		if(hasDown) {
			//verElements%2==0 when the first element is red and the next block's first element is also red
			//So if isFirstElemRed==true and (verElements%2==0)==true then isNextFirstElemRed==true
			//if isFirstElemRed==true and (verElements%2==0)==false then isNextFirstElemRed==false
			//and so on
			bool isNextFirstElemRed=(((verElements%2)==0)==isFirstElemRed);
			cartComm.Send(&isNextFirstElemRed,1,MPI::BOOL,destD,INIT_TAG);
		}
		if(hasRight) {
			bool isNextFirstElemRed=(((horElements%2)==0)==isFirstElemRed);
			cartComm.Send(&isNextFirstElemRed,1,MPI::BOOL,destR,INIT_TAG);
		}
	#else
		isFirstElemRed=true;
	#endif
	
	bool isBottomLeftElementRed;
	bool isTopRightElementRed;
	bool isBottomRightElementRed;
	if(isFirstElemRed) {
		//If the first element (top left, elems[1][1]) is red, then all odd elements below it are red
		//Therefore the bottom left element is red if it is odd
		isBottomLeftElementRed=(((verTotal-2)%2)==1);
		//Same for the horizontal
		isTopRightElementRed=(((horTotal-2)%2)==1);
		//If the top right element is red and the bottom left element is red, then the bottom right element is also red
		//If the top right element is black and the bottom left element is red, then the bottom right element is black
		//If the top right element is red and the bottom left element is black, then the bottom right element is black
		//If the top right element is black and the bottom left element is black, then the bottom right element is red
		//So we need them to be equal for the bottom right to be red
		isBottomRightElementRed=(isBottomLeftElementRed==isTopRightElementRed);
	} else {
		//If the second element (top left, elems[1][2]) is red, then all even elements below it are red
		isBottomLeftElementRed=(((verTotal-2)%2)==0);
		//Same for the horizontal
		isTopRightElementRed=(((horTotal-2)%2)==0);
		//If the top right element is red and the bottom left element is red, then the bottom right element is black
		//If the top right element is red and the bottom left element is black, then the bottom right element is red
		//If the top right element is black and the bottom left element is red, then the bottom right element is red
		//If the top right element is black and the bottom left element is black, then the bottom right element is black
		//So we need them to be unequal for the bottom right to be red
		isBottomRightElementRed=(isBottomLeftElementRed!=isTopRightElementRed);
	}
	
	#ifndef SINGLE_PROC
		//Find the number of red and black elements at the end (NOT the edges!)
		int redElemsHorUp;
		int redElemsHorDown;
		int redElemsVerLeft;
		int redElemsVerRight;
		int blackElemsHorUp;
		int blackElemsHorDown;
		int blackElemsVerLeft;
		int blackElemsVerRight;
		if(isFirstElemRed) {
			redElemsHorUp=horElements/2+horElements%2;
			blackElemsHorUp=horElements/2;
			redElemsVerLeft=verElements/2+verElements%2;
			blackElemsVerLeft=verElements/2;
		} 
		else {
			redElemsHorUp=horElements/2;
			blackElemsHorUp=horElements/2+horElements%2;
			redElemsVerLeft=verElements/2;
			blackElemsVerLeft=verElements/2+verElements%2;
		}
		if(isTopRightElementRed) {
			redElemsVerRight=verElements/2+verElements%2;
			blackElemsVerRight=verElements/2;
		}
		else {
			redElemsVerRight=verElements/2;
			blackElemsVerRight=verElements/2+verElements%2;
		}
		if(isBottomLeftElementRed) {
			redElemsHorDown=horElements/2+horElements%2;
			blackElemsHorDown=horElements/2;
		}
		else {
			redElemsHorDown=horElements/2;
			blackElemsHorDown=horElements/2+horElements%2;
		}
		#ifndef MULTI_BUFFER
			//Allocate communication buffers if necessary
			if(hasRight) {
				//The buffer is half the size because we're only taking the elements we need: half red, half black
				bufSRR=new double[redElemsVerRight](); //() means initialize to 0
				bufRRR=new double[blackElemsVerRight](); //() means initialize to 0
				bufSRB=new double[blackElemsVerRight](); //() means initialize to 0
				bufRRB=new double[redElemsVerRight](); //() means initialize to 0
			}
			if(hasLeft) {
				bufSLR=new double[redElemsVerLeft](); //() means initialize to 0
				bufRLR=new double[blackElemsVerLeft](); //() means initialize to 0
				bufSLB=new double[blackElemsVerLeft](); //() means initialize to 0
				bufRLB=new double[redElemsVerLeft](); //() means initialize to 0
			}
			if(hasUp) {
				bufSUR=new double[redElemsHorUp](); //() means initialize to 0
				bufRUR=new double[blackElemsHorUp](); //() means initialize to 0
				bufSUB=new double[blackElemsHorUp](); //() means initialize to 0
				bufRUB=new double[redElemsHorUp](); //() means initialize to 0
			}
			if(hasDown) {
				bufSDR=new double[redElemsHorDown](); //() means initialize to 0
				bufRDR=new double[blackElemsHorDown](); //() means initialize to 0
				bufSDB=new double[blackElemsHorDown](); //() means initialize to 0
				bufRDB=new double[redElemsHorDown](); //() means initialize to 0
			}
		#else //If we're using multiple buffers
			//Allocate communication buffers if necessary
			if(hasRight) {
				bufSR=new double*[BUFF_NUM];
				bufRR=new double*[BUFF_NUM];
			}
			if(hasLeft) {
				bufSL=new double*[BUFF_NUM];
				bufRL=new double*[BUFF_NUM];
			}
			if(hasUp) {
				bufSU=new double*[BUFF_NUM];
				bufRU=new double*[BUFF_NUM];
			}
			if(hasDown) {
				bufSD=new double*[BUFF_NUM];
				bufRD=new double*[BUFF_NUM];
			}
			for(int i = 0; i<BUFF_NUM; i+=2) {
				//Odd for black, even for red. We know we'll always have an even number of buffers
				if(hasRight) {
					//The buffer is half the size because we're only taking the elements we need: half red, half black
					bufSR[i]=new double[blackElemsVerRight](); //() means initialize to 0
					bufRR[i]=new double[redElemsVerRight](); //() means initialize to 0
					bufSR[i+1]=new double[redElemsVerRight](); //() means initialize to 0
					bufRR[i+1]=new double[blackElemsVerRight](); //() means initialize to 0
				}
				if(hasLeft) {
					bufSL[i]=new double[blackElemsVerLeft](); //() means initialize to 0
					bufRL[i]=new double[redElemsVerLeft](); //() means initialize to 0
					bufSL[i+1]=new double[redElemsVerLeft](); //() means initialize to 0
					bufRL[i+1]=new double[blackElemsVerLeft](); //() means initialize to 0
				}
				if(hasUp) {
					bufSU[i]=new double[blackElemsHorUp](); //() means initialize to 0
					bufRU[i]=new double[redElemsHorUp](); //() means initialize to 0
					bufSU[i+1]=new double[redElemsHorUp](); //() means initialize to 0
					bufRU[i+1]=new double[blackElemsHorUp](); //() means initialize to 0
				}
				if(hasDown) {
					bufSD[i]=new double[blackElemsHorDown](); //() means initialize to 0
					bufRD[i]=new double[redElemsHorDown](); //() means initialize to 0
					bufSD[i+1]=new double[redElemsHorDown](); //() means initialize to 0
					bufRD[i+1]=new double[blackElemsHorDown](); //() means initialize to 0
				}
			}
			
			//For all receive buffers other than the first, make a recv request
			//Imagine we have another processes. When the other process sends something with its sBuf==0, we'll receive it in our sBuf==1 (because the 0th request is NULL)
			//When the other process sends with its sBuf==1, we'll receive it in our sBuf==2, and so on
			//That means that we will always get the correct data and not the previous or next data. The BUFF_TAG+i helps us further ensure that
			for(int i=1;i<BUFF_NUM;++i) {
				//Remember, odd for black, even for red
				if(i%2==0) {
					if(hasRight) {
						reqRR[i]=cartComm.Irecv(bufRR[i],redElemsVerRight,MPI::DOUBLE,destR,BUFF_TAG+i);
					}
					if(hasLeft) {
						reqRL[i]=cartComm.Irecv(bufRL[i],redElemsVerLeft,MPI::DOUBLE,destL,BUFF_TAG+i);
					}
					if(hasUp) {
						reqRU[i]=cartComm.Irecv(bufRU[i],redElemsHorUp,MPI::DOUBLE,destU,BUFF_TAG+i);
					}
					if(hasDown) {
						reqRD[i]=cartComm.Irecv(bufRD[i],redElemsHorDown,MPI::DOUBLE,destD,BUFF_TAG+i);
					}
				} else {
					if(hasRight) {
						reqRR[i]=cartComm.Irecv(bufRR[i],blackElemsVerRight,MPI::DOUBLE,destR,BUFF_TAG+i);
					}
					if(hasLeft) {
						reqRL[i]=cartComm.Irecv(bufRL[i],blackElemsVerLeft,MPI::DOUBLE,destL,BUFF_TAG+i);
					}
					if(hasUp) {
						reqRU[i]=cartComm.Irecv(bufRU[i],blackElemsHorUp,MPI::DOUBLE,destU,BUFF_TAG+i);
					}
					if(hasDown) {
						reqRD[i]=cartComm.Irecv(bufRD[i],blackElemsHorDown,MPI::DOUBLE,destD,BUFF_TAG+i);
					}
				}
			}
		#endif
	#endif
	
	//Used for optimisation
	int isFirstElemRedIndex=(isFirstElemRed)?(1):(2);
	int isFirstElemRedIndexInverse=(isFirstElemRed)?(2):(1);
	int isBottomLeftElementRedIndex=(isBottomLeftElementRed)?(1):(2);
	int isBottomLeftElementRedIndexInverse=(isBottomLeftElementRed)?(2):(1);
	int isTopRightElementRedIndex=(isTopRightElementRed)?(1):(2);
	int isTopRightElementRedIndexInverse=(isTopRightElementRed)?(2):(1);
	int isFirstElemRedKappa=isFirstElemRedIndex-1;
	int isFirstElemRedKappaInverse=isFirstElemRedIndexInverse-1;
	int isBottomLeftElementRedKappa=(isBottomLeftElementRed)?(1):(0);
	int isBottomLeftElementRedKappaInverse=(isBottomLeftElementRed)?(0):(1);
	int isTopRightElementRedKappa=(isTopRightElementRed)?(1):(0);
	int isTopRightElementRedKappaInverse=(isTopRightElementRed)?(0):(1);
	
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
		#ifndef MULTI_BUFF
			if(hasRight) {
					reqRRR=cartComm.Irecv(bufRRR,blackElemsVerRight,MPI::DOUBLE,destR,BUFF_TAG);
				}
				if(hasLeft) {
					reqRLR=cartComm.Irecv(bufRLR,blackElemsVerLeft,MPI::DOUBLE,destL,BUFF_TAG);
				}
				if(hasUp) {
					reqRUR=cartComm.Irecv(bufRUR,blackElemsHorUp,MPI::DOUBLE,destU,BUFF_TAG);
				}
				if(hasDown) {
					reqRDR=cartComm.Irecv(bufRDR,blackElemsHorDown,MPI::DOUBLE,destD,BUFF_TAG);
				}
		#endif
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
		if(unlikely(checkIter)) {
			diffSum=0.0;
		}
		
		#ifndef SINGLE_PROC
			#ifndef MULTI_BUFFER
				//wait for the data from the appropriate receive buffer each time the looop starts 
				//and copy them to the edge. Then send a request for the next batch of edges 
				//(that will complete on the next loop)
				//First we wait on the black data
				if(hasRight) {
					reqRRB.Wait();
					//We want only the black elements. So if the top right element is red, 
					//then we know that the edge element on its right it will be black
					//and we start with it, else we start with the one below it which will be red
					//(It's verTotal-1 because we don't care about the corners)
					{int k,j; //Workaround for inability to declare multiple counters
					for(k=0,j=isTopRightElementRedIndex;j<verTotal-1;++k,j+=2) {
						elems[horTotal-1][j]=bufRRB[k];
					}}
					reqRRB=cartComm.Irecv(bufRRB,redElemsVerRight,MPI::DOUBLE,destR,BUFF_TAG+1); //+1 for black, +0 for red
				}
				if(hasLeft) {
					reqRLB.Wait();
					{int k,j;
					for(k=0,j=isFirstElemRedIndex;j<verTotal-1;++k,j+=2) {
						elems[0][j]=bufRLB[k];
					}}
					reqRLB=cartComm.Irecv(bufRLB,redElemsVerLeft,MPI::DOUBLE,destL,BUFF_TAG+1);
				}
				if(hasUp) {
					reqRUB.Wait();
					{int k,i;
					for(k=0,i=isFirstElemRedIndex;i<horTotal-1;++k,i+=2) {
						elems[i][0]=bufRUB[k];
					}}
					reqRUB=cartComm.Irecv(bufRUB,redElemsHorUp,MPI::DOUBLE,destU,BUFF_TAG+1);
				}
				if(hasDown) {
					reqRDB.Wait();
					{int k,i;
					for(k=0,i=isBottomLeftElementRedIndex;i<horTotal-1;++k,i+=2) {
						elems[i][verTotal-1]=bufRDB[k];
					}}
					reqRDB=cartComm.Irecv(bufRDB,redElemsHorDown,MPI::DOUBLE,destD,BUFF_TAG+1);
				}
			#else //If we are not a single process and are using multiple buffers
				//Save the previous buffer and select the next buffer
				short prevBuf=sBuf;
				sBuf=(sBuf+1)%BUFF_NUM;
				
				//Receive the data. Copying is unnecessary, since they'll be copied during the main loop
				//Make sure send buffers have been sent and we can safely write on them
				if(hasRight) {
					reqRR[prevBuf].Wait();
					reqSR[sBuf].Wait();
				}
				if(hasLeft) {
					reqRL[prevBuf].Wait();
					reqSL[sBuf].Wait();
				}
				if(hasUp) {
					reqRU[prevBuf].Wait();
					reqSU[sBuf].Wait();
				}
				if(hasDown) {
					reqRD[prevBuf].Wait();
					reqSD[sBuf].Wait();
				}
			#endif
		#endif
		
		#if defined(SINGLE_PROC) || !defined(MULTI_BUFFER) //If we are a single process or are not using multiple buffers
			//Compute all red
			for(int i=1;i<horTotal-1;++i) {
				for(int j=((isFirstElemRed)?((i+1)%2):(i%2))+1;j<verTotal-1;j+=2) {
					proccessData(elems,diffSum,omegaRed,checkIter,i,j);
				}
			}
			
			//Exchange data and get the new red elements
			#ifndef SINGLE_PROC
				//Finally, wait on the send buffers, copy new data there and 
				//make the buffers availiable to the other processes 
				//We send the red ones, since the others are going to need them to compute the black ones
				if(hasRight) {
					reqSRR.Wait();
					{int k,j;
					for(k=0,j=isTopRightElementRedIndex;j<verTotal-1;++k,j+=2) {
						bufSRR[k]=elems[horTotal-2][j];
					}}
					reqSRR=cartComm.Isend(bufSRR,redElemsVerRight,MPI::DOUBLE,destR,BUFF_TAG);
				}
				if(hasLeft) {
					reqSLR.Wait();
					{int k,j;
					for(k=0,j=isFirstElemRedIndex;j<verTotal-1;++k,j+=2) {
						bufSLR[k]=elems[1][j];
					}}
					reqSLR=cartComm.Isend(bufSLR,redElemsVerLeft,MPI::DOUBLE,destL,BUFF_TAG);
				}
				if(hasUp) {
					reqSUR.Wait();
					{int k,i;
					for(k=0,i=isFirstElemRedIndex;i<horTotal-1;++k,i+=2) {
						bufSUR[k]=elems[i][1];
					}}
					reqSUR=cartComm.Isend(bufSUR,redElemsHorUp,MPI::DOUBLE,destU,BUFF_TAG);
				}
				if(hasDown) {
					reqSDR.Wait();
					{int k,i;
					for(k=0,i=isBottomLeftElementRedIndex;i<horTotal-1;++k,i+=2) {
						bufSDR[k]=elems[i][verTotal-2];
					}}
					reqSDR=cartComm.Isend(bufSDR,redElemsHorDown,MPI::DOUBLE,destD,BUFF_TAG);
				}
				//wait for the data from the appropriate receive buffer
				//and copy them to the edge. Then send a request for the next batch of edges 
				//(that will complete on the next loop)
				//Now we want the red ones the others have computed, since we'll use them to do the black ones
				if(hasRight) {
					reqRRR.Wait();
					//We want only the red elements. So if the top right element is red, 
					//we know that the edge element on its right is going to be black, 
					//so we start with the one below it, else we start with it
					//(It's verTotal-1 because we don't care about the corners)
					{int k,j;
					for(k=0,j=isTopRightElementRedIndexInverse;j<verTotal-1;++k,j+=2) {
						elems[horTotal-1][j]=bufRRR[k];
					}}
					reqRRR=cartComm.Irecv(bufRRR,blackElemsVerRight,MPI::DOUBLE,destR,BUFF_TAG);
				}
				if(hasLeft) {
					reqRLR.Wait();
					{int k,j;
					for(k=0,j=isFirstElemRedIndexInverse;j<verTotal-1;++k,j+=2) {
						elems[0][j]=bufRLR[k];
					}}
					reqRLR=cartComm.Irecv(bufRLR,blackElemsVerLeft,MPI::DOUBLE,destL,BUFF_TAG);
				}
				if(hasUp) {
					reqRUR.Wait();
					{int k,i;
					for(k=0,i=isFirstElemRedIndexInverse;i<horTotal-1;++k,i+=2) {
						elems[i][0]=bufRUR[k];
					}}
					reqRUR=cartComm.Irecv(bufRUR,blackElemsHorUp,MPI::DOUBLE,destU,BUFF_TAG);
				}
				if(hasDown) {
					reqRDR.Wait();
					{int k,i;
					for(k=0,i=isBottomLeftElementRedIndexInverse;i<horTotal-1;++k,i+=2) {
						elems[i][verTotal-1]=bufRDR[k];
					}}
					reqRDR=cartComm.Irecv(bufRDR,blackElemsHorDown,MPI::DOUBLE,destD,BUFF_TAG);
				}
			#endif
			
			//Do the same as above but for black
			for(int i=1;i<horTotal-1;++i) {
				for(int j=((isFirstElemRed)?(i%2):((i+1)%2))+1;j<verTotal-1;j+=2) {
					proccessData(elems,diffSum,omegaBlack,checkIter,i,j);
				}
			}
		#else //If we are not a single process and are using multiple buffers
			//If the first element is red
			if(isFirstElemRed) {
				/////////////RED STARTS HERE//////////////
				
				//We start with the red elements
					//For the first element
						//Get the data from the receive buffers for the first element
				getFromRecLeft(1,0);
				getFromRecUp(1,0);
						//Compute the new value for the first element
				proccessData(elems,diffSum,omegaRed,checkIter,1,1);
				
						//Put that value to the necessary send buffers
				writeToSendLeft(1,0);
				writeToSendUp(1,0);
				
					//For the rest of the first collumn, get the data from the left, 
					//compute the new value and put the new data left
				{int kVer,j;
				for(kVer=1,j=3;j<verTotal-2;++kVer,j+=2) {
					getFromRecLeft(j,kVer);
					proccessData(elems,diffSum,omegaRed,checkIter,1,j);
					writeToSendLeft(j,kVer);
				}}
					//For the last element of the first collumn, if it is red
				if(isBottomLeftElementRed) {
					getFromRecLeft(verTotal-2,redElemsVerLeft-1);
					getFromRecDown(1,0);
					
					proccessData(elems,diffSum,omegaRed,checkIter,1,verTotal-2);
					
					writeToSendLeft(verTotal-2,redElemsVerLeft-1);
					writeToSendDown(1,0);
				}
					//For all collumns, except the end collumn
				{int kTop,kBot,i;
				for(kBot=isBottomLeftElementRedKappa, //Returns 1 for true, 0 for false. If it is true, we have already put the bottom left element in 0, so we need 1
						kTop=1,i=2;i<horTotal-2;i+=2,
						++kTop,++kBot) {
					//We know the first (elems[i][1]) is black, so we start with the red (elems[i][2]) and compute them normally (no need to retrieve data)
					for(int j=2;j<verTotal-2;j+=2) {
						proccessData(elems,diffSum,omegaRed,checkIter,i,j);
					}
					
					//Check if we have reached the end 
					//(horTotal-1 is the edge collumn, so we don't want to compute that or we get segfaults when trying to access elements beyond the edge)
					//(horTotal-2 is the end collumn, and we don't want to compute that in the current loop because we want to send its results to the right)
					int newI=i+1;
					if(unlikely(newI>=horTotal-2)) {
						break;
					}
					
					//We know the next collumn is going to have the first element be red, so we need to send/receive for that
					getFromRecUp(newI,kTop);
					//Compute the new value for the first element
					proccessData(elems,diffSum,omegaRed,checkIter,newI,1);
					//Put that value to the necessary send buffers
					writeToSendUp(newI,kTop);
					
					//Then we loop through all the others normally
					for(int j=3;j<verTotal-2;j+=2) {
						proccessData(elems,diffSum,omegaRed,checkIter,newI,j);
					}
					
					//If the last collumn element is red, check for bottom process
					//(If the bottom left element is red, then the one to the right of it is going to be black.
					//So if the bottom left element is black, then the one to the right of it is going to be red.
					if(!isBottomLeftElementRed) {
						getFromRecDown(i,kBot);
						proccessData(elems,diffSum,omegaRed,checkIter,i,verTotal-2);
						writeToSendDown(i,kBot);
					}
					//If the last collumn element is red, check for bottom process
					//(If the bottom left element is red, then the element that is 2 spaces right of it is going to be red
					else { //if(isBottomLeftElementRed)
						getFromRecDown(newI,kBot);
						proccessData(elems,diffSum,omegaRed,checkIter,newI,verTotal-2);
						writeToSendDown(newI,kBot);
					}
				}}
				//Now we do the final collumn. If its first element is red
				if(isTopRightElementRed) {
					//We compute the first element and send it up and right
					getFromRecUp(horTotal-2,redElemsHorUp-1);
					getFromRecRight(1,0);
					
					proccessData(elems,diffSum,omegaRed,checkIter,horTotal-2,1);
					
					writeToSendUp(horTotal-2,redElemsHorUp-1);
					writeToSendRight(1,0);
					
					//Then we compute the rest of the elements normally and send them right
					{int j,kVer;
					for(kVer=1,j=3;j<verTotal-2;j+=2,++kVer) {
						getFromRecRight(j,kVer);
						proccessData(elems,diffSum,omegaRed,checkIter,horTotal-2,j);
						writeToSendRight(j,kVer);
					}}
				}
				//Else if the first element is black
				else {
					//We simply compute all the elements and send them right
					{int j,kVer;
					for(kVer=0,j=2;j<verTotal-2;j+=2,++kVer) {
						getFromRecRight(j,kVer);
						proccessData(elems,diffSum,omegaRed,checkIter,horTotal-2,j);
						writeToSendRight(j,kVer);
					}}
					//And down if necessary
					
				}
				//Finally, send the bottom right element down and right if necessary
				if(isBottomRightElementRed) {
					getFromRecRight(verTotal-2,redElemsVerRight-1);
					getFromRecDown(horTotal-2,redElemsHorDown-1);
					proccessData(elems,diffSum,omegaRed,checkIter,horTotal-2,verTotal-2);
					writeToSendRight(verTotal-2,redElemsVerRight-1);
					writeToSendDown(horTotal-2,redElemsHorDown-1);
				}
				
				/////////////MID-POINT-DATA-TRANSFER////////////////
				
				//Send buffers should already be filled, so we just send them over. 
				//Same with receive buffers. Their contents have been transferred so they are ready to receive
				if(hasRight) {
					reqSR[sBuf]=cartComm.Isend(bufSR[sBuf],redElemsVerRight,MPI::DOUBLE,destR,BUFF_TAG+sBuf);
					reqRR[prevBuf]=cartComm.Irecv(bufRR[prevBuf],redElemsVerRight,MPI::DOUBLE,destR,BUFF_TAG+prevBuf);
				}
				if(hasLeft) {
					reqSL[sBuf]=cartComm.Isend(bufSL[sBuf],redElemsVerLeft,MPI::DOUBLE,destL,BUFF_TAG+sBuf);
					reqRL[prevBuf]=cartComm.Irecv(bufRL[prevBuf],redElemsVerLeft,MPI::DOUBLE,destL,BUFF_TAG+prevBuf);
				}
				if(hasUp) {
					reqSU[sBuf]=cartComm.Isend(bufSU[sBuf],redElemsHorUp,MPI::DOUBLE,destU,BUFF_TAG+sBuf);
					reqRU[prevBuf]=cartComm.Irecv(bufRU[prevBuf],redElemsHorUp,MPI::DOUBLE,destU,BUFF_TAG+prevBuf);
				}
				if(hasDown) { //sBuf=1,prevBuf=0
					reqSD[sBuf]=cartComm.Isend(bufSD[sBuf],redElemsHorDown,MPI::DOUBLE,destD,BUFF_TAG+sBuf);
					reqRD[prevBuf]=cartComm.Irecv(bufRD[prevBuf],redElemsHorDown,MPI::DOUBLE,destD,BUFF_TAG+prevBuf);
				}
				//Save the previous buffer and select the next buffer
				prevBuf=sBuf;
				sBuf=(sBuf+1)%BUFF_NUM;
				
				//Receive the data. Copying is unnecessary, since they'll be copied during the main loop
				//Make sure send buffers have been sent and we can safely write on them
				if(hasRight) {
					reqRR[prevBuf].Wait();
					reqSR[sBuf].Wait();
				}
				if(hasLeft) {
					reqRL[prevBuf].Wait();
					reqSL[sBuf].Wait();
				}
				if(hasUp) {
					reqRU[prevBuf].Wait();
					reqSU[sBuf].Wait();
				}
				if(hasDown) {
					reqRD[prevBuf].Wait();
					reqSD[sBuf].Wait();
				}
				
				
				/////////////BLACK STARTS HERE//////////////
					
				//We continue with the black elements
					//For the black elements of the first collumn, we know elem[1][1] is red so we start with the black, elem[1][2]
				{int kVer,j;
				for(kVer=0,j=2;j<verTotal-2;j+=2,++kVer) {
					getFromRecLeft(j,kVer);
					proccessData(elems,diffSum,omegaBlack,checkIter,1,j);
					writeToSendLeft(j,kVer);
				}}
					//For the last element of the first collumn, if it is black, compute the new value and send it left and down
				if(!isBottomLeftElementRed) {
					getFromRecLeft(verTotal-2,blackElemsVerLeft-1);
					getFromRecDown(1,0);
					proccessData(elems,diffSum,omegaBlack,checkIter,1,verTotal-2);
					writeToSendLeft(verTotal-2,blackElemsVerLeft-1);
					writeToSendDown(1,0);
				}
				
				//For all the collumns between first and last
				{int kTop,kBot,i;
				for(kTop=0,kBot=isBottomLeftElementRedKappaInverse,
						i=2;i<horTotal-2;i+=2,++kTop,++kBot) {
					//We know the first (elem[2][1]) is black, so we start with the black and compute them normally
					//First we send/rcv for the first black element
					getFromRecUp(i,kTop);
					proccessData(elems,diffSum,omegaBlack,checkIter,i,1);
					writeToSendUp(i,kTop);

					//Then we loop through all the others normally (no need to retrieve data)
					for(int j=3;j<verTotal-2;j+=2) {
						proccessData(elems,diffSum,omegaBlack,checkIter,i,j);
					}
					
					
					//Check if we have reached the end 
					//(horTotal-1 is the edge collumn, so we don't want to compute that or we get segfaults when trying to access elements beyond the edge)
					//(horTotal-2 is the end collumn, and we don't want to compute that in the current loop because we want to send its results to the right)
					int newI=i+1;
					if(unlikely(newI>=horTotal-2)) {
						break;
					}
					
					//Now we know the first element of the collumn is red, so we start with the black
					for(int j=2;j<verTotal-2;j+=2) {
						proccessData(elems,diffSum,omegaBlack,checkIter,newI,j);
					}
					
					//If the last collumn element is black, check for bottom process
					//(If the bottom left element is red, then the one to the right of it is going to be black)
					if(isBottomLeftElementRed) {
						getFromRecDown(i,kBot);
						proccessData(elems,diffSum,omegaBlack,checkIter,i,verTotal-2);
						writeToSendDown(i,kBot);
					}
					//If the bottom left element is black, then the one two elements right of it is going to be black
					else { //if(!isBottomLeftElementRed) {
						getFromRecDown(newI,kBot);
						proccessData(elems,diffSum,omegaBlack,checkIter,newI,verTotal-2);
						writeToSendDown(newI,kBot);
					}
				}}
				//If the top right is black
				if(!isTopRightElementRed) {
					//First we send the first element up and right
					getFromRecRight(1,0);
					getFromRecUp(horTotal-2,blackElemsHorUp-1);
					proccessData(elems,diffSum,omegaBlack,checkIter,horTotal-2,1);
					writeToSendRight(1,0);
					writeToSendUp(horTotal-2,blackElemsHorUp-1);
					
					//Then we compute the rest of the elements normally and send them right
					{int kVer,j;
					for(kVer=1,j=3;j<verTotal-2;j+=2,++kVer) {
						getFromRecRight(j,kVer);
						proccessData(elems,diffSum,omegaBlack,checkIter,horTotal-2,j);
						writeToSendRight(j,kVer);
					}}
				}
				//Else if if the top right element is red
				else {
					//We simply compute all the black elements and send them right
					{int kVer,j;
					for(kVer=0,j=2;j<verTotal-2;j+=2,++kVer) {
						getFromRecRight(j,kVer);
						proccessData(elems,diffSum,omegaBlack,checkIter,horTotal-2,j);
						writeToSendRight(j,kVer);
					}}
				}
				//And send the final element down if necessary
				if(!isBottomRightElementRed) {
					getFromRecRight(verTotal-2,blackElemsVerRight-1);
					getFromRecDown(horTotal-2,blackElemsHorDown-1);
					proccessData(elems,diffSum,omegaBlack,checkIter,horTotal-2,verTotal-2);
					writeToSendRight(verTotal-2,blackElemsVerRight-1);
					writeToSendDown(horTotal-2,blackElemsHorDown-1);
				}
			}
			
			///////////////////////////////////
			//
			//Else if the first element is black
			//
			///////////////////////////////////
			
			else {
				/////////////RED STARTS HERE//////////////
					
				//We start with the red elements
					//For the red elements of the first collumn, we know elem[1][1] is black so we start with the red, elem[1][2]
				{int kVer,j;
				for(kVer=0,j=2;j<verTotal-2;j+=2,++kVer) {
					getFromRecLeft(j,kVer);
					proccessData(elems,diffSum,omegaRed,checkIter,1,j);
					writeToSendLeft(j,kVer);
				}}
					//For the last element of the first collumn, if it is red
				if(isBottomLeftElementRed) {
					getFromRecLeft(verTotal-2,redElemsVerLeft-1);
					getFromRecDown(1,0);
					proccessData(elems,diffSum,omegaRed,checkIter,1,verTotal-2);
					writeToSendLeft(verTotal-2,redElemsVerLeft-1);
					writeToSendDown(1,0);
				}
				
				//For all the collumns between first and last
				{int i,kTop,kBot;
				for(i=2,kTop=0,kBot=isBottomLeftElementRedKappa;i<horTotal-2;i+=2,++kTop,++kBot) {
					//We know the first (elem[2][1]) is red, so we start with the red and compute them normally
					//First we send/rcv for the first red element
					getFromRecUp(i,kTop);
					proccessData(elems,diffSum,omegaRed,checkIter,i,1);
					writeToSendUp(i,kTop);
					
					
					//Then we loop through all the others normally (no need to receive any data)
					for(int j=3;j<verTotal-2;j+=2) {
						proccessData(elems,diffSum,omegaRed,checkIter,i,j);
					}
					
					
					//Check if we have reached the end 
					//(horTotal-1 is the edge collumn, so we don't want to compute that or we get segfaults when trying to access elements beyond the edge)
					//(horTotal-2 is the end collumn, and we don't want to compute that in the current loop because we want to send its results to the right)
					int newI=i+1;
					if(unlikely(newI>=horTotal-2)) {
						break;
					}
					
					//Now we know the first element of the collumn is black, so we start with the red
					for(int j=2;j<verTotal-2;j+=2) {
						proccessData(elems,diffSum,omegaRed,checkIter,newI,j);
					}
					
					//If the last collumn element is red, check for bottom process
					//(If the bottome left is black, then the one to the right of it will be red)
					if(!isBottomLeftElementRed) {
						getFromRecDown(i,kBot);
						proccessData(elems,diffSum,omegaRed,checkIter,i,verTotal-2);
						writeToSendDown(i,kBot);
					}
					//(If the bottom left element is red, then the one two spaces to the right of it will red)
					else{ //if(isBottomLeftElementRed) {
						getFromRecDown(newI,kBot);
						proccessData(elems,diffSum,omegaRed,checkIter,newI,verTotal-2);
						writeToSendDown(newI,kBot);
					}
				}}
				//Now we check if the last collumn begins with red or black
				//If it begins with red
				if(isTopRightElementRed) {
					//So first we send the first element up and right
					getFromRecRight(1,0);
					getFromRecUp(horTotal-2,redElemsHorUp-1);
					proccessData(elems,diffSum,omegaRed,checkIter,horTotal-2,1);
					writeToSendRight(1,0);
					writeToSendUp(horTotal-2,redElemsHorUp-1);
					
					//Then we compute the rest of the elements normally and send them right
					{int j,kVer;
					for(j=3,kVer=1;j<verTotal-2;j+=2,++kVer) {
						getFromRecRight(j,kVer);
						proccessData(elems,diffSum,omegaRed,checkIter,horTotal-2,j);
						writeToSendRight(j,kVer);
					}}
				}
				//Else if the top right element is black
				else {
					//We simply compute all the black elements and send them right
					{int j,kVer;
					for(j=2,kVer=0;j<verTotal-2;j+=2,++kVer) {
						getFromRecRight(j,kVer);
						proccessData(elems,diffSum,omegaRed,checkIter,horTotal-2,j);
						writeToSendRight(j,kVer);
					}}
				}
				//Send the bottom right element down if necessary
				if(isBottomRightElementRed) {
					getFromRecRight(verTotal-2,redElemsVerRight-1);
					getFromRecDown(horTotal-2,redElemsHorDown-1);
					proccessData(elems,diffSum,omegaRed,checkIter,horTotal-2,verTotal-2);
					writeToSendRight(verTotal-2,redElemsVerRight-1);
					writeToSendDown(horTotal-2,redElemsHorDown-1);
				}
				
				/////////////MID-POINT-DATA-TRANSFER////////////////
				
				//Send buffers should already be filled, so we just send them over. 
				//Same with receive buffers. Their contents have been transferred so they are ready to receive
				if(hasRight) {
					reqSR[sBuf]=cartComm.Isend(bufSR[sBuf],redElemsVerRight,MPI::DOUBLE,destR,BUFF_TAG+sBuf);
					reqRR[prevBuf]=cartComm.Irecv(bufRR[prevBuf],redElemsVerRight,MPI::DOUBLE,destR,BUFF_TAG+prevBuf);
				}
				if(hasLeft) {
					reqSL[sBuf]=cartComm.Isend(bufSL[sBuf],redElemsVerLeft,MPI::DOUBLE,destL,BUFF_TAG+sBuf);
					reqRL[prevBuf]=cartComm.Irecv(bufRL[prevBuf],redElemsVerLeft,MPI::DOUBLE,destL,BUFF_TAG+prevBuf);
				}
				if(hasUp) {
					reqSU[sBuf]=cartComm.Isend(bufSU[sBuf],redElemsHorUp,MPI::DOUBLE,destU,BUFF_TAG+sBuf);
					reqRU[prevBuf]=cartComm.Irecv(bufRU[prevBuf],redElemsHorUp,MPI::DOUBLE,destU,BUFF_TAG+prevBuf);
				}
				if(hasDown) { //sBuf=1,prevBuf=0
					reqSD[sBuf]=cartComm.Isend(bufSD[sBuf],redElemsHorDown,MPI::DOUBLE,destD,BUFF_TAG+sBuf);
					reqRD[prevBuf]=cartComm.Irecv(bufRD[prevBuf],redElemsHorDown,MPI::DOUBLE,destD,BUFF_TAG+prevBuf);
				}
				//Save the previous buffer and select the next buffer
				prevBuf=sBuf;
				sBuf=(sBuf+1)%BUFF_NUM;
				
				//Receive the data. Copying is unnecessary, since they'll be copied during the main loop
				//Make sure send buffers have been sent and we can safely write on them
				if(hasRight) {
					reqRR[prevBuf].Wait();
					reqSR[sBuf].Wait();
				}
				if(hasLeft) {
					reqRL[prevBuf].Wait();
					reqSL[sBuf].Wait();
				}
				if(hasUp) {
					reqRU[prevBuf].Wait();
					reqSU[sBuf].Wait();
				}
				if(hasDown) {
					reqRD[prevBuf].Wait();
					reqSD[sBuf].Wait();
				}
				
				/////////////BLACK STARTS HERE//////////////
				
				//We start with the black elements
					//For the first element (which we know to be black)
						//Get the data from the receive buffers for the first element
				getFromRecUp(1,0);
				getFromRecLeft(1,0);
				proccessData(elems,diffSum,omegaBlack,checkIter,1,1);
				writeToSendUp(1,0);
				writeToSendLeft(1,0);
						
					//For the rest of the first collumn
				{int j,kVer;
				for(j=3,kVer=1;j<verTotal-2;j+=2,++kVer) {
					getFromRecLeft(j,kVer);
					proccessData(elems,diffSum,omegaBlack,checkIter,1,j);
					writeToSendLeft(j,kVer);
				}}
					//For the last element of the first collumn, if it is black
				if(!isBottomLeftElementRed) {
					getFromRecLeft(verTotal-2,blackElemsVerLeft-1);
					getFromRecDown(1,0);
					proccessData(elems,diffSum,omegaBlack,checkIter,1,verTotal-2);
					writeToSendLeft(verTotal-2,blackElemsVerLeft-1);
					writeToSendDown(1,0);
				}
				
					//For all collumns, except the end collumn
				{int i,kTop,kBot;
				for(i=2,kTop=1,kBot=isBottomLeftElementRedKappaInverse
						;i<horTotal-2;i+=2,++kTop,++kBot) {
					//We know the first (elem[2][1]) is red, so we start with the black and compute them normally (no need to retrieve data)
					for(int j=2;j<verTotal-2;j+=2) {
						proccessData(elems,diffSum,omegaBlack,checkIter,i,j);
					}
					//call debugPrint(elems,0,0,horTotal,verTotal)
					//Check if we have reached the end 
					//(horTotal-1 is the edge collumn, so we don't want to compute that or we get segfaults when trying to access elements beyond the edge)
					//(horTotal-2 is the end collumn, and we don't want to compute that in the current loop because we want to send its results to the right)
					int newI=i+1;
					if(unlikely(newI>=horTotal-2)) {
						break;
					}
					
					//We know the next collumn is going to have the first element be black, so we need to send/receive for that
					getFromRecUp(newI,kTop);
					proccessData(elems,diffSum,omegaBlack,checkIter,newI,verTotal-2);
					writeToSendUp(newI,kTop);
					
					//Then we loop through all the others normally
					for(int j=3;j<verTotal-2;j+=2) {
						proccessData(elems,diffSum,omegaBlack,checkIter,newI,j);
					}
					
					//If the last collumn element is black, check for bottom process
					//(If the bottom left element is red, then the one to the right of it is going to be black)
					if(isBottomLeftElementRed) {
						getFromRecDown(i,kBot);
						proccessData(elems,diffSum,omegaBlack,checkIter,i,verTotal-2);
						writeToSendDown(i,kBot);
					}
					//(If the bottom left element is black, then the one two spaces to the right of it is going to be black)
					else { //if(!isBottomLeftElementRed) {
						getFromRecDown(newI,kBot);
						proccessData(elems,diffSum,omegaBlack,checkIter,newI,verTotal-2);
						writeToSendDown(newI,kBot);
					}
				}}
				//If the top right is red
				if(isTopRightElementRed) {
					//We simply compute all the black elements and send them right
					{int j,kVer;
					for(j=2,kVer=0;j<verTotal-2;j+=2,++kVer) {
						getFromRecRight(j,kVer);
						proccessData(elems,diffSum,omegaBlack,checkIter,horTotal-2,j);
						writeToSendRight(j,kVer);
					}}
				}
				//Else if the top right is black
				else {
					//First we send the first element up and right
					getFromRecUp(horTotal-2,blackElemsHorUp-1);
					getFromRecRight(1,0);
					proccessData(elems,diffSum,omegaBlack,checkIter,horTotal-2,1);
					writeToSendUp(horTotal-2,blackElemsHorUp-1);
					writeToSendRight(1,0);
					
					//Then we compute the rest of the elements normally and send them right
					{int j,kVer;
					for(j=3,kVer=1;j<verTotal-2;j+=2,++kVer) {
						//Get the data from the receive buffers
						getFromRecRight(j,kVer);
						proccessData(elems,diffSum,omegaBlack,checkIter,horTotal-2,j);
						writeToSendRight(j,kVer);
					}}
				}
				//Do the bottom right if it's black
				if(!isBottomRightElementRed) {
					getFromRecRight(verTotal-2,blackElemsVerRight-1);
					getFromRecDown(horTotal-2,blackElemsHorDown-1);
					proccessData(elems,diffSum,omegaRed,checkIter,horTotal-2,verTotal-2);
					writeToSendRight(verTotal-2,blackElemsVerRight-1);
					writeToSendDown(horTotal-2,blackElemsHorDown-1);
				}
			}
		#endif
		
		#ifndef SINGLE_PROC
			//Make a reduce request, now that we have all the data
			double allSum;
			if(unlikely(checkIter)) {
				MPI_Iallreduce(&diffSum,&allSum,1,MPI::DOUBLE,MPI::SUM,cartComm,&reqSum);
			}
		#endif
		
		#ifndef SINGLE_PROC
			#ifndef MULTI_BUFFER
				//Finally, wait on the send buffers, copy new data there and 
				//make the buffers availiable to the other processes 
				//Now we send the black ones, since the others will need them for the red ones
				if(hasRight) {
					reqSRB.Wait();
					{int k,j;
					for(k=0,j=isTopRightElementRedIndexInverse;j<verTotal-1;++k,j+=2) {
						bufSRB[k]=elems[horTotal-2][j];
					}}
					reqSRB=cartComm.Isend(bufSRB,blackElemsVerRight,MPI::DOUBLE,destR,BUFF_TAG+1); //+1 for black
				}
				if(hasLeft) {
					reqSLB.Wait();
					{int k,j;
					for(k=0,j=isFirstElemRedIndexInverse;j<verTotal-1;++k,j+=2) {
						bufSLB[k]=elems[1][j];
					}}
					reqSLB=cartComm.Isend(bufSLB,blackElemsVerLeft,MPI::DOUBLE,destL,BUFF_TAG+1);
				}
				if(hasUp) {
					reqSUB.Wait();
					{int k,i;
					for(k=0,i=isFirstElemRedIndexInverse;i<horTotal-1;++k,i+=2) {
						bufSUB[k]=elems[i][1];
					}}
					reqSUB=cartComm.Isend(bufSUB,blackElemsHorUp,MPI::DOUBLE,destU,BUFF_TAG+1);
				}
				if(hasDown) {
					reqSDB.Wait();
					{int k,i;
					for(k=0,i=isBottomLeftElementRedIndexInverse;i<horTotal-1;++k,i+=2) {
						bufSDB[k]=elems[i][verTotal-2];
					}}
					reqSDB=cartComm.Isend(bufSDB,blackElemsHorDown,MPI::DOUBLE,destD,BUFF_TAG+1);
				}
			#else //If we are using multiple buffers
				//Send buffers should already be filled, so we just send them over. Same with receive buffers
				if(hasRight) {
					reqSR[sBuf]=cartComm.Isend(bufSR[sBuf],blackElemsVerRight,MPI::DOUBLE,destR,BUFF_TAG+sBuf);
					reqRR[prevBuf]=cartComm.Irecv(bufRR[prevBuf],blackElemsVerRight,MPI::DOUBLE,destR,BUFF_TAG+prevBuf);
				}
				if(hasLeft) {
					reqSL[sBuf]=cartComm.Isend(bufSL[sBuf],blackElemsVerLeft,MPI::DOUBLE,destL,BUFF_TAG+sBuf);
					reqRL[prevBuf]=cartComm.Irecv(bufRL[prevBuf],blackElemsVerLeft,MPI::DOUBLE,destL,BUFF_TAG+prevBuf);
				}
				if(hasUp) {
					reqSU[sBuf]=cartComm.Isend(bufSU[sBuf],blackElemsHorUp,MPI::DOUBLE,destU,BUFF_TAG+sBuf);
					reqRU[prevBuf]=cartComm.Irecv(bufRU[prevBuf],blackElemsHorUp,MPI::DOUBLE,destU,BUFF_TAG+prevBuf);
				}
				if(hasDown) {
					reqSD[sBuf]=cartComm.Isend(bufSD[sBuf],blackElemsHorDown,MPI::DOUBLE,destD,BUFF_TAG+sBuf);
					reqRD[prevBuf]=cartComm.Irecv(bufRD[prevBuf],blackElemsHorDown,MPI::DOUBLE,destD,BUFF_TAG+prevBuf);
				}
			#endif
		#endif
		
		#ifndef SINGLE_PROC
			//Each ITER_CHECK times, check for the sum and exit if we have reached the required level of convergence
			if(unlikely(checkIter)) {
				MPI_Wait(&reqSum,MPI_STATUS_IGNORE);
				if(unlikely(allSum<epsilon)) {
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
			if(unlikely(checkIter && diffSum<epsilon)) {
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
			#ifndef NDEBUG
				cout<<"iter:"<<iteration<<endl;
			#endif
		}
	#else
		//Get end time
		time_end = high_resolution_clock::now();
		duration<double> dur= duration_cast<duration<double>>(time_end-time_initial);
		//Output time taken to complete
		cout<<fixed<<setprecision(20)<<dur.count()<<endl;
		//cerr<<fixed<<setprecision(20)<<(time_end-time_initial)<<endl;
		#ifndef NDEBUG
			cout<<"iter:"<<iteration<<endl;
		#endif
	#endif
	
	//free memory
	deleteElements(elems,horTotal);
	
	#ifndef SINGLE_PROC
		#ifndef MULTI_BUFFER
			//Destroy communication buffers if necessary
			if(hasRight) {
				delete[] bufSRR;
				delete[] bufRRR;
				delete[] bufSRB;
				delete[] bufRRB;
			}
			if(hasLeft) {
				delete[] bufSLR;
				delete[] bufRLR;
				delete[] bufSLB;
				delete[] bufRLB;
			}
			if(hasUp) {
				delete[] bufSUR;
				delete[] bufRUR;
				delete[] bufSUB;
				delete[] bufRUB;
			}
			if(hasDown) {
				delete[] bufSDR;
				delete[] bufRDR;
				delete[] bufSDB;
				delete[] bufRDB;
			}
		#else //If we are using multiple buffers
			for(int i=0;i<BUFF_NUM;++i) {
				//Destroy communication buffers if necessary
				if(hasRight) {
					delete[] bufSR[i];
					delete[] bufRR[i];
				}
				if(hasLeft) {
					delete[] bufSL[i];
					delete[] bufRL[i];
				}
				if(hasUp) {
					delete[] bufSU[i];
					delete[] bufRU[i];
				}
				if(hasDown) {
					delete[] bufSD[i];
					delete[] bufRD[i];
				}
			}
			if(hasRight) {
				delete[] bufSR;
				delete[] bufRR;
			}
			if(hasLeft) {
				delete[] bufSL;
				delete[] bufRL;
			}
			if(hasUp) {
				delete[] bufSU;
				delete[] bufRU;
			}
			if(hasDown) {
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

#ifdef MULTI_BUFFER
	inline void writeToSendLR(double ** elems, double** buff, int sBuf, int i, int j, bool destExists, int k) {
		if(destExists) {
			buff[sBuf][k]=elems[i][j];
		}
	}

	inline void writeToSendUD(double ** elems, double** buff, int sBuf, int i, int j, bool destExists, int k) {
		if(destExists) {
			buff[sBuf][k]=elems[i][j];
		}
	}

	inline void getFromRecLR(double ** elems, double** buff, int prevBuf, int i, int j, int offsX, bool destExists, int k) {
		if(destExists) {
			elems[i+offsX][j]=buff[prevBuf][k];
		}
	}

	inline void getFromRecUD(double ** elems, double** buff, int prevBuf, int i, int j, int offsY, bool destExists, int k) {
		if(destExists) {
			elems[i][j+offsY]=buff[prevBuf][k];
		}
	}
#endif

inline void proccessData(double ** elems, double& diffSum, double omega, bool checkIter, int i, int j) {
	if(unlikely(checkIter)) {
		double newVal=equation(omega,elems,i,j);
		diffSum+=((newVal-elems[i][j])*(newVal-elems[i][j]));
		elems[i][j]=newVal;
	}
	else {
		elems[i][j]=equation(omega,elems,i,j);
	}
}

inline void deleteElements(double ** elems, int horTotal) {
	for(int i=0;i<horTotal;++i) {
		delete[] elems[i];
	}
	delete[] elems;
}

#ifndef NDEBUG
	void debugPrint(double ** elems, int startX, int startY, int endX, int endY) {
		for(int j=startY;j<endY;++j) {
			cout<<"{";
			for(int i=startX;i<endX-1;++i) {
				cout<<elems[i][j]<<", ";
			}
			cout<<elems[endX-1][j];
			cout<<"}"<<endl;
		}
	}
#endif

