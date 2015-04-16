#ifndef __DB1_HASH__
#define __DB1_HASH__

extern "C"
{
	#include "BF.h"
}
#include "DB1_Record.hpp"

//Maximum size an attribute name can have
#define __HT_MAXNAMESIZE__ 15

//Error codes for HT layer
#define HTE_OK						 0
#define HTE_NOTAHASHFILE			-24
#define HTE_UNKNOWNATTR				-26
#define HTE_MALLOCERR				-27

//The last errornum is stored here
extern int HT_Errno;

class HT_info
{
public:
	//Refers to the file used
	int fileDesc;
	//The type of the key attribute, either 'c' or 'i'
	char attrType;
	//The name of the key attribute
	char attrName[__HT_MAXNAMESIZE__];
	//The size of the key attribute (sizeof(<keyAttribute>))
	int attrLength;
	//The depth of the hash table
	int depth;
	//Where the next index is stored
	int nextIndex;
	
	HT_info(void);
	HT_info(int FileDesc, char AttrType, 
		const char* AttrName, int AttrLength, int Depth,int NextIndex);
	void update(int FileDesc, char AttrType, 
		const char* AttrName, int AttrLength, int Depth,int NextIndex);
	//Takes a hash value and returns the appropriate index. Returns NULL for error.
	int* r_hash(int hash);
	//Accesses the index as an array. Returns Null for error. 
	int* i_hash(int pos);
	//Takes a value and produces the appropriate hash value. Returns negative for error;
	int m_hash(const void* value);
	//Writes the index to disk. Returns negative for error;
	int i_update(void);
};

class HT_FirstBlock
{
public:
	int attrType;
	char attrName[__HT_MAXNAMESIZE__];
	int attrLength;
	int depth;
	int nextIndex;
	
	HT_FirstBlock(void);
	HT_FirstBlock(int AttrType,std::string AttrName,int AttrLength,int Depth,int NextIndex);
	void update(int AttrType,std::string AttrName,int AttrLength,int Depth, int NextIndex);
	void update(HT_info* hinfo);
};

class HT_Bucket
{
public:
	int depth;
	int records;
	
	HT_Bucket(void);
	HT_Bucket(int Depth);
};

class HT_Index
{
public:
	int next;
	
	HT_Index(void);
};


//Initialises everything that needs initialising for the hash functions to work.
void HT_Init(void);

//Creates a new hash file with name <fileName>, which will use a key 
//of type <attrType> and name <attrName> of size <attrLength> (sizeof(<keyAttribute>))
//The hash table will have a depth of <depth>
//Returns 0 for success or a negative number for failure
int HT_CreateIndex(char* fileName, 
	char attrType, char* attrName, int attrLength, int depth);

//Opens an already existing hash file with name <filename>
//Returns an HT_info object that serves as the file descriptor 
//or NULL in case of error.
//The user is responsible for deleting the returned object, either
//manually or through the HT_CloseIndex function.
HT_info* HT_OpenIndex(char* fileName);

//Closes the file and, if the file closes successfuly,
//also deletes the HT_info object
//Returns 0 for success or a negative number for failure
int HT_CloseIndex(HT_info* header_info);

//Inserts the <record> into the hash file referred to by <header_info>
//Returns 0 for success or a negative number for failure
int HT_InsertEntry(HT_info* header_info, Record record);

//Prints the entries with a key value of <value> (if such an entry exists) 
//from the heap file reffered to by <header_info>. 
//If <value> is NULL, all records are printed
//Also prints the number of blocks read
//The type of <value> must match the type of the key
void HT_GetAllEntries(HT_info* header_info, const void* value);

//Prints <message> and then the error.
void HT_PrintError(const char * message);

#endif
