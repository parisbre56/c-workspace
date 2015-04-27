#include <mpi.h>

#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>

#include <iostream>
#include <iomanip>

//Prints debug messages
//#define __DEBUG__MODE_EX1__

//Use this to disable inter-thread communication to run it in single processor mode
//#define __SingleProc__

//Use this for the simple partition of collumns
//#define __Question1__
//Use this for the shuffle partition of collumns
//#define __Question2__

//Makes the program compute the inverse instead
//#define __QuestionExtra__

//The number of collumns per thread (*2 for the inverse)
//#define kappa 2
//#define kappa 4
//#define kappa 16
//#define kappa 32
//#define kappa 64
//#define kappa 128

//This generates a random matrix with numbers between [-MAX_DEVIATION,MAX_DEVIATION]
#define MAX_DEVIATION 10

//Various tags for communication
#define COL_NUM_TAG 1
#define COL_TAG 2

//How many digits will be printed (used for debug, not for normal printing)
#define OUTPUT_PRECISION 20

//Uses broadcast instead of sending the data from one thread to the next
//#define USE_BROADCAST
//#define USE_BROADCAST_ASYNC

using namespace std;

/**
 * @brief Creates a matrix with n rows and n+1 collumns
 * @param cartComm The 1D cartesian communicator being used
 * @param tid The id of the calling thread
 * @param nthreads The total number of threads
 * @param n The rows of the matrix
 * @param matrPart Where the part of the matrix corresponding to this thread will be returned
 * @param partSize The size of the returned part
 */
void createMatrix(MPI_Comm cartComm, int tid, int nthreads, int n,double *** matrPart, int& partSize);

/**
 * @brief Prints a matrix with n rows and n+1 collumns by combining the individual parts
 * @param cartComm The 1D cartesian communicator being used
 * @param tid The id of the calling thread
 * @param nthreads The total number of threads
 * @param n The rows of the matrix
 * @param matrPart Where the part of the matrix corresponding to this thread will be returned
 * @param partSize The size of the returned part
 */
void printMatrix(MPI_Comm cartComm, int tid, int nthreads, int n, double *** matrPart, int partSize);

/**
 * @brief Checks if the collumn given (in global collumn number) is for this thread.
 * @param tid The id of the calling thread
 * @param nthreads The total number of threads
 * @param n The rows of the matrix
 * @param col The collumn to test (in global collumn number, zero base)
 * @return True if col belongs to tid, false otherwise
 */
bool colValidForThread(int tid, int nthreads, int n, int col);

/**
 * @brief 
 * @param tid The id of the calling thread
 * @param nthreads The total number of threads
 * @param n The rows of the matrix
 * @param col The collumn to test (in global collumn number, zero base)
 * @return Returns the thread (zero base) the given collumn belongs to 
 */
int threadForCollumn(int nthreads, int n, int col);

/**
 * @brief Converts the given collumn number in local part number to global number
 * @param tid The id of the calling thread
 * @param nthreads The total number of threads
 * @param n The rows of the matrix
 * @param col The collumn to test (in global collumn number, zero base)
 * @return The given collumn's global number
 */
int partColToGlobCol(int tid, int nthreads, int n, int col);

/**
 * @brief Converts the given collumn number in global number to local part number
 * @param tid The id of the calling thread
 * @param nthreads The total number of threads
 * @param n The rows of the matrix
 * @param col The collumn to test (in local part collumn number, zero base)
 * @return The given collumn's local number
 */
int globColToPartCol(int tid, int nthreads, int n, int col);

int main(int argc, char **argv)
{
	MPI_Comm cartComm; //Cartesian communicator
	
	int tid; //Thread id
	int nthreads; //Number of threads
	
	double time_initial; //Start time
	double time_end; //End time
	
	int n; //N is the size of the matrix
	
	//Wrap around
	int wrapAround=1;
	
	#if defined(USE_BROADCAST_ASYNC) //If asynchronus broadcast is enabled, keep a request for testing if the data can be safely modified after being sent
		MPI_Request bcastRequest;
	#endif
	
	//Initialize the MPI environment
	if(MPI_Init(NULL,NULL)!=MPI_SUCCESS) {
		cerr<<"ERROR"<<endl;
	}
	
	//Get number of threads
	if(MPI_Comm_size(MPI_COMM_WORLD, &nthreads)!=MPI_SUCCESS) {
		cerr<<"ERROR"<<endl;
	}
	
	//Create one dimensional cartesian grouping that is NOT ring-shaped (does not wrap around)
	//??? Make it wrap around and add a note to the message that tells it to stop when it has reached the start?
	if(MPI_Cart_create(MPI_COMM_WORLD,1,&nthreads,&wrapAround,1,&cartComm)!=MPI_SUCCESS) {
		cerr<<"ERROR"<<endl;
	}
	
	//Get number of threads
	if(MPI_Comm_size(cartComm, &nthreads)!=MPI_SUCCESS) {
		cerr<<"ERROR"<<endl;
	}
	
	//Get thread id
	if(MPI_Comm_rank(cartComm, &tid)!=MPI_SUCCESS) {
		cerr<<"ERROR"<<endl;
	}
	int destinationN;
	int destinationP;
	MPI_Cart_shift(cartComm,0,1,&destinationP,&destinationN);
	
	//Set the size of the matrix
	n=kappa*nthreads;
	
	//Initialize rand
	srand(time(NULL)+tid);
	 
	//Create the matrix and split it amongs the threads
	double ** matrPart; //Holds this thread's part of the matrix
	int partSize;
	
	createMatrix(cartComm,tid,nthreads,n,&matrPart,partSize);
	
	#ifdef __DEBUG__MODE_EX1__
	if(tid==0) {
		cout<<"------------------------------------------"<<endl;
	}
	printMatrix(cartComm,tid,nthreads,n,&matrPart,partSize);
	if(tid==0) {
		cout<<"------------------------------------------"<<endl;
	}
	#endif
	
	//Create a cache for optimization
	//This ensures that there is no difference due to how expensive the functions used to find the right collumn or processor are
	#ifdef __QuestionExtra__
		int colnum = n*2;
	#else
		int colnum = n+1;
	#endif
	int* thrForCol = new int[colnum]; //Tells us which thread each collumn belongs to
	for (int i=0;i<colnum;++i) {
		thrForCol[i]=threadForCollumn(nthreads,n,i);
	}
	
	bool* colValidForThr = new bool[colnum]; //Tells us if the coolumn selected is valid for the current thread
	for (int i=0;i<colnum;++i) {
		colValidForThr[i]=(thrForCol[i]==tid);
	}
	
	int* glColToPartCol = new int[colnum]; //Holds the part collumn for the global collumn given (-1 if invalid)
	for (int i=0;i<colnum;++i) {
		if(colValidForThr[i]) {
			glColToPartCol[i]=globColToPartCol(tid,nthreads,n,i);
		}
		else {
			glColToPartCol[i]=-1;
		}
	}
	
	int* ptColToGlobCol = new int[partSize]; //Holds the global collumn for the part column given
	for (int i=0;i<partSize;++i) {
		ptColToGlobCol[i]=partColToGlobCol(tid,nthreads,n,i);
	}
	
	//If this is computing the inverse matrix
	#ifdef __QuestionExtra__ 
		bool* inInverseMatrix = new bool[partSize]; //True if in the inverse matrix
		for (int i=0;i<partSize;++i) {
			inInverseMatrix[i]=(ptColToGlobCol[i]>=n);
		}
	#endif
	
	//Set the active diagonal to 0
	int k=0;
	int kapOwner;
	
	if(tid==0) {
		//Get the start time
		time_initial  = MPI_Wtime();
	}
	
	//Start solving
	while(k<n) {
		kapOwner=thrForCol[k];
		//If this is the owner of kappa
		if(tid==kapOwner) {
			//Get the collumn you need
			int curCol=glColToPartCol[k];
			//For row k, divide it so that it becomes 1 and send what you divided it with to the other rows
			//First send what we need to do to it to the other threads (which is [k,k])
			//(Data sent is number to divide with (the other threads should have the correct k and sender))
			#ifndef __SingleProc__
				#ifdef USE_BROADCAST
					MPI_Bcast(&(matrPart[curCol][k]),1,MPI_DOUBLE,kapOwner,cartComm);
				#elif defined(USE_BROADCAST_ASYNC)
					MPI_Ibcast(&(matrPart[curCol][k]),1,MPI_DOUBLE,kapOwner,cartComm, &bcastRequest);
				#else //if not defined USE_BROADCAST
					MPI_Send(&(matrPart[curCol][k]),1,MPI_DOUBLE,destinationN,COL_TAG,cartComm);
				#endif
			#endif
			//Then divide with that number
			for(int jj=curCol+1;jj<partSize;++jj) {
				matrPart[jj][k]=matrPart[jj][k]/matrPart[curCol][k];
			}
			#if !defined(__SingleProc__) && defined(USE_BROADCAST_ASYNC)
				//Wait for the buffer to be read if sending asynchronously, to avoid race conditions
				MPI_Wait(&bcastRequest, MPI_STATUS_IGNORE);
			#endif
			matrPart[curCol][k]=1; //No need to do a real division for the first element
			
			//Then for all rows, subtract and send what we are multiplying to subtract to the other threads
			for(int i=k+1;i<n;++i) {
				//First send
				#ifndef __SingleProc__
					#ifdef USE_BROADCAST
						MPI_Bcast(&(matrPart[curCol][i]),1,MPI_DOUBLE,kapOwner,cartComm);
					#elif defined(USE_BROADCAST_ASYNC)
						MPI_Ibcast(&(matrPart[curCol][i]),1,MPI_DOUBLE,kapOwner,cartComm, &bcastRequest);
					#else //if not defined USE_BROADCAST
						MPI_Send(&(matrPart[curCol][i]),1,MPI_DOUBLE,destinationN,COL_TAG,cartComm);
					#endif
				#endif
				//For all partcollumns, check to see if we can subtract anything
				//(their global col must be greater than k and current collumn)
				for(int jj=curCol+1;jj<partSize;++jj) {
					matrPart[jj][i]=matrPart[jj][i]-matrPart[jj][k]*matrPart[curCol][i];
				}
				#if !defined(__SingleProc__) && defined(USE_BROADCAST_ASYNC)
					//Wait for the buffer to be read if sending asynchronously, to avoid race conditions
					MPI_Wait(&bcastRequest, MPI_STATUS_IGNORE);
				#endif
				//Then subtract
				matrPart[curCol][i]=0; //NO need to do real subtraction for the first element
			}
		}
		//Else, if this is not the owner of kappa
		else {
			//Used for optimisation
			bool isValid=false;
			bool isValidArr[partSize];
			for(int j=0;j<partSize;++j) {
				if(ptColToGlobCol[j]>k) {
					isValid=true;
					isValidArr[j]=true;
				}
				else {
					isValidArr[j]=false;
				}
			}
			//First receive the number you need to divide k row with and send it to the next one 
			//(unless next one is sender)
			double recD;
			#ifdef USE_BROADCAST
				MPI_Bcast(&recD,1,MPI_DOUBLE,kapOwner,cartComm);
			#elif defined(USE_BROADCAST_ASYNC)
				MPI_Ibcast(&recD,1,MPI_DOUBLE,kapOwner,cartComm, &bcastRequest);
				MPI_Wait(&bcastRequest, MPI_STATUS_IGNORE);
			#else //if not defined USE_BROADCAST
				MPI_Recv(&recD,1,MPI_DOUBLE,destinationP,MPI_ANY_TAG,cartComm,MPI_STATUS_IGNORE);
				if(destinationN!=kapOwner) {
					MPI_Send(&recD,1,MPI_DOUBLE,destinationN,COL_TAG,cartComm);
				}
			#endif
			//Then divide k row if necessary
			if(isValid) {
				for(int j=0;j<partSize;++j) {
					if(isValidArr[j]) {
						matrPart[j][k]=matrPart[j][k]/recD;
					}
				}
			}
			//Then for all rows below k row, receive what we need to multiply the subtraction with
			//and do that if necessary
			for(int i=k+1;i<n;++i) {
				#ifdef USE_BROADCAST
					MPI_Bcast(&recD,1,MPI_DOUBLE,kapOwner,cartComm);
				#elif defined(USE_BROADCAST_ASYNC)
					MPI_Ibcast(&recD,1,MPI_DOUBLE,kapOwner,cartComm, &bcastRequest);
					MPI_Wait(&bcastRequest, MPI_STATUS_IGNORE);
				#else //if not defined USE_BROADCAST
					MPI_Recv(&recD,1,MPI_DOUBLE,destinationP,MPI_ANY_TAG,cartComm,MPI_STATUS_IGNORE);
					if(destinationN!=kapOwner) {
						MPI_Send(&recD,1,MPI_DOUBLE,destinationN,COL_TAG,cartComm);
					}
				#endif
				if(isValid) {
					for(int j=0;j<partSize;++j) {
						if(isValidArr[j]) {
							matrPart[j][i]=matrPart[j][i]-recD*matrPart[j][k];
						}
					}
				}
			}
		}
		//Finally, increment k
		++k;
		
		#ifdef __DEBUG__MODE_EX1__
		printMatrix(cartComm,tid,nthreads,n,&matrPart,partSize);
		if(tid==0) {
			cout<<"------------------------------------------"<<endl;
		}
		#endif
	}
	
	k=n-1;
	
	#ifdef __QuestionExtra__ //IF THIS IS COMPUTING THE INVERSE MATRIX
		while(k>0) {
			kapOwner=thrForCol[k];
			
			//If this is the owner of kappa
			if(tid==kapOwner) {
				//Get the collumn you need
				int curCol=glColToPartCol[k];
				for(int i=k-1;i>=0;--i) {
					#ifndef __SingleProc__
						#ifdef USE_BROADCAST
							MPI_Bcast(&(matrPart[curCol][i]),1,MPI_DOUBLE,kapOwner,cartComm);
						#elif defined(USE_BROADCAST_ASYNC)
							MPI_Ibcast(&(matrPart[curCol][i]),1,MPI_DOUBLE,kapOwner,cartComm, &bcastRequest);
						#else //if not defined USE_BROADCAST
							MPI_Send(&(matrPart[curCol][i]),1,MPI_DOUBLE,destinationN,COL_TAG,cartComm);
						#endif
					#endif
					for(int j=curCol+1;j<partSize;++j) {
						//If this is in the inverse matrix
						if(inInverseMatrix[j]) { 
							matrPart[j][i]=matrPart[j][i]-matrPart[j][k]*matrPart[curCol][i];
						}
					}
					#if !defined(__SingleProc__) && defined(USE_BROADCAST_ASYNC)
						//Wait for the buffer to be read if sending asynchronously, to avoid race conditions
						MPI_Wait(&bcastRequest, MPI_STATUS_IGNORE);
					#endif
					matrPart[curCol][i]=0; //No need to do real subtraction.
				}
			}
			//Else, if this is not the owner of kappa
			else {
				//for all rows above k row, receive what we need to multiply the subtraction with
				//and do that if necessary
				double recD;
				for(int i=k-1;i>=0;--i) {
					#ifdef USE_BROADCAST
						MPI_Bcast(&recD,1,MPI_DOUBLE,kapOwner,cartComm);
					#elif defined(USE_BROADCAST_ASYNC)
						MPI_Ibcast(&recD,1,MPI_DOUBLE,kapOwner,cartComm, &bcastRequest);
						MPI_Wait(&bcastRequest, MPI_STATUS_IGNORE);
					#else //if not defined USE_BROADCAST
						MPI_Recv(&recD,1,MPI_DOUBLE,destinationP,MPI_ANY_TAG,cartComm,MPI_STATUS_IGNORE);
						if(destinationN!=kapOwner) { //Pass it along to the next thread
							MPI_Send(&recD,1,MPI_DOUBLE,destinationN,COL_TAG,cartComm);
						}
					#endif
					//For all collumns
					for(int j=0;j<partSize;++j) {
						//If this is in the inverse matrix
						if(inInverseMatrix[j]) {
							matrPart[j][i]=matrPart[j][i]-recD*matrPart[j][k];
						}
					}
				}
			}
			//Finally, decrement kappa
			--k;
			
			#ifdef __DEBUG__MODE_EX1__
				printMatrix(cartComm,tid,nthreads,n,&matrPart,partSize);
				if(tid==0) {
					cout<<"------------------------------------------"<<endl;
				}
			#endif
		}
	#else //If this is not computing the inverse matrix but doing elimination
		while(k>0) {
			//Used for optimisation
			int endCol;
			bool isValid=colValidForThr[n];
			if(isValid) {
				endCol=glColToPartCol[n];
			}
			
			kapOwner=thrForCol[k];
			//If this is the owner of kappa
			if(tid==kapOwner) {
				//Get the collumn you need
				int curCol=glColToPartCol[k];
				for(int i=k-1;i>=0;--i) {
					#ifndef __SingleProc__
						#ifdef USE_BROADCAST
							MPI_Bcast(&(matrPart[curCol][i]),1,MPI_DOUBLE,kapOwner,cartComm);
						#elif defined(USE_BROADCAST_ASYNC)
							MPI_Ibcast(&(matrPart[curCol][i]),1,MPI_DOUBLE,kapOwner,cartComm, &bcastRequest);
						#else //if not defined USE_BROADCAST
							MPI_Send(&(matrPart[curCol][i]),1,MPI_DOUBLE,destinationN,COL_TAG,cartComm);
						#endif
					#endif
					if(isValid) {
						matrPart[endCol][i]=matrPart[endCol][i]-matrPart[endCol][k]*matrPart[curCol][i];
					}
					#if !defined(__SingleProc__) && defined(USE_BROADCAST_ASYNC)
						//Wait for the buffer to be read if sending asynchronously, to avoid race conditions
						MPI_Wait(&bcastRequest, MPI_STATUS_IGNORE);
					#endif
					matrPart[curCol][i]=0; //No need to do real subtraction.
				}
			}
			//Else, if this is not the owner of kappa
			else {
				//for all rows above k row, receive what we need to multiply the subtraction with
				//and do that if necessary
				double recD;
				for(int i=k-1;i>=0;--i) {
					#ifdef USE_BROADCAST
						MPI_Bcast(&recD,1,MPI_DOUBLE,kapOwner,cartComm);
					#elif defined(USE_BROADCAST_ASYNC)
						MPI_Ibcast(&recD,1,MPI_DOUBLE,kapOwner,cartComm, &bcastRequest);
						MPI_Wait(&bcastRequest, MPI_STATUS_IGNORE);
					#else //if not defined USE_BROADCAST
						MPI_Recv(&recD,1,MPI_DOUBLE,destinationP,MPI_ANY_TAG,cartComm,MPI_STATUS_IGNORE);
						if(destinationN!=kapOwner) {
							MPI_Send(&recD,1,MPI_DOUBLE,destinationN,COL_TAG,cartComm);
						}
					#endif
					if(isValid) {
						matrPart[endCol][i]=matrPart[endCol][i]-recD*matrPart[endCol][k];
					}
				}
			}
			//Finally, decrement kappa
			--k;
			
			#ifdef __DEBUG__MODE_EX1__
				printMatrix(cartComm,tid,nthreads,n,&matrPart,partSize);
				if(tid==0) {
					cout<<"------------------------------------------"<<endl;
				}
			#endif
		}
	#endif
	
	if(tid==0) {
		//Get the end time
		time_end = MPI_Wtime();
	}
	
	#ifdef __DEBUG__MODE_EX1__
		//Print the solution
		printMatrix(cartComm,tid,nthreads,n,&matrPart,partSize);
	#endif
	
	if(tid==0) {
		#ifdef __DEBUG__MODE_EX1__
			//Write some info
			cout<<"Solved in "<<(time_end-time_initial)<<" seconds in "<<nthreads<<" threads using configuration ";
			#ifdef __Question1__
			cout<<"1:\"serial\""<<endl;
			#endif
			#ifdef __Question2__
			cout<<"2:\"shuffle\""<<endl;
			#endif
		#else
			/*if(isnan(matrPart[0][0])) {
				cout<<"INVALID MATRIX: NAN"<<endl;
			}
			else {*/
				cout<<fixed<<setprecision(20)<<(time_end-time_initial)<<endl;
			//}
		#endif
	}
	
	//Delete data
	for(int j=0;j<partSize;++j) {
		delete[] matrPart[j];
	}
	delete[] matrPart;
	
	//Delete cache
	delete[] thrForCol;
	delete[] colValidForThr;
	delete[] glColToPartCol;
	delete[] ptColToGlobCol;
	#ifdef __QuestionExtra__ 
		delete[] inInverseMatrix;
	#endif
	
	//Finalize the MPI environment
	if(MPI_Finalize()!=MPI_SUCCESS) {
		cerr<<tid<<" ERROR"<<endl;
	}
	
	//Exit
	return EXIT_SUCCESS;
}

void createMatrix(MPI_Comm cartComm, int tid, int nthreads, int n, double *** matrPart, int& partSize) {
	//Create empty matrix
	partSize=0;
	#ifdef __QuestionExtra__ //IF THIS IS COMPUTING THE INVERSE MATRIX
		(*matrPart)=new double*[kappa*2];
	#else
		//If this is thread will hold the extra collumn
		if(colValidForThread(tid,nthreads,n,n)) {
			(*matrPart)=new double*[kappa+1];
		} else {
			(*matrPart)=new double*[kappa];
		}
	#endif
	
	int destinationN;
	int destinationP;
	MPI_Cart_shift(cartComm,0,1,&destinationP,&destinationN);
	
	//For process 0
	if(tid==0) {
		#ifdef __QuestionExtra__ //IF THIS IS COMPUTING THE INVERSE MATRIX
			double ** matr = new double*[n*2];
			//Create an array to hold the matrix (right now we just hope determinant is non-zero)
			//Create the matrix (dynamic creation prevents stack overflow)
			for(int j=0;j<n*2;++j) {
				matr[j]=new double[n];
			}
		#else
			double ** matr = new double*[n+1];
			//Create an array to hold the matrix (right now we just hope determinant is non-zero)
			//Create the matrix (dynamic creation prevents stack overflow)
			for(int j=0;j<n+1;++j) {
				matr[j]=new double[n];
			}
		#endif
		
		#ifdef __QuestionExtra__ //IF THIS IS COMPUTING THE INVERSE MATRIX
			//For all matrix collumns
			for(int j=0;j<n;++j) {
				//Generate the matrix rows
				for(int i=0;i<n;++i) {
					matr[j][i]= (rand()%((MAX_DEVIATION*2)+1)) - MAX_DEVIATION;
				}
			}
			//For all matrix collumns
			for(int j=n;j<n*2;++j) {
				//Generate the matrix rows (diagonal is 1, rest is 0)
				for(int i=0;i<n;++i) {
					if(i!=j-n) {
						matr[j][i]=0.0;
					} else {
						matr[j][i]=1.0;
					}
				}
			}
		#else
			//For all matrix collumns
			for(int j=0;j<n+1;++j) {
				//Generate the matrix rows
				for(int i=0;i<n;++i) {
					matr[j][i]= (rand()%((MAX_DEVIATION*2)+1)) - MAX_DEVIATION;
				}
			}
		#endif
		
		//TODO: MAKE LOOP TO CHECK DETERMINANT
		
		#ifdef __QuestionExtra__ //IF THIS IS COMPUTING THE INVERSE MATRIX
			//Send each thread its collumns
			for(int j=0;j<n*2;++j) { 
				//If this collumn is for this thread
				if(colValidForThread(tid,nthreads,n,j)) {
					(*matrPart)[partSize]=new double[n];
					memcpy(&((*matrPart)[partSize][0]),&(matr[j][0]),sizeof(double)*n);
					++partSize;
				}
				//If this collumn is for another thread
				else {
					#ifndef __SingleProc__
					//Send the global collumn number
					MPI_Send(&j,1,MPI_INT,destinationN,COL_NUM_TAG,cartComm);
					//Send the collumn
					MPI_Send(&(matr[j][0]),n,MPI_DOUBLE,destinationN,COL_TAG,cartComm);
					#endif
				}
			}
		#else //IF THIS IS GAUSSIAN ELIMINATION
			//Send each thread its collumns
			for(int j=0;j<n+1;++j) { 
				//If this collumn is for this thread
				if(colValidForThread(tid,nthreads,n,j)) {
					(*matrPart)[partSize]=new double[n];
					memcpy(&((*matrPart)[partSize][0]),&(matr[j][0]),sizeof(double)*n);
					++partSize;
				}
				//If this collumn is for another thread
				else {
					#ifndef __SingleProc__
					//Send the global collumn number
					MPI_Send(&j,1,MPI_INT,destinationN,COL_NUM_TAG,cartComm);
					//Send the collumn
					MPI_Send(&(matr[j][0]),n,MPI_DOUBLE,destinationN,COL_TAG,cartComm);
					#endif
				}
			}
		#endif
		
		#ifndef __SingleProc__
		//Send the end of data signal and wait for it to come back from behind
		int temp=-1;
		MPI_Send(&temp,1,MPI_INT,destinationN,COL_NUM_TAG,cartComm);
		MPI_Recv(&temp,1,MPI_INT,destinationP,COL_NUM_TAG,cartComm,MPI_STATUS_IGNORE);
		#endif
		
		//Free the memory
		#ifdef __QuestionExtra__ //IF THIS IS COMPUTING THE INVERSE MATRIX
			for(int j=0;j<n*2;++j) {
				delete[] matr[j];
			}
			delete[] matr;
		#else
			for(int j=0;j<n+1;++j) {
				delete[] matr[j];
			}
			delete[] matr;
		#endif
	}
	//For all processes except zero
	else {
		int globColNum;
		double tempBuffer[n];
		//Keep looping until we receive the stop signal
		while(true) {
			MPI_Recv(&globColNum,1,MPI_INT,destinationP,COL_NUM_TAG,cartComm,MPI_STATUS_IGNORE);
			//If the end signal has been received, pass it on and then stop
			if(globColNum<0) {
				MPI_Send(&globColNum,1,MPI_INT,destinationN,COL_NUM_TAG,cartComm);
				break;
			}
			//Else receive the data
			MPI_Recv(tempBuffer,n,MPI_DOUBLE,destinationP,COL_TAG,cartComm,MPI_STATUS_IGNORE);
			//If they are for this thread, keep them
			if(colValidForThread(tid,nthreads,n,globColNum)) {
				(*matrPart)[partSize]=new double[n];
				memcpy(&((*matrPart)[partSize][0]),tempBuffer,sizeof(double)*n);
				++partSize;
			}
			//Else pass them on
			else {
				MPI_Send(&globColNum,1,MPI_INT,destinationN,COL_NUM_TAG,cartComm);
				MPI_Send(tempBuffer,n,MPI_DOUBLE,destinationN,COL_TAG,cartComm);
			}
		}
	}
}

void printMatrix(MPI_Comm cartComm, int tid, int nthreads, int n, double *** matrPart, int partSize) {
	
	//For process 0,
	if(tid==0) {
		//Create the matrix (dynamic creation prevents stack overflow)
		#ifdef __QuestionExtra__ //IF THIS IS COMPUTING THE INVERSE MATRIX
			double ** matr = new double*[n*2];
			for(int j=0;j<n*2;++j) {
				matr[j]=new double[n];
			}
		#else
			double ** matr = new double*[n+1];
			for(int j=0;j<n+1;++j) {
				matr[j]=new double[n];
			}
		#endif
		
		int colnum;
		//Put your data in it
		for(int j=0;j<partSize;++j) {
			colnum=partColToGlobCol(tid,nthreads,n,j);
			memcpy(&(matr[colnum][0]),&((*matrPart)[j][0]),sizeof(double)*n);
		}
		#ifndef __SingleProc__
		//Put everyone else's data in it
		for(int t=1;t<nthreads;++t) {
			while(true) {
				MPI_Recv(&colnum,1,MPI_INT,t,COL_NUM_TAG,cartComm,MPI_STATUS_IGNORE);
				//If this is the end of transmission signal, stop
				if(colnum<0) {
					break;
				}
				//Else keep copying data
				MPI_Recv(&(matr[colnum][0]),n,MPI_DOUBLE,t,COL_TAG,cartComm,MPI_STATUS_IGNORE);
			}
		}
		#endif
		
		//Print once we have everything
		#ifdef __QuestionExtra__ //IF THIS IS COMPUTING THE INVERSE MATRIX
			for(int i=0;i<n;++i) {
				for(int j=0;j<n*2;++j) {
					cout<<setprecision(OUTPUT_PRECISION)<<matr[j][i]<<"\t";
				}
				cout<<endl;
			}
		#else
			for(int i=0;i<n;++i) {
				for(int j=0;j<n+1;++j) {
					cout<<setprecision(OUTPUT_PRECISION)<<matr[j][i]<<"\t";
				}
				cout<<endl;
			}
		#endif
		
		//Free memory
		#ifdef __QuestionExtra__ //IF THIS IS COMPUTING THE INVERSE MATRIX
			for(int j=0;j<n*2;++j) {
				delete[] matr[j];
			}
			delete[] matr;
		#else
			for(int j=0;j<n+1;++j) {
				delete[] matr[j];
			}
			delete[] matr;
		#endif
	}
	//Else, for all other processes
	else {
		int colnum;
		//Send your data to 0 and give him an end signal when you're done
		for(int j=0;j<partSize;++j) {
			colnum=partColToGlobCol(tid,nthreads,n,j);
			MPI_Send(&colnum,1,MPI_INT,0,COL_NUM_TAG,cartComm);
			MPI_Send(&((*matrPart)[j][0]),n,MPI_DOUBLE,0,COL_TAG,cartComm);
		}
		colnum=-1;
		MPI_Send(&colnum,1,MPI_INT,0,COL_NUM_TAG,cartComm);
	}
}

bool colValidForThread(int tid, int nthreads, int n, int col) {
	return tid==threadForCollumn(nthreads,n,col);
}

int threadForCollumn(int nthreads, int n, int col) {
	#ifdef __Question1__
		#ifdef __QuestionExtra__ //IF THIS IS COMPUTING THE INVERSE MATRIX
			int temp=col/(kappa*2);
			if(temp>=nthreads) {
				temp=nthreads-1;
			}
			return temp;
		#else
			int temp=col/kappa;
			if(temp>=nthreads) {
				temp=nthreads-1;
			}
			return temp;
		#endif
	#endif
	#ifdef __Question2__
		int temp=col%nthreads;
		return temp;
	#endif
}

int partColToGlobCol(int tid, int nthreads, int n, int col) {
	#ifdef __Question1__
		#ifdef __QuestionExtra__ //IF THIS IS COMPUTING THE INVERSE MATRIX
			int temp= tid*(kappa*2) + col;
			return temp;
		#else
			int temp= tid*kappa + col;
			return temp;
		#endif
	#endif
	#ifdef __Question2__
		int temp= col*nthreads + tid;
		return temp;
	#endif
}

int globColToPartCol(int tid, int nthreads, int n, int col) {
	#ifdef __Question1__
		#ifdef __QuestionExtra__ //IF THIS IS COMPUTING THE INVERSE MATRIX
			return col%(kappa*2);
		#else
			int temp=col/kappa;
			if(temp>=nthreads) {
				return kappa;
			}
			return col%kappa;
		#endif
	#endif
	#ifdef __Question2__
		int temp=col/nthreads;
		return temp;
	#endif
}
