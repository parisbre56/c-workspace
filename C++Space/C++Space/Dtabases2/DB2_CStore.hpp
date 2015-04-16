#ifndef __DB2_CStore__
#define __DB2_CStore__

#include "DB2_DHeap.hpp"
#include "DB2_Record.hpp"

extern int CS_Errno;

#define CSE_OK					0
#define CSE_INVALIDFIELDNAME	-40
#define CSE_IOFAIL				-41
#define CSE_NOTACSFILE			-42

void CS_Init();

//Takes a null teminated array of strings that contains the names of all the data members of record
//and creates a collumn store file. The first string is used to name the header_inffo file.
//Returns 0 for sucess or negative for failure.
int CS_CreateFiles(const char** fieldNames);

//Opens a header_info file and all the files neccessary for the collumn store to function.
//Returns the file descriptor of the opened file for sucess or negative for failure.
int CS_OpenFile(const char* header_info);

//Closes a header_info file and all the files that go with it.
//Returns 0 for sucess and negative for failure
int CS_CloseFile(int fileDesc);

//Inserts <record> in the collumn store file reffered to by <fileDesc>
//Returns 0 for sucess and negative for failure
int CS_InsertEntry(int fileDesc, Record record);

//Prints the values of fields with names given in the NULL terminated string array <fieldNames>
//of all the records that have a value with name <fieldName> equal to <value> in file <fileDesc>
//If <value>==NULL, prints all records.
void CS_GetAllEntries(int fileDesc, const char* fieldName, void* value, const char** fieldNames);

//Prints <message> and then the error.
void CS_PrintError(const char * message, int Errno=CS_Errno);

#endif
