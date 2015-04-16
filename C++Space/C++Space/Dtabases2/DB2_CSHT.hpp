#ifndef __DB2__CSHT__
#define __DB2__CSHT__

#include "DB2_CStore.hpp"
#include "DB2_Record.hpp"
#include "DB2_DHash.hpp"
#include "DB2_DHeap.hpp"
extern "C"
{
	#include "BF.h"
}

extern int CSHT_Errno;

#define CSHTE_OK					0
#define CSHTE_IOFAIL				-90
#define CSHTE_INVALIDATTRNAME		-91

class CSHT_info{
public:
	HTD_info* file;
	char attrType;
	char attrName[__HTD_MAXNAMESIZE__];
	int attrLength;
	int depth;
	int row;
	
	CSHT_info();
	void update(HTD_info* fileDesc, int attrType, const char* attrName, int attrLength, int depth, int row);
	void update(HTD_info* file);
};

void CSHT_Init();

//Creates a CSHTFile with name <fileName> and a depth of <depth> from a CSFile with name CSFile_<attrName>  
//Returns 0 for success and negative for failure
int CSHT_CreateIndex(const char* fileName, char attrType, const char* attrName, int attrLength, int depth);

//Opens a Collumn Store hash table file wih name <fileName>
//Returns a file info object or NULL for failure
CSHT_info* CSHT_OpenIndex(const char* fileName);

//Closes the file reffered to by <header_info>
//Returns 0 for success and negative for failure
int CSHT_CloseIndex(CSHT_info* header_info);

//Updates the hash file referrd to by <header_info> with
//any new records from CSFile_attrName described in <header_filename>
//Returns 0 for success and negative for failure
int CSHT_InsertEntries(const char* header_filename, CSHT_info* header_info);

//Prints the values of fields with names given in the NULL terminated string array <fieldNames>
//of all the records that have a value equal to <value> in the hash table <header_info>
//If <value>==NULL, prints all records.
void CSHT_GetAllEntries(const char* header_filename, CSHT_info* header_info, void* value, const char** fieldNames);

//Prints <message> and then the error.
void CSHT_PrintError(const char * message, int Errno=CSHT_Errno);

#endif
