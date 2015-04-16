#ifndef __DB1_HEAP__
#define __DB1_HEAP__

extern "C"
{
	#include "BF.h"
}
#include "DB1_Record.hpp"

//Error codes for HT layer
#define HPE_OK						 0
#define HPE_NOTAHEAPFILE			-25

class HP_Block
{
public:
	//The number of the next block
	int next;
	
	HP_Block();
};

class HP_FirstBlock : public HP_Block
{
public:
	//The number of the last block
	int last;
	
	HP_FirstBlock();
};

class HP_DataBlock : public HP_Block
{
public:
	//The number of records inside this block
	int records;
	
	HP_DataBlock();
};

//The last errornum is stored here
extern int HP_Errno;

//Initialises everything that needs initialising for the heap functions to work.
void HP_Init(void);

//Creates a heap file with name <fileName>
//Returns 0 for success and negative for failure
int HP_CreateFile(char* fileName);

//Opens a heap file with name <fileName>
//Returns 0 for success and negative for failure
int HP_OpenFile(char* fileName);

//Closes a heap file with a file descriptor of <fileDesc>
//Returns 0 for success and negative for failure
int HP_CloseFile(int fileDesc);

//Inserts record <record> in heap file reffered to by <fileDesc>
//Returns 0 for success and negative for failure
int HP_InsertEntry(int fileDesc, Record record);

//Prints all records that have the field with name <fieldname>
//equal to value <value>. If <fieldName> is NULL, all records are printed
//Also prints the number of blocks read
//The type of <value> must match the type of the key
void HP_GetAllEntries(int fileDesc,char* fieldName,const void* value);

//Prints <message> and then the error.
void HP_PrintError(const char * message);

#endif
