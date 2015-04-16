#include <iostream>
#include <cstdlib>
#include <string>

#include "DB1_Record.hpp"
#include "DB1_Hash.hpp"
#include "DB1_Heap.hpp"

int BF_Errno;

// #define __HEAP__	//Do the heap part
 #define __HASH__	//Do the hash part

#define __MAXRECORDS__	10	//The number of records to be inserted.

using namespace std;

int main(int argc, char **argv)
{
	int temp;
	int file;
	HT_info* fhash;
	int i;
	Record trecord;
	
	//Check args
	if(argc<2)
	{
		cout<<"We need a file name!"<<endl;
		return EXIT_FAILURE;
	}
	
	///////////// HEAP PART //////////////
	
	#ifdef __HEAP__
	//Create or open file
	HP_Init();
	cerr<<"Opening file: "<<argv[1]<<endl;
	if((temp=HP_OpenFile(argv[1]))<0)
	{
		if(HP_Errno==BFE_FILENOTEXISTS)
		{
			if((temp=HP_CreateFile(argv[1]))<0)
			{
				HP_PrintError(argv[1]);
				return EXIT_FAILURE;
			}
			if((temp=HP_OpenFile(argv[1]))<0)
			{
				HP_PrintError(argv[1]);
				return EXIT_FAILURE;
			}
		}
		else
		{
			HP_PrintError(argv[1]);
			return EXIT_FAILURE;
		}
	}
	file=temp;
	//File is now open
	
	//Insert __MAXRECORDS__ test records
	i=1;
	while(i<__MAXRECORDS__)
	{
		trecord.update(i++,"George","Dallas",'S',"12-34-5678",9000,'A',4,7);
		if((temp=HP_InsertEntry(file,trecord))<0)
		{
			HP_PrintError("Error inserting George Dallas");
		}
		trecord.update(i++,"Georgia","Wiley",'M',"87-65-4321",55,'B',7,4);
		if((temp=HP_InsertEntry(file,trecord))<0)
		{
			HP_PrintError("Error inserting Georgia Wiley");
		}
	}
	
	//Test entry retrieval
	HP_GetAllEntries(file,"name","George");
	HP_PrintError("Retreive <name> 'George'");
	HP_GetAllEntries(file,"surname","Wiley");
	HP_PrintError("Retreive <surname> 'Wiley'");
	HP_GetAllEntries(file,NULL,NULL);
	HP_PrintError("Retreive <NULL> NULL");
	temp=55;
	HP_GetAllEntries(file,"salary",&temp);
	HP_PrintError("Retreive <salary> 55");
	HP_GetAllEntries(file,"!!SPACE!!",&temp);
	HP_PrintError("Retreive <!!SPACE!!> 55");
	temp=1;
	HP_GetAllEntries(file,"id",&temp);
	HP_PrintError("Retreive <id> 1");
	
	if((temp=HP_CloseFile(file))<0)
	{
		HP_PrintError(argv[1]);
	}
	
	//File is now closed
	#endif
	
	////////////// HASH PART ///////////////////
	
	#ifdef __HASH__
	//Create or open file
	HT_Init();
	cerr<<"Opening file: "<<argv[1]<<endl;
	if((fhash=HT_OpenIndex(argv[1]))==NULL)
	{
		if(HT_Errno==BFE_FILENOTEXISTS)
		{
			if((temp=HT_CreateIndex(argv[1],'i',"id",sizeof(int),3))<0)
			{
				HT_PrintError(argv[1]);
				return EXIT_FAILURE;
			}
			if((fhash=HT_OpenIndex(argv[1]))==NULL)
			{
				HT_PrintError(argv[1]);
				return EXIT_FAILURE;
			}
		}
		else
		{
			HT_PrintError(argv[1]);
			return EXIT_FAILURE;
		}
	}
	//File is now open
	
	//Insert __MAXRECORDS__ test records
	i=1;
	while(i<__MAXRECORDS__)
	{
		trecord.update(i++,"George","Dallas",'S',"12-34-5678",9000,'A',4,7);
		if((temp=HT_InsertEntry(fhash,trecord))<0)
		{
			HT_PrintError("Error inserting George Dallas");
		}
		trecord.update(i++,"Georgia","Wiley",'M',"87-65-4321",55,'B',7,4);
		if((temp=HT_InsertEntry(fhash,trecord))<0)
		{
			HT_PrintError("Error inserting George Wiley");
		}
	}
	
	//Test entry retrieval
	HT_GetAllEntries(fhash,NULL);
	temp=3;
	HT_GetAllEntries(fhash,&temp);
	
	//Close the file
	if((temp=HT_CloseIndex(fhash))<0)
	{
		HT_PrintError(argv[1]);
	}
	
	#endif
	
	return EXIT_SUCCESS;
}
