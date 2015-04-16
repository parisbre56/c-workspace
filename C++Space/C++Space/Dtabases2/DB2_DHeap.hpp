#ifndef __DB2_DHeap__
#define __DB2_DHeap__

extern "C"
{
	#include "BF.h"
}

//Error codes for HT layer
#define HPDE_OK						 0
#define HPDE_NOTAHEAPDFILE			-30
#define HPDE_ZEROSIZE				-31
#define HPDE_NOTFOUND				-32

class HPD_FirstBlock
{
public:
	//The number of the next block
	int next;
	//The number of the last block
	int last;
	//The size of the data held in the heap
	unsigned int size;
	//The last row-id in use
	int row;
	
	HPD_FirstBlock();
};

class HPD_Block
{
public:
	//The number of the next block
	int next;
	//The numbe of records held in this block
	unsigned int records;
	
	HPD_Block();
};

//The last errornum is stored here
extern int HPD_Errno;

//Initialises everything that needs initialising for the heap data functions to work.
void HPD_Init(void);

//Creates a heap data file with name <fileName> made 
//to contain data with sizeof(data)=size
//Returns 0 for success and negative for failure
int HPD_CreateFile(const char* fileName, unsigned int size);

//Opens a heap data file with name <fileName>
//Returns the file descriptor for success and negative for failure
int HPD_OpenFile(const char* fileName);

//Closes a heap data file with a file descriptor of <fileDesc>
//Returns 0 for success and negative for failure
int HPD_CloseFile(int fileDesc);

//Inserts data <data> in the heap data file reffered to by <fileDesc>
//Returns 0 for success and negative for failure
int HPD_InsertEntry(int fileDesc,const void* data);

//Retrieves data with row-id of <row> from heap data file reffered to by <fileDesc>
//Returns NULL for failure
void* HPD_GetEntry(int fileDesc, int row);

//Returns the first row-id which is greater then or equal to <row> that matches <data> in
//the heap data file reffered to by <fileDesc>
//Returns negative for failure (failure includes HPDE_NOTFOUND which means that the requested data
//was not found)
int HPD_GetRow(int fileDesc, const void* data, int row = 0);

//Returns the number of rows in the file or negative for failure
int HPD_GetRows(int fileDesc);

//Prints <message> and then the error.
void HPD_PrintError(const char * message);

#endif
