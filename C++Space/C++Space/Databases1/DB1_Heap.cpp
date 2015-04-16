#include "DB1_Heap.hpp"

#include <cstring>
#include <iostream>

int HP_Errno=HPE_OK;

using namespace std;

HP_Block::HP_Block()
{
	next=-1;
}

HP_FirstBlock::HP_FirstBlock():HP_Block()
{
	last=-1;
}

HP_DataBlock::HP_DataBlock():HP_Block()
{
	records=0;
}

//Initialises everything that needs initialising for the heap functions to work.
void HP_Init(void)
{
	BF_Init();
	HP_Errno=BF_Errno;
}

//Creates a heap file with name <fileName>
//Returns the file descriptor if succesful or negative for failure
int HP_CreateFile(char* fileName)
{
	HP_Errno=HPE_OK;
	int file=0;
	HP_FirstBlock* block;
	void* data;
	
	//Open File
	if(BF_CreateFile(fileName)<0)
	{
		HP_Errno=BF_Errno;
		return HP_Errno;
	}
	if((file=BF_OpenFile(fileName))<0)
	{
		HP_Errno=BF_Errno;
		return HP_Errno;
	}
	
	//Allocate block
	if(BF_AllocateBlock(file)<0)
	{
		HP_Errno=BF_Errno;
		BF_CloseFile(file);
		return HP_Errno;
	}
	
	//Write data to block
	if(BF_ReadBlock(file,0,&data)<0)
	{
		HP_Errno=BF_Errno;
		BF_CloseFile(file);
		return HP_Errno;
	}
	*((char*)data)='p';
	
	block=new HP_FirstBlock();
	memcpy(((char*)data)+sizeof(char),block,sizeof(HP_FirstBlock));
	delete block;
	
	if(BF_WriteBlock(file,0)<0)
	{
		HP_Errno=BF_Errno;
		BF_CloseFile(file);
		return HP_Errno;
	}
	
	//Close file
	if(BF_CloseFile(file)<0)
	{
		HP_Errno=BF_Errno;
		return HP_Errno;
	}
	return HP_Errno;
}


//Opens a heap file with name <fileName>
//Returns 0 for success and negative for failure
int HP_OpenFile(char* fileName)
{
	HP_Errno=HPE_OK;
	int file=0;
	int temp=0;
	void* data;
	
	//Open file
	if((file=BF_OpenFile(fileName))<0)
	{
		HP_Errno=BF_Errno;
		return HP_Errno;
	}
	
	//Get number of blocks
	if((temp=BF_GetBlockCounter(file))<0)
	{
		HP_Errno=BF_Errno;
		BF_CloseFile(file);
		return HP_Errno;
	}
	
	//A valid heap file should have at least one block.
	if(temp==0)
	{
		HP_Errno=HPE_NOTAHEAPFILE;
		BF_CloseFile(file);
		return HP_Errno;
	}
	
	//A valid heap file should have its first byte=='p'
	if(BF_ReadBlock(file,0,&data)<0)
	{
		HP_Errno=BF_Errno;
		BF_CloseFile(file);
		return HP_Errno;
	}
	if(*((char*)data)!='p')
	{
		HP_Errno=HPE_NOTAHEAPFILE;
		BF_CloseFile(file);
		return HP_Errno;
	}
	
	if(HP_Errno<0)
	{
		return HP_Errno;
	}
	else
	{
		return file;
	}
}


//Closes a heap file with a file descriptor of <fileDesc>
//Returns 0 for success and negative for failure
int HP_CloseFile(int file)
{
	HP_Errno=HPE_OK;
	void* firstdata;
	
	if(BF_ReadBlock(file,0,&firstdata)<0)
	{
		HP_Errno=BF_Errno;
		return HP_Errno;
	}
	if(*((char*)firstdata)!='p')
	{
		HP_Errno=HPE_NOTAHEAPFILE;
		return HP_Errno;
	}
	
	if(BF_CloseFile(file)<0)
	{
		HP_Errno=BF_Errno;
		return HP_Errno;
	}
	
	return HP_Errno;
}

	
//Inserts record <record> in heap file reffered to by <fileDesc>
//Returns 0 for success and negative for failure
int HP_InsertEntry(int file, Record record)
{
	HP_Errno=HPE_OK;
	void* firstdata;
	HP_FirstBlock* firstblock;
	void* data;
	HP_DataBlock* block;
	int temp;
	
	//Get data from first block
	if(BF_ReadBlock(file,0,&firstdata)<0)
	{
		HP_Errno=BF_Errno;
		return HP_Errno;
	}
	//Check for valid heap file
	if(*((char*)firstdata)!='p')
	{
		HP_Errno=HPE_NOTAHEAPFILE;
		return HP_Errno;
	}
	firstblock=(HP_FirstBlock*)(((char*)firstdata)+sizeof(char));
	
	//Make sure there is at least one block
	if(firstblock->next<0)
	{
		temp=BF_GetBlockCounter(file);
		if(temp<0)
		{
			HP_Errno=BF_Errno;
			return HP_Errno;
		}
		if(temp==1)
		{
			if(BF_AllocateBlock(file)<0)
			{
				HP_Errno=BF_Errno;
				return HP_Errno;
			}
			if(BF_ReadBlock(file,1,&data)<0)
			{
				HP_Errno=BF_Errno;
				return HP_Errno;
			}
			block=new HP_DataBlock();
			memcpy(data,block,sizeof(HP_DataBlock));
			delete block;
		}
		firstblock->next=1;
		firstblock->last=1;
		if(BF_WriteBlock(file,0)<0)
		{
			HP_Errno=BF_Errno;
			return HP_Errno;
		}
	}
	
	//Make sure that block has some space in it and if not, go to a new block
	
	//First read the block
	if(BF_ReadBlock(file,firstblock->last,&data)<0)
	{
		HP_Errno=BF_Errno;
		return HP_Errno;
	}
	block=(HP_DataBlock*)data;
	//Then allocate a new block if necesary
	if((block->records+1)*sizeof(Record)+sizeof(HP_DataBlock)>BLOCK_SIZE)
	{
		if(BF_AllocateBlock(file)<0)
		{
			HP_Errno=BF_Errno;
			return HP_Errno;
		}
		if((temp=BF_GetBlockCounter(file))<0)
		{
			HP_Errno=BF_Errno;
			return HP_Errno;
		}
		block->next=temp-1;
		if(BF_WriteBlock(file,firstblock->last)<0)
		{
			HP_Errno=BF_Errno;
			return HP_Errno;
		}
		firstblock->last=temp-1;
		if(BF_WriteBlock(file,0)<0)
		{
			HP_Errno=BF_Errno;
			return HP_Errno;
		}
		//Finally, read the newly allocated block and give it its data
		if(BF_ReadBlock(file,firstblock->last,&data)<0)
		{
			HP_Errno=BF_Errno;
			return HP_Errno;
		}
		block=new HP_DataBlock();
		memcpy(data,block,sizeof(HP_DataBlock));
		delete block;
		if(BF_WriteBlock(file,firstblock->last)<0)
		{
			HP_Errno=BF_Errno;
			return HP_Errno;
		}
		block=(HP_DataBlock*)data;
	}
	
	//Wirte the record to memory
	memcpy(((char*)data+sizeof(HP_DataBlock)+block->records*sizeof(Record)),&record,sizeof(Record));
	block->records+=1;
	if(BF_WriteBlock(file,firstblock->last)<0)
	{
		HP_Errno=BF_Errno;
		return HP_Errno;
	}
	
	return HP_Errno;
}


//Prints all records that have the field with name <fieldname>
//equal to value <value>. If <fieldName> is NULL, all records are printed
//Also prints the number of blocks read
//The type of <value> must match the type of the key
void HP_GetAllEntries(int file,char* fieldName,const void* value)
{
	HP_Errno=HPE_OK;
	int blocks=1;
	void* data;
	HP_DataBlock* block;
	Record* record;
	int i;
	void* container;
	char type;
	int length;
	
	//Check for valid field name
	if(fieldName!=NULL)
	{
		Record().getVariable(&container,fieldName,&type,&length);
		if(container==NULL)
		{
			//This is not a valid fieldName. No reason to continue.
			cout<<fieldName<<" is not a valid Field Name"<<endl;
			return;
		}
	}
	
	//Check for valid heap file
	if(BF_ReadBlock(file,0,&data)<0)
	{
		HP_Errno=BF_Errno;
		return;
	}
	if(*((char*)data)!='p')
	{
		HP_Errno=HPE_NOTAHEAPFILE;
		return;
	}
	
	//Begin searching all the blocks
	block=(HP_DataBlock*)((char*)data+sizeof(char));
	while(block->next!=-1)
	{
		if(BF_ReadBlock(file,block->next,&data)<0)
		{
			HP_Errno=BF_Errno;
			return;
		}
		++blocks;
		
		//Begin searching all records in this block
		block=(HP_DataBlock*)data;
		record=(Record*)((char*)data+sizeof(HP_DataBlock));
		for(i=0;i<block->records;++i,++record)
		{
			if(fieldName==NULL)
			{
				record->print();
			}
			else
			{
				record->getVariable(&container,fieldName,&type,&length);
				if(type=='i'&&(*(int*)container==*(int*)value))
				{
					record->print();
				}
				else if(type=='c'&&!strncmp((char*)value,(char*)container,length))
				{
					record->print();
				}
			}
		}
	}
	cout<<"-- Search Complete! --"<<endl<<"Blocks read:"<<blocks<<endl;
	return;
}


//Prints <message> and then the error.
void HP_PrintError(const char * message)
{
	switch(HP_Errno)
	{
		case HPE_OK:
			cerr<<"HPE ERROR: "<<message<<": No error"<<endl;
		break;
		case HPE_NOTAHEAPFILE:
			cerr<<"HPE ERROR: "<<message<<": File is not a heap file"<<endl;
		break;
		default:
			BF_Errno=HP_Errno;
			BF_PrintError(message);
		break;
	}
}

