#ifndef __DB1_HASH__
#define __DB1_HASH__

extern "C"
{
	#include "BF.h"
}
#include <string>

//Maximum size an attribute name can have
#define __HTD_MAXNAMESIZE__ 15

//Error codes for HT layer
#define HTDE_OK						 0
#define HTDE_NOTAHASHFILE			-74
#define HTDE_UNKNOWNATTR			-76
#define HTDE_MALLOCERR				-77
#define HTDE_NOTFOUND				-78

//The last errornum is stored here
extern int HTD_Errno;

class HTD_info
{
public:
	//Refers to the file used
	int fileDesc;
	//The type of the key attribute, either 'c' or 'i'
	char attrType;
	//The name of the key attribute
	char attrName[__HTD_MAXNAMESIZE__];
	//The size of the key attribute (sizeof(<keyAttribute>))
	int attrLength;
	//The depth of the hash table
	int depth;
	//Where the next index is stored
	int nextIndex;
	//The last row-id in use
	int row;
	
	HTD_info(void);
	HTD_info(int FileDesc, char AttrType, 
		const char* AttrName, int AttrLength, int Depth,int NextIndex,int row);
	void update(int FileDesc, char AttrType, 
		const char* AttrName, int AttrLength, int Depth,int NextIndex, int row);
	//Takes a hash value and returns the appropriate index. Returns NULL for error.
	int* r_hash(int hash);
	//Accesses the index as an array. Returns Null for error. 
	int* i_hash(int pos);
	//Takes a value and produces the appropriate hash value. Returns negative for error;
	int m_hash(const void* value);
	//Writes the index to disk. Returns negative for error;
	int i_update(void);
};

class HTD_FirstBlock
{
public:
	int attrType;
	char attrName[__HTD_MAXNAMESIZE__];
	int attrLength;
	int depth;
	int nextIndex;
	int row;
	
	HTD_FirstBlock(void);
	HTD_FirstBlock(int AttrType,std::string AttrName,int AttrLength,int Depth,int NextIndex, int Row);
	void update(int AttrType,std::string AttrName,int AttrLength,int Depth, int NextIndex, int Row);
	void update(HTD_info* hinfo);
};

class HTD_Bucket
{
public:
	int depth;
	int records;
	
	HTD_Bucket(void);
	HTD_Bucket(int Depth);
};

class HTD_Index
{
public:
	int next;
	
	HTD_Index(void);
};


//Initialises everything that needs initialising for the hash functions to work.
void HTD_Init(void);

//Creates a new hash file with name <fileName>, which will use a key 
//of type <attrType> and name <attrName> of size <attrLength> (sizeof(<keyAttribute>))
//The hash table will have a depth of <depth>
//Returns 0 for success or a negative number for failure
int HTD_CreateIndex(const char* fileName, 
	char attrType, const char* attrName, int attrLength, int depth);

//Opens an already existing hash file with name <filename>
//Returns an HTD_info object that serves as the file descriptor 
//or NULL in case of error.
//The user is responsible for deleting the returned object, either
//manually or through the HTD_CloseIndex function.
HTD_info* HTD_OpenIndex(const char* fileName);

//Closes the file and, if the file closes successfuly,
//also deletes the HTD_info object
//Returns 0 for success or a negative number for failure
int HTD_CloseIndex(HTD_info* header_info);

//Inserts the <record> into the hash file referred to by <header_info>
//The record is an integer(row-id) followed by data with sizeof(data)==attrLength
//Returns 0 for success or a negative number for failure
int HTD_InsertEntry(HTD_info* header_info, void* record);

//Returns the row of the first record that matches <value> and has row-id greater then or equal to <row>
//Returns negative for failure (failure includes HTDE_NOTFOUND which means that the requested data
//was not found)
int HTD_GetRow(HTD_info* header_info, const void* value, int row = 0);

//Prints <message> and then the error.
void HTD_PrintError(const char * message);

#endif
