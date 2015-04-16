#include "DB2_DHeap.hpp"

#include <cstring>
#include <cstdlib>
#include <iostream>

int HPD_Errno=HPDE_OK;

extern int blocksread;

using namespace std;

HPD_FirstBlock::HPD_FirstBlock()
{
	next=-1;
	last=-1;
	size=0;
	row=-1;
}

HPD_Block::HPD_Block()
{
	next=-1;
	records=0;
}

//Initialises everything that needs initialising for the heap data functions to work.
void HPD_Init(void)
{
	BF_Init();
	HPD_Errno=BF_Errno;
}

//Creates a heap data file with name <fileName> made 
//to contain data with sizeof(data)=size
//Returns 0 for success and negative for failure
int HPD_CreateFile(const char* fileName, unsigned int size)
{
	HPD_Errno=HPDE_OK;
	int file=0;
	HPD_FirstBlock* block;
	void* data;
	
	if(size==0)
	{
		HPD_Errno=HPDE_ZEROSIZE;
		return HPD_Errno;
	}
	
	//Open File
	if(BF_CreateFile(fileName)<0)
	{
		HPD_Errno=BF_Errno;
		return HPD_Errno;
	}
	if((file=BF_OpenFile(fileName))<0)
	{
		HPD_Errno=BF_Errno;
		return HPD_Errno;
	}
	
	//Allocate block
	if(BF_AllocateBlock(file)<0)
	{
		HPD_Errno=BF_Errno;
		BF_CloseFile(file);
		return HPD_Errno;
	}
	
	//Write data to block
	if(BF_ReadBlock(file,0,&data)<0)
	{
		HPD_Errno=BF_Errno;
		BF_CloseFile(file);
		return HPD_Errno;
	}
	++blocksread;
	*((char*)data)='d';
	
	block=new HPD_FirstBlock();
	block->size=size;
	memcpy(((char*)data)+sizeof(char),block,sizeof(HPD_FirstBlock));
	delete block;
	
	if(BF_WriteBlock(file,0)<0)
	{
		HPD_Errno=BF_Errno;
		BF_CloseFile(file);
		return HPD_Errno;
	}
	
	//Close file
	if(BF_CloseFile(file)<0)
	{
		HPD_Errno=BF_Errno;
		return HPD_Errno;
	}
	return HPD_Errno;
}

//Opens a heap data file with name <fileName>
//Returns the file descriptor for success and negative for failure
int HPD_OpenFile(const char* fileName)
{
	HPD_Errno=HPDE_OK;
	int file=0;
	int temp=0;
	void* data;
	
	//Open file
	if((file=BF_OpenFile(fileName))<0)
	{
		HPD_Errno=BF_Errno;
		return HPD_Errno;
	}
	
	//Get number of blocks
	if((temp=BF_GetBlockCounter(file))<0)
	{
		HPD_Errno=BF_Errno;
		BF_CloseFile(file);
		return HPD_Errno;
	}
	
	//A valid heap file should have at least one block.
	if(temp==0)
	{
		HPD_Errno=HPDE_NOTAHEAPDFILE;
		BF_CloseFile(file);
		return HPD_Errno;
	}
	
	//A valid heap data file should have its first byte=='d'
	if(BF_ReadBlock(file,0,&data)<0)
	{
		HPD_Errno=BF_Errno;
		BF_CloseFile(file);
		return HPD_Errno;
	}
	++blocksread;
	if(*((char*)data)!='d')
	{
		HPD_Errno=HPDE_NOTAHEAPDFILE;
		BF_CloseFile(file);
		return HPD_Errno;
	}
	if(((HPD_FirstBlock*)((char*)data+sizeof(char)))->size==0)
	{
		HPD_Errno=HPDE_ZEROSIZE;
		return HPD_Errno;
	}
	
	if(HPD_Errno<0)
	{
		return HPD_Errno;
	}
	else
	{
		return file;
	}
}

//Closes a heap data file with a file descriptor of <fileDesc>
//Returns 0 for success and negative for failure
int HPD_CloseFile(int fileDesc)
{
	HPD_Errno=HPDE_OK;
	void* firstdata;
	
	if(BF_ReadBlock(fileDesc,0,&firstdata)<0)
	{
		HPD_Errno=BF_Errno;
		return HPD_Errno;
	}
	++blocksread;
	if(*((char*)firstdata)!='d')
	{
		HPD_Errno=HPDE_NOTAHEAPDFILE;
		return HPD_Errno;
	}
	
	if(BF_CloseFile(fileDesc)<0)
	{
		HPD_Errno=BF_Errno;
		return HPD_Errno;
	}
	
	return HPD_Errno;
}

//Inserts data <data> in the heap data file reffered to by <fileDesc>
//Returns 0 for success and negative for failure
int HPD_InsertEntry(int file, const void* record)
{
	HPD_Errno=HPDE_OK;
	void* firstdata;
	HPD_FirstBlock* firstblock;
	void* data;
	HPD_Block* block;
	int temp;
	
	//Get data from first block
	if(BF_ReadBlock(file,0,&firstdata)<0)
	{
		HPD_Errno=BF_Errno;
		return HPD_Errno;
	}
	++blocksread;
	//Check for valid heap file
	if(*((char*)firstdata)!='d')
	{
		HPD_Errno=HPDE_NOTAHEAPDFILE;
		return HPD_Errno;
	}
	firstblock=(HPD_FirstBlock*)(((char*)firstdata)+sizeof(char));
	
	//Make sure there is at least one block
	if(firstblock->next<0)
	{
		temp=BF_GetBlockCounter(file);
		if(temp<0)
		{
			HPD_Errno=BF_Errno;
			return HPD_Errno;
		}
		if(temp==1)
		{
			if(BF_AllocateBlock(file)<0)
			{
				HPD_Errno=BF_Errno;
				return HPD_Errno;
			}
			if(BF_ReadBlock(file,1,&data)<0)
			{
				HPD_Errno=BF_Errno;
				return HPD_Errno;
			}
			++blocksread;
			block=new HPD_Block();
			memcpy(data,block,sizeof(HPD_Block));
			delete block;
			if(BF_WriteBlock(file,1)<0)
			{
				HPD_Errno=BF_Errno;
				return HPD_Errno;
			}
		}
		firstblock->next=1;
		firstblock->last=1;
		if(BF_WriteBlock(file,0)<0)
		{
			HPD_Errno=BF_Errno;
			return HPD_Errno;
		}
	}
	
	//Make sure that block has some space in it and if not, go to a new block
	
	//First read the block
	if(BF_ReadBlock(file,firstblock->last,&data)<0)
	{
		HPD_Errno=BF_Errno;
		return HPD_Errno;
	}
	++blocksread;
	block=(HPD_Block*)data;
	//Then allocate a new block if necesary
	if((block->records+1)*(sizeof(int)+firstblock->size)+sizeof(HPD_Block)>BLOCK_SIZE)
	{
		if(BF_AllocateBlock(file)<0)
		{
			HPD_Errno=BF_Errno;
			return HPD_Errno;
		}
		if((temp=BF_GetBlockCounter(file))<0)
		{
			HPD_Errno=BF_Errno;
			return HPD_Errno;
		}
		block->next=temp-1;
		if(BF_WriteBlock(file,firstblock->last)<0)
		{
			HPD_Errno=BF_Errno;
			return HPD_Errno;
		}
		firstblock->last=temp-1;
		if(BF_WriteBlock(file,0)<0)
		{
			HPD_Errno=BF_Errno;
			return HPD_Errno;
		}
		//Finally, read the newly allocated block and give it its data
		if(BF_ReadBlock(file,firstblock->last,&data)<0)
		{
			HPD_Errno=BF_Errno;
			return HPD_Errno;
		}
		++blocksread;
		block=new HPD_Block();
		memcpy(data,block,sizeof(HPD_Block));
		delete block;
		if(BF_WriteBlock(file,firstblock->last)<0)
		{
			HPD_Errno=BF_Errno;
			return HPD_Errno;
		}
		block=(HPD_Block*)data;
	}
	
	//Wirte the record to memory
	memcpy(((char*)data+sizeof(HPD_Block)+block->records*(sizeof(int)+firstblock->size)+sizeof(int)),record,firstblock->size);
	*((int*)((char*)data+sizeof(HPD_Block)+block->records*(sizeof(int)+firstblock->size)))=firstblock->row+1;
	++(firstblock->row);
	++(block->records);
	if(BF_WriteBlock(file,firstblock->last)<0)
	{
		HPD_Errno=BF_Errno;
		return HPD_Errno;
	}
	if(BF_WriteBlock(file,0)<0)
	{
		HPD_Errno=BF_Errno;
		return HPD_Errno;
	}
	
	return HPD_Errno;
}

//Retrieves data with row-id of <row> from heap data file reffered to by <fileDesc>
//Returns NULL for failure. Assumes row-id is unique
void* HPD_GetEntry(int file, int row)
{
	HPD_Errno=HPDE_OK;
	void* data;
	HPD_Block* block;
	HPD_FirstBlock* firstblock;
	int* record;
	unsigned int i;
	int next;
	int size;
	
	//Check for valid heap file
	if(BF_ReadBlock(file,0,&data)<0)
	{
		HPD_Errno=BF_Errno;
		return NULL;
	}
	++blocksread;
	if(*((char*)data)!='d')
	{
		HPD_Errno=HPDE_NOTAHEAPDFILE;
		return NULL;
	}
	firstblock=(HPD_FirstBlock*)((char*)data+sizeof(char));
	
	//Begin searching all the blocks
	size=firstblock->size;
	/*The code below assumes rows are entered serially. Uncomment this code block if that is not the case and comment the one below.
	next=((HPD_FirstBlock*)((char*)data+sizeof(char)))->next;
	while(next!=-1)
	{
		if(BF_ReadBlock(file,next,&data)<0)
		{
			HPD_Errno=BF_Errno;
			return NULL;
		}
		++blocksread;
		//Begin searching all records in this block
		block=(HPD_Block*)data;
		record=(int*)((char*)data+sizeof(HPD_Block));
		for(i=0;i<block->records;++i,record=(int*)((char*)record+sizeof(int)+size))
		{
			if(*record==row)
			{
				record=(int*)((char*)record+sizeof(int));
				return record;
			}
		}
		next=block->next;
	}
	
	return NULL;
	*/
	
	if(BF_ReadBlock(file,(((row>0)?(row):(0))/((BLOCK_SIZE-sizeof(HPD_Block))/(sizeof(int)+size)))+1,&data)<0)
	{
		HPD_Errno=BF_Errno;
		return NULL;
	}
	++blocksread;
	//Begin searching all records in this block
	block=(HPD_Block*)data;
	record=(int*)((char*)data+sizeof(HPD_Block));
	for(i=0;i<block->records;++i,record=(int*)((char*)record+sizeof(int)+size))
	{
		if(*record==row)
		{
			record=(int*)((char*)record+sizeof(int));
			return record;
		}
	}

	return NULL;
}

int HPD_GetRow(int file, const void* search, int row)
{
	HPD_Errno=HPDE_OK;
	void* data;
	HPD_Block* block;
	int* record;
	unsigned int i;
	int next;
	int size;
	int temp;
	
	//Check for valid heap file
	if(BF_ReadBlock(file,0,&data)<0)
	{
		HPD_Errno=BF_Errno;
		return HPD_Errno;
	}
	++blocksread;
	if(*((char*)data)!='d')
	{
		HPD_Errno=HPDE_NOTAHEAPDFILE;
		return HPD_Errno;
	}
	
	size=((HPD_FirstBlock*)((char*)data+sizeof(char)))->size;
	/* The code below assumes rows are entered serially. Uncomment this code block if that is not the case and comment the one below.
	//Begin searching all the blocks
	next=((HPD_FirstBlock*)((char*)data+sizeof(char)))->next;
	while(next!=-1)
	{
		if(BF_ReadBlock(file,next,&data)<0)
		{
			HPD_Errno=BF_Errno;
			return HPD_Errno;
		}
		++blocksread;
		//Begin searching all records in this block
		block=(HPD_Block*)data;
		record=(int*)((char*)data+sizeof(HPD_Block));
		for(i=0;i<block->records;++i,record=(int*)((char*)record+sizeof(int)+size))
		{
			if(*record>=row&&!strncmp((char*)record+sizeof(int),(char*)search,size))
			{
				return *record;
			}
		}
		next=block->next;
	}
	
	return HPDE_NOTFOUND;
	*/
	
	next=(((row>0)?(row):(0))/((BLOCK_SIZE-sizeof(HPD_Block))/(sizeof(int)+size)))+1;
	while(next!=-1)
	{
		if(BF_ReadBlock(file,next,&data)<0)
		{
			HPD_Errno=BF_Errno;
			return HPD_Errno;
		}
		++blocksread;
		//Begin searching all records in this block
		block=(HPD_Block*)data;
		record=(int*)((char*)data+sizeof(HPD_Block));
		for(i=0;i<block->records;++i,record=(int*)((char*)record+sizeof(int)+size))
		{
			if(*record>=row&&!strncmp((char*)record+sizeof(int),(char*)search,size))
			{
				return *record;
			}
		}
		next=block->next;
	}
	
	return HPDE_NOTFOUND;
}

int HPD_GetRows(int file)
{
	void* data;
	
	//Check for valid heap file
	if(BF_ReadBlock(file,0,&data)<0)
	{
		HPD_Errno=BF_Errno;
		return HPD_Errno;
	}
	++blocksread;
	if(*((char*)data)!='d')
	{
		HPD_Errno=HPDE_NOTAHEAPDFILE;
		return HPD_Errno;
	}
	
	return ((HPD_FirstBlock*)((char*)data+sizeof(char)))->row;
}

//Prints <message> and then the error.
void HPD_PrintError(const char * message)
{
	switch(HPD_Errno)
	{
		case HPDE_OK:
			cerr<<"HPDE ERROR: "<<message<<": No error"<<endl;
		break;
		case HPDE_NOTAHEAPDFILE:
			cerr<<"HPDE ERROR: "<<message<<": File is not a heap data file"<<endl;
		break;
		case HPDE_ZEROSIZE:
			cerr<<"HPDE ERROR: "<<message<<": Data size cannot be zero"<<endl;
		break;
		case HPDE_NOTFOUND:
			cerr<<"HPDE ERROR: "<<message<<": No match found to query"<<endl;
		break;
		default:
			BF_Errno=HPD_Errno;
			BF_PrintError(message);
		break;
	}
}
