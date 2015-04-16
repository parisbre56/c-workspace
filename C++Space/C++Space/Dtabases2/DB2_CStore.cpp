#include "DB2_CStore.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <cstdio>
#include <cstring>
#include <cerrno>

using namespace std;

int CS_Errno;

vector<fstream*> files;

void CS_Init()
{
	HPD_Init();
	CS_Errno=HPD_Errno;
}

//Takes a null teminated array of strings that contains the names of all the data members of record
//and creates a collumn store file. The first string is used to name the header_info file.
//Returns 0 for sucess or negative for failure.
int CS_CreateFiles(const char** fieldNames)
{
	CS_Errno=CSE_OK;
	const char** currstring=fieldNames;
	void* temp;
	char currtype;
	int currsize;
	fstream CSinfo;
	int i=-1;
	
	CSinfo.open((string(*fieldNames)+string(".CSDB")).c_str(),ios_base::out|ios_base::binary|ios_base::trunc);
	if(CSinfo.fail())
	{
		CS_Errno=CSE_IOFAIL;
		return CS_Errno;
	}
	
	while(*(++currstring)!=NULL)
	{
		Record().getVariable(&temp,*currstring,&currtype,&currsize);
		if(currsize<0)
		{
			CS_Errno=CSE_INVALIDFIELDNAME;
			CSinfo.close();
			return CS_Errno;
		}
		if(HPD_CreateFile((string("CSFile_")+string(*currstring)).c_str(),currsize)<0)
		{
			CS_Errno=HPD_Errno;
			CSinfo.close();
			return CS_Errno;
		}
		CSinfo.write(
			(string("CSFile_")+string(*currstring)).c_str(),
			(string("CSFile_")+string(*currstring)).size()+1/*NULL TERMINATOR*/);
		CSinfo.write((char*)(&i),sizeof(int));
		if(CSinfo.fail())
		{
			CS_Errno=CSE_IOFAIL;
			CSinfo.close();
			return CS_Errno;
		}
	}
	
	CSinfo.close();
	
	return CS_Errno;
}

//Opens a header_info file and all the files neccessary for the collumn store to function.
//Returns the file descriptor of the opened file for sucess or negative for failure.
int CS_OpenFile(const char* header_info)
{
	CS_Errno=CSE_OK;
	fstream CSinfo;
	fstream* tosave;
	string fname;
	char c;
	int i;
	vector<fstream*>::iterator it;
	
	if(string(string(header_info),string(header_info).size()-5).compare(".CSDB"))
	{
		cerr<<"Warning, non standard file type extension used."<<endl;
	}
	
	CSinfo.open(header_info,ios_base::in|ios_base::out|ios::binary);
	if(CSinfo.fail())
	{
		CS_Errno=CSE_IOFAIL;
		return CS_Errno;
	}
	
	while(!CSinfo.eof())
	{
		//Read the file name
		CSinfo.read(&c,sizeof(char));
		if(CSinfo.eof())
		{
			break;
		}
		if(CSinfo.fail())
		{
			CS_Errno=CSE_IOFAIL;
			CSinfo.close();
			return CS_Errno;
		}
		while(c!='\0')
		{
			fname+=c;
			CSinfo.read(&c,sizeof(char));
			if(CSinfo.fail())
			{
				CS_Errno=CSE_IOFAIL;
				CSinfo.close();
				return CS_Errno;
			}
		}
		
		//Open the file
		if((i=HPD_OpenFile(fname.c_str()))<0)
		{
			CS_Errno=HPD_Errno;
			CSinfo.close();
			return CS_Errno;
		}
				
		//Write the file descriptor in the header_info file
		CSinfo.write((char*)(&i),sizeof(int));
		
		//Clear fname for the next iteration
		fname.erase();
	}
	
	CSinfo.close();
	
	//Add the header info file in the file descriptor vector
	tosave=new fstream;
	tosave->open(header_info,ios_base::in|ios_base::out|ios::binary);
	if(tosave->fail())
	{
		CS_Errno=CSE_IOFAIL;
		return CS_Errno;
	}
	//Search for an empty spot
	for(i=0, it = files.begin(); it != files.end(); ++it, ++i)
	{
		if(*it==NULL)
		{
			*it=tosave;
			return i;
		}
	}
	//If there isn't an empty spot, put it in the end
	files.push_back(tosave);
	return files.size()-1;
}

//Closes a header_info file and all the files that go with it.
//Returns 0 for sucess and negative for failure
int CS_CloseFile(int file)
{
	CS_Errno=CSE_OK;
	char c;
	int i;
	int temppos;
	string fname;
	
	files.at(file)->clear();
	files.at(file)->seekg(0,ios_base::beg);
	//Close all files
	while(!files.at(file)->eof())
	{
		files.at(file)->read(&c,sizeof(char));
		if(files.at(file)->eof())
		{
			break;
		}
		while(c!='\0')
		{
			files.at(file)->read(&c,sizeof(char));
			if(files.at(file)->eof())
			{
				break;
			}
		}
		temppos=files.at(file)->tellg();
		files.at(file)->read((char*)(&i),sizeof(int));
		if(HPD_CloseFile(i)<0)
		{
			CS_Errno=HPD_Errno;
			return CS_Errno;
		}
		files.at(file)->seekp(temppos);
		i=-1;
		files.at(file)->write((char*)(&i),sizeof(int));
	}
	files.at(file)->close();
	
	//Remove file from vector so that the file descriptor can be reused later
	delete files.at(file);
	files.at(file)=NULL;
	
	return CSE_OK;
}

//Inserts <record> in the collumn store file reffered to by <fileDesc>
//Returns 0 for sucess and negative for failure
int CS_InsertEntry(int file, Record record)
{
	CS_Errno=CSE_OK;
	char c;
	int i;
	string fname;
	void* container;
	char type;
	int size;
	
	files.at(file)->clear();
	files.at(file)->seekg(0,ios_base::beg);
	while(!files.at(file)->eof())
	{
		//Read the file name
		files.at(file)->read(&c,sizeof(char));
		if(files.at(file)->eof())
		{
			break;
		}
		if(files.at(file)->fail())
		{
			CS_Errno=CSE_IOFAIL;
			return CS_Errno;
		}
		while(c!='\0')
		{
			fname+=c;
			files.at(file)->read(&c,sizeof(char));
			if(files.at(file)->fail())
			{
				CS_Errno=CSE_IOFAIL;
				return CS_Errno;
			}
		}
		
		//Leave only the variable name in the string
		fname.erase(0,string("CSFile_").size());
		
		//Get the variable
		record.getVariable(&container,fname,&type,&size);
		
		//Store the variable in the appropriate file
		files.at(file)->read((char*)(&i),sizeof(int));
		if(HPD_InsertEntry(i,container))
		{
			CS_Errno=HPD_Errno;
			return CS_Errno;
		}
		
		//Clear fname for the next iteration
		fname.erase();
	}
	
	files.at(file)->clear();
	files.at(file)->seekg(0,ios_base::beg);
	files.at(file)->flush();
	
	return CS_Errno;
}

//Prints the values of fields with names given in the NULL terminated string array <fieldNames>
//of all the records that have a value with name <fieldName> equal to <value> in file <fileDesc>
void CS_GetAllEntries(int file, const char* fieldName, void* value, const char** fieldNames)
{
	CS_Errno=CSE_OK;
	const char** currstring=fieldNames;
	void* container;
	string fname;
	stack<int> rowid;
	int i;
	int j;
	int size;
	char c;
	int rows;
	
	//Print all entries
	if(fieldName==NULL)	
	{
		//Find a file containing the requested value type
		files.at(file)->clear();
		files.at(file)->seekg(0,ios_base::beg);
		currstring=fieldNames;
		while(true)
		{
			//Read next char
			files.at(file)->read(&c,sizeof(char));
			if(files.at(file)->eof())
			{
				files.at(file)->seekg(0,ios_base::beg);
				files.at(file)->read(&c,sizeof(char));
			}
			//End of cycle
			if(c=='\0')
			{
				files.at(file)->read((char*)(&i),sizeof(int));
				if(files.at(file)->eof())
				{
					files.at(file)->seekg(0,ios_base::beg);
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
			CS_Errno=HPD_Errno;
			return;
		}
		//Print all rows
		for(j=0;j<=rows;++j)
		{
			files.at(file)->seekg(0,ios_base::beg);
			currstring=fieldNames;
			cout<<"ROW_"<<j<<": ";
			while(*currstring!=NULL)
			{
				Record().getVariable(&container,*currstring,&c,&size);
				if(container==NULL)
				{
					CS_Errno=CSE_INVALIDFIELDNAME;
					return;
				}
				while(true)
				{
					//Read next char
					files.at(file)->read(&c,sizeof(char));
					if(files.at(file)->eof())
					{
						files.at(file)->seekg(0,ios_base::beg);
						files.at(file)->read(&c,sizeof(char));
					}
					//End of cycle
					if(c=='\0')
					{
						files.at(file)->read((char*)(&i),sizeof(int));
						if(files.at(file)->eof())
						{
							files.at(file)->seekg(0,ios_base::beg);
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
	//Print matching entries
	else
	{
		Record().getVariable(&container,fieldName);
		if(container==NULL)
		{
			CS_Errno=CSE_INVALIDFIELDNAME;
			return;
		}
		
		//Find the file containing the requested value type
		files.at(file)->clear();
		files.at(file)->seekg(0,ios_base::beg);
		currstring=&fieldName;
		while(true)
		{
			//Read next char
			files.at(file)->read(&c,sizeof(char));
			if(files.at(file)->eof())
			{
				files.at(file)->clear();
				files.at(file)->seekg(0,ios_base::beg);
				files.at(file)->read(&c,sizeof(char));
			}
			//End of cycle
			if(c=='\0')
			{
				files.at(file)->read((char*)(&i),sizeof(int));
				if(files.at(file)->eof())
				{
					files.at(file)->clear();
					files.at(file)->seekg(0,ios_base::beg);
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
		//Push row-id's to the stack
		j=-1;
		while(true)
		{
			if((j=HPD_GetRow(i,value,j+1))<0)
			{
				if(j==HPDE_NOTFOUND)
				{
					break;
				}
				else
				{
					CS_Errno=HPD_Errno;
					return;
				}
			}
			rowid.push(j);
		}
	
		while(rowid.size()!=0)
		{
			j=rowid.top();
			rowid.pop();
			files.at(file)->clear();
			files.at(file)->seekg(0,ios_base::beg);
			currstring=fieldNames;
			cout<<"ROW_"<<j<<": ";
			while(*currstring!=NULL)
			{
				Record().getVariable(&container,*currstring,&c,&size);
				if(container==NULL)
				{
					CS_Errno=CSE_INVALIDFIELDNAME;
					return;
				}
				while(true)
				{
					//Read next char
					files.at(file)->read(&c,sizeof(char));
					if(files.at(file)->eof())
					{
						files.at(file)->clear();
						files.at(file)->seekg(0,ios_base::beg);
						files.at(file)->read(&c,sizeof(char));
					}
					//End of cycle
					if(c=='\0')
					{
						files.at(file)->read((char*)(&i),sizeof(int));
						if(files.at(file)->eof())
						{
							files.at(file)->clear();
							files.at(file)->seekg(0,ios_base::beg);
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
				Record().getVariable(&container,*currstring,&c,&size);
				if((container=HPD_GetEntry(i,j))==NULL)
				{
					CS_Errno=HPD_Errno;
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
	
	files.at(file)->clear();
	files.at(file)->seekg(0,ios_base::beg);
	files.at(file)->flush();
}


void CS_PrintError(const char* message, int Errno)
{
	switch(Errno)
	{
		case CSE_OK:
			cerr<<"CSE ERROR: "<<message<<": No error"<<endl;
		break;
		case CSE_INVALIDFIELDNAME:
			cerr<<"CSE ERROR: "<<message<<": Invalid Field Name"<<endl;
		break;
		case CSE_IOFAIL:
			cerr<<"CSE ERROR: "<<message<<": I/O Error"<<strerror(errno)<<endl;
		break;
		case CSE_NOTACSFILE:
			cerr<<"CSE ERROR: "<<message<<": Not a Collumn Store file"<<endl;
		break;
		default:
			HPD_Errno=CS_Errno;
			HPD_PrintError(message);
		break;
	}
}
