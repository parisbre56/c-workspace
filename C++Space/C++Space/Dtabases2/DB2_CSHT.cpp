#include "DB2_CSHT.hpp"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <fstream>
#include <stack>
#include <cerrno>

using namespace std;

int CSHT_Errno=CSHTE_OK;

CSHT_info::CSHT_info()
{
	file=NULL;
	attrType='\0';
	attrName[0]='\0';
	attrLength=-1;
	depth=-1;
	row=-1;
}

void CSHT_info::update(HTD_info* File,int AttrType, const char* AttrName, int AttrLength, int Depth, int Row)
{
	file=File;
	attrType=AttrType;
	strncpy(attrName,AttrName,sizeof(attrName));
	attrLength=AttrLength;
	depth=Depth;
	row=Row;
}

void CSHT_info::update(HTD_info* hinfo)
{
	file=hinfo;
	attrType=hinfo->attrType;
	strncpy(attrName,hinfo->attrName,sizeof(attrName));
	attrLength=hinfo->attrLength;
	depth=hinfo->depth;
	row=hinfo->row;
}

void CSHT_Init()
{
	HTD_Init();
	CSHT_Errno=HTD_Errno;
}

//Creates a CSHTFile with name <fileName> and a depth of <depth> from a CSFile with name CSFile_<attrName>  
//Returns 0 for success and negative for failure
int CSHT_CreateIndex(const char* fileName, char attrType, const char* attrName, int attrLength, int depth)
{
	CSHT_Errno=CSHTE_OK;
	void* container;
	
	//Check for valid attrName
	Record().getVariable(&container,attrName);
	if(container==NULL)
	{
		CSHT_Errno=CSHTE_INVALIDATTRNAME;
		return CSHT_Errno;
	}
	//Create index
	if(HTD_CreateIndex(fileName,attrType,attrName,attrLength,depth)<0)
	{
		CSHT_Errno=HTD_Errno;
	}
	return CSHT_Errno;
}

//Opens a Collumn Store hash table file wih name <fileName>
//Returns a file info object or NULL for failure
CSHT_info* CSHT_OpenIndex(const char* fileName)
{
	CSHT_Errno=CSHTE_OK;
	HTD_info* tempc;
	CSHT_info* temph;
	if((tempc=HTD_OpenIndex(fileName))==NULL)
	{
		CSHT_Errno=HTD_Errno;
		return NULL;
	}
	temph=new CSHT_info;
	temph->update(tempc);
	return temph;
}

//Closes the file reffered to by <header_info>
//Returns 0 for success and negative for failure
int CSHT_CloseIndex(CSHT_info* header_info)
{
	CSHT_Errno=CSHTE_OK;
	if(HTD_CloseIndex(header_info->file)<0)
	{
		CSHT_Errno=HTD_Errno;
		return CSHT_Errno;
	}
	delete header_info;
	return CSHT_Errno;
}

//Updates the hash file referrd to by <header_info> with
//any new records from CSFile_attrName described in <header_filename>
//Returns 0 for success and negative for failure
int CSHT_InsertEntries(const char* header_filename, CSHT_info* header_info)
{
	CSHT_Errno=CSHTE_OK;
	ifstream CSinfo;
	string fname;
	char c;
	char record[sizeof(int)+header_info->attrLength];
	void* data;
	int i;
	int j;
	
	//Open the CSinfo file
	if(string(string(header_filename),string(header_filename).size()-5).compare(".CSDB"))
	{
		cerr<<"Warning, non standard file type extension used."<<endl;
	}
	CSinfo.open(header_filename,ios_base::binary|ios_base::in);
	if(CSinfo.fail())
	{
		CSHT_Errno=CSHTE_IOFAIL;
		return CSHT_Errno;
	}
	
	CSinfo.seekg(0,ios_base::beg);
	
	//Find CSFile_attrName
	while(true)
	{
		//Read the file name
		CSinfo.read(&c,sizeof(char));
		if(CSinfo.fail())
		{
			CSHT_Errno=CSHTE_IOFAIL;
			CSinfo.close();
			return CSHT_Errno;
		}
		while(c!='\0')
		{
			fname+=c;
			CSinfo.read(&c,sizeof(char));
			if(CSinfo.fail())
			{
				CSHT_Errno=CSHTE_IOFAIL;
				CSinfo.close();
				return CSHT_Errno;
			}
		}
		//Get the files' fileDesc
		CSinfo.read((char*)(&i),sizeof(int));
		if(CSinfo.fail())
		{
			CSHT_Errno=CSHTE_IOFAIL;
			CSinfo.close();
			return CSHT_Errno;
		}
		//If it's the one we wanted, exit
		if(!fname.compare(string(string("CSFile_")+string(header_info->attrName))))
		{
			break;
		}
		fname.erase();
	}
	//Get the number of rows in CSFile_attrName
	if((j=HPD_GetRows(i))<0)
	{
		CSHT_Errno=HPD_Errno;
		return CSHT_Errno;
	}
	//For all rows greater than header_info->row
	for(int k=header_info->row+1;k<=j;++k)
	{
		//Copy data
		if((data=HPD_GetEntry(i,k))==NULL)
		{
			CSHT_Errno=HPD_Errno;
			return CSHT_Errno;
		}
		memcpy((char*)record,(char*)(&k),sizeof(int));
		memcpy(((char*)record)+sizeof(int),(char*)data,header_info->attrLength);
		//insert them to the hash file
		if(HTD_InsertEntry(header_info->file,record)<0)
		{
			CSHT_Errno=HTD_Errno;
			return CSHT_Errno;
		}
	}
	
	header_info->update(header_info->file);
	CSinfo.close();
	
	return CSHT_Errno;
}

//Prints the values of fields with names given in the NULL terminated string array <fieldNames>
//of all the records that have a value equal to <value> in the hash table <header_info>
//If <value>==NULL, prints all records.
void CSHT_GetAllEntries(const char* header_filename, CSHT_info* header_info, void* value, const char** fieldNames)
{
	CSHT_Errno=CSHTE_OK;
	ifstream CSinfo;
	const char** currstring=fieldNames;
	void* container;
	string fname;
	stack<int> rowid;
	int i;
	int j;
	int size;
	char c;
	int rows;
	
	CSinfo.open(header_filename,ios_base::binary|ios_base::in);
	
	if(value==NULL)
	{
		//Find a file containing the requested value type
		CSinfo.clear();
		CSinfo.seekg(0,ios_base::beg);
		currstring=fieldNames;
		while(true)
		{
			//Read next char
			CSinfo.read(&c,sizeof(char));
			if(CSinfo.eof())
			{
				CSinfo.clear();
				CSinfo.seekg(0,ios_base::beg);
				CSinfo.read(&c,sizeof(char));
			}
			//End of cycle
			if(c=='\0')
			{
				CSinfo.read((char*)(&i),sizeof(int));
				if(CSinfo.eof())
				{
					CSinfo.clear();
					CSinfo.seekg(0,ios_base::beg);
				}
				//Leave only the variable name in the string
				fname.erase(0,string("CSFile_").size());
				//Check if it's one the requested name
				if(!fname.compare(*currstring))
				{
					fname.erase();
					break;
				}
				//Erase file name for next cycle
				fname.erase();
			}
			//Put char in string
			fname+=c;
		}
		//Get the number of rows in the file
		if((rows=HPD_GetRows(i))<0)
		{
			CSHT_Errno=HPD_Errno;
			return;
		}
		//Print all rows
		for(j=0;j<=rows;++j)
		{
			CSinfo.clear();
			CSinfo.seekg(0,ios_base::beg);
			currstring=fieldNames;
			cout<<"ROW_"<<j<<": ";
			while(*currstring!=NULL)
			{
				Record().getVariable(&container,*currstring,&c,&size);
				if(container==NULL)
				{
					CSHT_Errno=CSHTE_INVALIDATTRNAME;
					return;
				}
				while(true)
				{
					//Read next char
					CSinfo.read(&c,sizeof(char));
					if(CSinfo.eof())
					{
						CSinfo.clear();
						CSinfo.seekg(0,ios_base::beg);
						CSinfo.read(&c,sizeof(char));
					}
					//End of cycle
					if(c=='\0')
					{
						CSinfo.read((char*)(&i),sizeof(int));
						if(CSinfo.eof())
						{
							CSinfo.clear();
							CSinfo.seekg(0,ios_base::beg);
						}
						//Leave only the variable name in the string
						fname.erase(0,string("CSFile_").size());
						//Check if it's one of the requested names
						if(!fname.compare(*currstring))
						{
							fname.erase();
							break;
						}
						//Erase file name for next cycle
						fname.erase();
					}
					//Put char in string
					fname+=c;
				}
				//Get the requested data
				Record().getVariable(&container,*currstring,&c);
				container=HPD_GetEntry(i,j);
				if(c=='c')
				{
					cout<<*currstring<<": "<<string((char*)container)<<' ';
				}
				else
				{
					cout<<*currstring<<": "<<*(int*)container<<' ';
				}
				++currstring;
			}
			cout<<endl;
		}
	}
	else
	{
		Record().getVariable(&container,header_info->attrName);
		if(container==NULL)
		{
			CSHT_Errno=CSHTE_INVALIDATTRNAME;
			return;
		}
		
		//Push row-id's to the stack
		j=-1;
		while(true)
		{
			if((j=HTD_GetRow(header_info->file,value,j+1))<0)
			{
				if(j==HTDE_NOTFOUND)
				{
					break;
				}
				else
				{
					CSHT_Errno=HPD_Errno;
					return;
				}
			}
			rowid.push(j);
		}
	
		while(rowid.size()!=0)
		{
			j=rowid.top();
			rowid.pop();
			CSinfo.clear();
			CSinfo.seekg(0,ios_base::beg);
			currstring=fieldNames;
			cout<<"ROW_"<<j<<": ";
			while(*currstring!=NULL)
			{
				Record().getVariable(&container,*currstring,&c,&size);
				if(container==NULL)
				{
					CSHT_Errno=CSHTE_INVALIDATTRNAME;
					return;
				}
				while(true)
				{
					//Read next char
					CSinfo.read(&c,sizeof(char));
					if(CSinfo.eof())
					{
						CSinfo.clear();
						CSinfo.seekg(0,ios_base::beg);
						CSinfo.read(&c,sizeof(char));
					}
					//End of cycle
					if(c=='\0')
					{
						CSinfo.read((char*)(&i),sizeof(int));
						if(CSinfo.eof())
						{
							CSinfo.clear();
							CSinfo.seekg(0,ios_base::beg);
						}
						//Leave only the variable name in the string
						fname.erase(0,string("CSFile_").size());
						//Check if it's one of the requested names
						if(!fname.compare(*currstring))
						{
							fname.erase();
							break;
						}
						//Erase file name for next cycle
						fname.erase();
					}
					//Put char in string
					fname+=c;
				}
				//Get the requested data
				Record().getVariable(&container,*currstring,&c);
				if((container=HPD_GetEntry(i,j))==NULL)
				{
					CSHT_Errno=HPD_Errno;
					return;
				}
				if(c=='c')
				{
					cout<<*currstring<<": "<<string((char*)container)<<' ';
				}
				else
				{
					cout<<*currstring<<": "<<*(int*)container<<' ';
				}
				++currstring;
			}
			cout<<endl;
		}
	}
	
	
	CSinfo.close();
}

//Prints <message> and then the error.
void CSHT_PrintError(const char * message, int Errno)
{
	switch(Errno)
	{
		case CSHTE_OK:
			cerr<<"CSHTE Error: "<<message<<": No error"<<endl;
		break;
		case CSHTE_IOFAIL:
			cerr<<"CSHTE ERROR: "<<message<<": I/O Error"<<strerror(errno)<<endl;
		break;
		case CSHTE_INVALIDATTRNAME:
			cerr<<"CSHTE ERROR: "<<message<<": Invalid Field Name"<<endl;
		break;
		default:
			if(Errno>=-78&&Errno<-70)
			{
				HTD_Errno=Errno;
				HTD_PrintError(message);
			}
			else
			{
				HPD_Errno=Errno;
				HPD_PrintError(message);
			}
		break;
	}
}
