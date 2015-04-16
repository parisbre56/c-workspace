#include <iostream>
#include <string>
#include <cstdlib>

#include "DB2_CStore.hpp"
#include "DB2_CSHT.hpp"

#define __VARNAME__ "id"
#define __DisplayRecords__

using namespace std;

int BF_Errno;
int blocksread;

int main(int argc, char **argv)
{
	const char* creation[] = {"test",
						"id",
						"name",
						"surname",
						"status",
						"dateOfBirth",
						"salary",
						"section",
						"daysOff",
						"prevYears",
						NULL};
						
	const char* printing[] = {"id",
						"name",
						"surname",
						NULL};
	
	int CSfile;
	CSHT_info* CSHTfile;
	Record record;
	void* container;
	int i;
	char c;
	
	CS_Init();
	CSHT_Init();
	
	#ifndef __DisplayRecords__
	//Create CS file
	if(CS_CreateFiles(creation)<0)
	{
		CS_PrintError("Error during file creation");
		return CS_Errno;
	}
	#endif
	//Open CS file
	if((CSfile=CS_OpenFile((string(creation[0])+".CSDB").c_str()))<0)
	{
		CS_PrintError("Error when opening file");
		return CS_Errno;
	}
	#ifndef __DisplayRecords__
	//Insert data to CS file
	while(!cin.eof())
	{
		cin>>record.id;
		if(cin.eof())
		{
			break;
		}
		cin>>record.name;
		cin>>record.surname;
		cin>>record.status;
		cin>>record.dateOfBirth;
		cin>>record.salary;
		cin>>record.section;
		cin>>record.daysOff;
		cin>>record.prevYears;
		if(CS_InsertEntry(CSfile,record)<0)
		{
			CS_PrintError("Error when inserting entry");
			return CS_Errno;
		}
	}
	//Create CSHT file
	record.getVariable(&container,__VARNAME__,&c,&i);
	if(container==NULL)
	{
		cerr<<"ERROR: Unknown variable name:"<<__VARNAME__<<endl;
		return EXIT_FAILURE;
	}
	if(CSHT_CreateIndex((string("CSHTFile_")+__VARNAME__).c_str(),c,__VARNAME__,i,3)<0)
	{
		CSHT_PrintError("Error creating CSHT file");
		return CSHT_Errno;
	}
	#endif
	//Open CSHT file
	if((CSHTfile=CSHT_OpenIndex((string("CSHTFile_")+__VARNAME__).c_str()))==NULL)
	{
		CSHT_PrintError("Error opening CSHT file");
		return CSHT_Errno;
	}
	//Update CSHT file
	if(CSHT_InsertEntries(((string(creation[0])+".CSDB").c_str()),CSHTfile)<0)
	{
		CSHT_PrintError("Error updating CSHT file");
		return CSHT_Errno;
	}
	#ifdef __DisplayRecords__
	cin>>i;
	while(!cin.eof())
	{
		blocksread=0;
		cout<<"Reading record with CS method."<<endl;
		if(i<0)
		{
			CS_GetAllEntries(CSfile,NULL,NULL,printing);
			if(CS_Errno<0)
			{
				CS_PrintError("Error printing all entries");
			}
		}
		else
		{
			CS_GetAllEntries(CSfile,"id",&i,printing);
			if(CS_Errno<0)
			{
				CS_PrintError("Error printing entry");
			}
		}
		cout<<"Blocks read: "<<blocksread<<endl;
		blocksread=0;
		cout<<"Reading record with CSHT method."<<endl;
		if(i<0)
		{
			CSHT_GetAllEntries(((string(creation[0])+".CSDB").c_str()),CSHTfile,NULL,printing);
			if(CSHT_Errno<0)
			{
				CSHT_PrintError("Error printing all entries");
			}
		}
		else
		{
			CSHT_GetAllEntries(((string(creation[0])+".CSDB").c_str()),CSHTfile,&i,printing);
			if(CSHT_Errno<0)
			{
				CSHT_PrintError("Error printing entry");
			}
		}
		cout<<"Blocks read: "<<blocksread<<endl;
		cin>>i;
	}
	#endif
	//Close CSHT file
	if(CSHT_CloseIndex(CSHTfile)<0)
	{
		CSHT_PrintError("Error closing CSHT file");
		return CSHT_Errno;
	}
	//Close CS file
	if(CS_CloseFile(CSfile)<0)
	{
		CS_PrintError("Error when closing file");
		return CS_Errno;
	}
	
	return EXIT_SUCCESS;
}
