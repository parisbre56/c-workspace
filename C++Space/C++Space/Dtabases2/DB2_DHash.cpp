#include "DB2_DHash.hpp"

#include <cstring>
#include <iostream>
#include <string>
#include <cmath>
#include <climits>
#include <cstdlib>
#include <queue>

int HTD_Errno=HTDE_OK;

extern int blocksread;

using namespace std;

//Reverses the last <NO_OF_BITS> bits of number num
int reverseBits(int num, unsigned int NO_OF_BITS = sizeof(int) * sizeof(char))
{
	unsigned int reverse_num = 0;
	for (unsigned int i = 0; i < NO_OF_BITS; ++i)
	{
		if((num & (1 << i)))
		{
			reverse_num |= 1 << ((NO_OF_BITS - 1) - i);
		}
	}
	return reverse_num;
}

HTD_info::HTD_info(void)
{
	fileDesc=-1;
	attrType='\0';
	attrName[0]='\0';
	attrLength=-1;
	depth=-1;
	nextIndex=-1;
	row=-1;
}

HTD_info::HTD_info(int FileDesc, char AttrType, 
	const char* AttrName, int AttrLength, int Depth, int NextIndex, int Row)
{
	fileDesc=FileDesc;
	attrType=AttrType;
	strncpy(attrName,AttrName,sizeof(attrName));
	attrLength=AttrLength;
	depth=Depth;
	nextIndex=NextIndex;
	row=Row;
}

void HTD_info::update(int FileDesc, char AttrType, 
	const char* AttrName, int AttrLength, int Depth, int NextIndex, int Row)
{
	fileDesc=FileDesc;
	attrType=AttrType;
	strncpy(attrName,AttrName,sizeof(attrName));
	attrLength=AttrLength;
	depth=Depth;
	nextIndex=NextIndex;
	row=Row;
}

//Takes a hash value and returns the appropriate index. Returns Null for error.
int* HTD_info::r_hash(int hash)
{
	int temp;
	
	temp = hash & ((1<<depth)-1);
	temp = reverseBits(temp,depth);
	
	return i_hash(temp);
}
//Accesses the index as an array. Returns Null for error.
int* HTD_info::i_hash(int pos)
{
	HTD_Errno=HTDE_OK;
	int temp = pos & ((1<<depth)-1);
	void* data;
	int* t_index;
	HTD_Index* index;
	
	//Check if the requested index is in the first block
	if(temp<(BLOCK_SIZE-sizeof(char)-sizeof(HTD_FirstBlock))/sizeof(int))
	{
		if(BF_ReadBlock(fileDesc,0,&data)<0)
		{
			HTD_Errno=BF_Errno;
			return NULL;
		}
		++blocksread;
		t_index=(int*)((char*)data+sizeof(char)+sizeof(HTD_FirstBlock));
		return &(t_index[temp]);
	}
	
	//If it isn't, remove all the indexes we have already searched from temp
	temp=temp-((BLOCK_SIZE-sizeof(char)-sizeof(HTD_FirstBlock))/sizeof(int));
	if(BF_ReadBlock(fileDesc,nextIndex,&data)<0)
	{
		HTD_Errno=BF_Errno;
		return NULL;
	}
	++blocksread;
	index=(HTD_Index*)data;
	//Then start searching the next indexes
	while (temp>=(BLOCK_SIZE-sizeof(HTD_Index))/sizeof(int))
	{
		temp=temp-((BLOCK_SIZE-sizeof(HTD_Index))/sizeof(int));
		if(BF_ReadBlock(fileDesc,index->next,&data)<0)
		{
			HTD_Errno=BF_Errno;
			return NULL;
		}
		++blocksread;
		index=(HTD_Index*)data;
	}
	t_index=(int*)((char*)data+sizeof(HTD_Index));
	return &(t_index[temp]);
}

int HTD_info::m_hash(const void* value)
{
	HTD_Errno=HTDE_OK;
	if(attrType=='c')
	{
		string s((char*) value,attrLength);
		int intLength = s.length() / 4;
		long sum = 0;
		for (int j = 0; j < intLength; j++) 
		{
			string c(s,j * 4, (j * 4) + 4);
			long mult = 1;
			for (int k = 0; k < c.length() ; k++) 
			{
				sum += c[k] * mult;
				mult *= 256;
			}
		}

		string c(s,intLength * 4);
		long mult = 1;
		for (int k = 0; k < c.length(); k++) 
		{
			sum += c[k] * mult;
			mult *= 256;
		}

		return ((int)abs(sum) % INT_MAX);
	}
	else if(attrType=='i')
	{
		return abs((*(int*)value));
	}
	else
	{
		HTD_Errno=HTDE_UNKNOWNATTR;
		return -1;
	}
}

int HTD_info::i_update(void)
{
	HTD_Errno=HTDE_OK;
	void* data;
	
	//Write the first index
	if(BF_WriteBlock(fileDesc,0)<0)
	{
		if(BF_Errno!=BFE_BLOCKNOTINBUF)
		{
			HTD_Errno=BF_Errno;
			return HTD_Errno;
		}
	}
	
	//If there are no other index blocks, we're done.
	if(nextIndex<0)
	{
		return HTD_Errno;
	}
	
	//Write the second index
	if(BF_WriteBlock(fileDesc,nextIndex)<0)
	{
		if(BF_Errno!=BFE_BLOCKNOTINBUF)
		{
			HTD_Errno=BF_Errno;
			return HTD_Errno;
		}
	}
	//Read the second index
	if(BF_ReadBlock(fileDesc,nextIndex,&data)<0)
	{
		HTD_Errno=BF_Errno;
		return HTD_Errno;
	}
	++blocksread;
	//Write the rest of the indexes
	while(((HTD_Index*)data)->next>0)
	{
		//Write the next index
		if(BF_WriteBlock(fileDesc,((HTD_Index*)data)->next)<0)
		{
			if(BF_Errno!=BFE_BLOCKNOTINBUF)
			{
				HTD_Errno=BF_Errno;
				return HTD_Errno;
			}
		}
		//Read the next index
		if(BF_ReadBlock(fileDesc,((HTD_Index*)data)->next,&data)<0)
		{
			HTD_Errno=BF_Errno;
			return HTD_Errno;
		}
		++blocksread;
	}
	
	return HTD_Errno;
}

HTD_FirstBlock::HTD_FirstBlock(void)
{
	attrType='\0';
	attrName[0]='\0';
	attrLength=-1;
	depth=-1;
	nextIndex=-2;
	row=-1;
}

HTD_FirstBlock::HTD_FirstBlock(int AttrType,std::string AttrName,int AttrLength,int Depth,int NextIndex,int Row)
{
	attrType=AttrType;
	strncpy(attrName,AttrName.c_str(),sizeof(attrName));
	attrLength=AttrLength;
	depth=Depth;
	nextIndex=NextIndex;
	row=Row;
}

void HTD_FirstBlock::update(int AttrType,std::string AttrName,int AttrLength,int Depth,int NextIndex, int Row)
{
	attrType=AttrType;
	strncpy(attrName,AttrName.c_str(),sizeof(attrName));
	attrLength=AttrLength;
	depth=Depth;
	nextIndex=NextIndex;
	row=Row;
}

void HTD_FirstBlock::update(HTD_info* hinfo)
{
	attrType=hinfo->attrType;
	strncpy(attrName,hinfo->attrName,sizeof(attrName));
	attrLength=hinfo->attrLength;
	depth=hinfo->depth;
	nextIndex=hinfo->nextIndex;
	row=hinfo->row;
}

HTD_Bucket::HTD_Bucket(void)
{
	depth=-1;
	records=0;
}

HTD_Bucket::HTD_Bucket(int Depth)
{
	depth=Depth;
	records=0;
}

HTD_Index::HTD_Index(void)
{
	next=-1;
}

//Initialises everything that needs initialising for the hash variables to work.
void HTD_Init(void)
{
	BF_Init();
	HTD_Errno=BF_Errno;
}


//Creates a new hash file with name <fileName>, which will use a key 
//of type <attrType> and name <attrName> of size <attrLength>
//The hash table will have a depth of <depth>
//Returns 0 for success or a negative number for failure
int HTD_CreateIndex(const char* fileName, 
	char attrType, const char* attrName, int attrLength, int depth)
{
	HTD_Errno=HTDE_OK;
	int file;
	void* data;
	void* bucketdata;
	void* indexdata;
	int temp;
	int i;
	int sindex;
	int* tempindex;
	HTD_FirstBlock firstblock;
	HTD_Bucket bucket(0);
	HTD_Index indexblock;
	HTD_info info;
	
	//Open File
	if(BF_CreateFile(fileName)<0)
	{
		HTD_Errno=BF_Errno;
		return HTD_Errno;
	}
	if((file=BF_OpenFile(fileName))<0)
	{
		HTD_Errno=BF_Errno;
		return HTD_Errno;
	}
	
	//Allocate block
	if(BF_AllocateBlock(file)<0)
	{
		HTD_Errno=BF_Errno;
		BF_CloseFile(file);
		return HTD_Errno;
	}
	
	//Write data to block
	if(BF_ReadBlock(file,0,&data)<0)
	{
		HTD_Errno=BF_Errno;
		BF_CloseFile(file);
		return HTD_Errno;
	}
	++blocksread;
	*((char*)data)='t';
	
	//Create index
	sindex=pow(2,depth);
	if(sindex<(BLOCK_SIZE-sizeof(char)-sizeof(HTD_FirstBlock))/sizeof(int))
	{
		firstblock.update(attrType,attrName,attrLength,depth,-1,-1);
		memcpy((char*)data+sizeof(char),&firstblock,sizeof(HTD_FirstBlock));
		if(BF_WriteBlock(file,0)<0)
		{
			HTD_Errno=BF_Errno;
			BF_CloseFile(file);
			return HTD_Errno;
		}
	}
	else
	{
		sindex=sindex-((BLOCK_SIZE-sizeof(char)-sizeof(HTD_FirstBlock))/sizeof(int));
		if(BF_AllocateBlock(file)<0)
		{
			HTD_Errno=BF_Errno;
			BF_CloseFile(file);
			return HTD_Errno;
		}
		firstblock.update(attrType,attrName,attrLength,depth,(temp=BF_GetBlockCounter(file))-1,-1);
		if(temp<0)
		{
			HTD_Errno=BF_Errno;
			BF_CloseFile(file);
			return HTD_Errno;
		}
		memcpy((char*)data+sizeof(char),&firstblock,sizeof(HTD_FirstBlock));
		if(BF_WriteBlock(file,0)<0)
		{
			HTD_Errno=BF_Errno;
			BF_CloseFile(file);
			return HTD_Errno;
		}
		if(BF_ReadBlock(file,firstblock.nextIndex,&indexdata)<0)
		{
			HTD_Errno=BF_Errno;
			BF_CloseFile(file);
			return HTD_Errno;
		}
		++blocksread;
		memcpy(indexdata,&indexblock,sizeof(HTD_Index));
		while(sindex>=((BLOCK_SIZE-sizeof(HTD_Index))/sizeof(int)))
		{
			sindex=sindex-((BLOCK_SIZE-sizeof(HTD_Index))/sizeof(int));
			if(BF_AllocateBlock(file)<0)
			{
				HTD_Errno=BF_Errno;
				BF_CloseFile(file);
				return HTD_Errno;
			}
			if((temp=BF_GetBlockCounter(file))<0)
			{
				HTD_Errno=BF_Errno;
				BF_CloseFile(file);
				return HTD_Errno;
			}
			((HTD_Index*)indexdata)->next=temp-1;
			if(BF_ReadBlock(file,((HTD_Index*)indexdata)->next,&indexdata)<0)
			{
				HTD_Errno=BF_Errno;
				BF_CloseFile(file);
				return HTD_Errno;
			}
			++blocksread;
			memcpy(indexdata,&indexblock,sizeof(HTD_Index));
		}
	}
	
	//Create first bucket block
	if(BF_AllocateBlock(file)<0)
	{
		HTD_Errno=BF_Errno;
		BF_CloseFile(file);
		return HTD_Errno;
	}
	if((temp=BF_GetBlockCounter(file))<0)
	{
		HTD_Errno=BF_Errno;
		BF_CloseFile(file);
		return HTD_Errno;
	}
	--temp;
	if(BF_ReadBlock(file,temp,&bucketdata)<0)
	{
		HTD_Errno=BF_Errno;
		BF_CloseFile(file);
		return HTD_Errno;
	}
	++blocksread;
	memcpy(bucketdata,&bucket,sizeof(HTD_Bucket));
	if(BF_WriteBlock(file,temp)<0)
	{
		HTD_Errno=BF_Errno;
		BF_CloseFile(file);
		return HTD_Errno;
	}
	
	//Make all indexes point to the first bucket block
	info.update(file,attrType,attrName,attrLength,depth,firstblock.nextIndex,-1);
	for(i=0;(i<pow(2,depth));++i)
	{
		if((tempindex=info.i_hash(i))==NULL)
		{
			return HTD_Errno;
		}
		*tempindex=temp;
	}
	//Write index changes to disk
	if(info.i_update())
	{
		return HTD_Errno;
	}
	
	//Close file
	if(BF_CloseFile(file)<0)
	{
		HTD_Errno=BF_Errno;
		return HTD_Errno;
	}

	return HTD_Errno;
}


//Opens an already existing hash file with name <filename>
//Returns an HTD_info object that serves as the file descriptor 
//or NULL in case of error.
//The user is responsible for deleting the returned object, either
//manually or prefferably through the HTD_CloseIndex function.
HTD_info* HTD_OpenIndex(const char* fileName)
{
	HTD_Errno=HTDE_OK;
	HTD_info* htinfo=NULL;
	HTD_FirstBlock* firstblock;
	int file;
	void* data;
	
	if((file=BF_OpenFile(fileName))<0)
	{
		HTD_Errno=BF_Errno;
		return NULL;
	}
	if(BF_ReadBlock(file,0,&data)<0)
	{
		HTD_Errno=BF_Errno;
		BF_CloseFile(file);
		return NULL;
	}
	++blocksread;
	if(*(char*)data!='t')
	{
		HTD_Errno=HTDE_NOTAHASHFILE;
		BF_CloseFile(file);
		return NULL;
	}
	firstblock=(HTD_FirstBlock*)((char*)data+sizeof(char));
	htinfo=new HTD_info(file,firstblock->attrType,firstblock->attrName,firstblock->attrLength,firstblock->depth,firstblock->nextIndex,firstblock->row);
	
	return htinfo;
}


//Closes the file and, if the file closes successfuly,
//also deletes the HTD_info object
//Returns 0 for success or a negative number for failure
int HTD_CloseIndex(HTD_info* header_info)
{
	HTD_Errno=HTDE_OK;
	HTD_FirstBlock block;
	void* data;
	
	//Write header_info to file and exit
	block.update(header_info);
	if(BF_ReadBlock(header_info->fileDesc,0,&data)<0)
	{
		HTD_Errno=BF_Errno;
		return HTD_Errno;
	}
	++blocksread;
	memcpy((char*)data+sizeof(char),&block,sizeof(HTD_FirstBlock));
	if(BF_WriteBlock(header_info->fileDesc,0)<0)
	{
		HTD_Errno=BF_Errno;
		return HTD_Errno;
	}
	if(BF_CloseFile(header_info->fileDesc)<0)
	{
		HTD_Errno=BF_Errno;
		return HTD_Errno;
	}
	delete header_info;
	
	return HTD_Errno;
}


//Inserts the <record> into the hash file referred to by <header_info>
//Returns 0 for success or a negative number for failure
int HTD_InsertEntry(HTD_info* header_info, void* record)
{
	HTD_Errno=HTDE_OK;
	int hash;
	int* index;
	void* key;
	void* bucketdata;
	HTD_Bucket* bucket;
	void* l_bucketdata;
	HTD_Bucket* l_bucket;
	void* r_bucketdata;
	HTD_Bucket* r_bucket;
	HTD_Bucket tempbucket(0);
	int temp;
	int temphash;
	void* tempkey;
	int i;
	int newblock;
	int* tempindex;
	void* indexdata;
	queue<int> q_index;
	HTD_Index copyindex;
	
	//Get the key variable
	key=(char*)record+sizeof(int);
	//Hash the key variable
	if((hash=header_info->m_hash(key))<0)
	{
		return HTD_Errno;
	}
	//Get the index pointing to the bucket for <hash>
	if((index=header_info->r_hash(hash))==NULL)
	{
		return HTD_Errno;
	}
	//Get the bucket for hash
	if(BF_ReadBlock(header_info->fileDesc,*index,&bucketdata)<0)
	{
		HTD_Errno=BF_Errno;
		return HTD_Errno;
	}
	++blocksread;
	bucket=(HTD_Bucket*)bucketdata;
	
	//If the bucket has room for this record just put it in and we're done
	if((sizeof(int)+header_info->attrLength)<BLOCK_SIZE-(sizeof(HTD_Bucket)+(bucket->records)*(sizeof(int)+header_info->attrLength)))
	{
		memcpy((char*)bucketdata+sizeof(HTD_Bucket)+(bucket->records)*(sizeof(int)+header_info->attrLength),record,(sizeof(int)+header_info->attrLength));
		++(bucket->records);
		if(BF_WriteBlock(header_info->fileDesc,*index)<0)
		{
			HTD_Errno=BF_Errno;
			return HTD_Errno;
		}
		if(*(int*)record>header_info->row)
		{
			header_info->row=*(int*)record;
		}
		return HTD_Errno;
	}
	
	//If the bucket hasn't yet reached its maximum depth, split the bucket
	if(bucket->depth!=header_info->depth)
	{
		//Create two temporary buckets in the memory
		if((l_bucketdata=malloc(BLOCK_SIZE))==NULL)
		{
			HTD_Errno=HTDE_MALLOCERR;
			return HTD_Errno;
		}
		if((r_bucketdata=malloc(BLOCK_SIZE))==NULL)
		{
			HTD_Errno=HTDE_MALLOCERR;
			free(l_bucketdata);
			return HTD_Errno;
		}
		tempbucket.depth=bucket->depth+1;
		memcpy(l_bucketdata,&tempbucket,sizeof(HTD_Bucket));
		memcpy(r_bucketdata,&tempbucket,sizeof(HTD_Bucket));
		l_bucket=(HTD_Bucket*)l_bucketdata;
		r_bucket=(HTD_Bucket*)r_bucketdata;
		
		//For all records in the bucket being split
		for(i=0;i<bucket->records;++i)
		{
			//Get its hash
			tempkey=((char*)bucketdata+sizeof(HTD_Bucket)+i*(sizeof(int)+header_info->attrLength)+sizeof(int));
			if(tempkey==NULL)
			{
				HTD_Errno=HTDE_UNKNOWNATTR;
				free(l_bucketdata);
				free(r_bucketdata);
				return HTD_Errno;
			}
			if((temphash = header_info->m_hash(tempkey))<0)
			{
				free(l_bucketdata);
				free(r_bucketdata);
				return HTD_Errno;
			}
			//Get the digit that interests us
			temp = temphash & (1<<(tempbucket.depth-1));
			temp = temp >> (tempbucket.depth-1);
			//If the digit is 0 the element gets inserted into the left bucket
			if(temp==0)
			{
				memcpy((char*)l_bucketdata+sizeof(HTD_Bucket)+l_bucket->records*(sizeof(int)+header_info->attrLength),
					(char*)bucketdata+sizeof(HTD_Bucket)+i*(sizeof(int)+header_info->attrLength),(sizeof(int)+header_info->attrLength));
				l_bucket->records+=1;
			}
			else if(temp==1)
			{
				memcpy((char*)r_bucketdata+sizeof(HTD_Bucket)+r_bucket->records*(sizeof(int)+header_info->attrLength),
					(char*)bucketdata+sizeof(HTD_Bucket)+i*(sizeof(int)+header_info->attrLength),(sizeof(int)+header_info->attrLength));
				r_bucket->records+=1;
			}
			else
			{
				cerr<<"Debug: Check your code."<<endl;
				cin.get();
				free(l_bucketdata);
				free(r_bucketdata);
				return -1;
			}
		}
		
		//Where should the inserted value go?
		temp = hash & (1<<(tempbucket.depth-1));
		temp = temp >> (tempbucket.depth-1);
		//If left
		if(temp==0)
		{
			//The block pointed by index becomes the left bucket
			memcpy(bucketdata,l_bucketdata,BLOCK_SIZE);
			free(l_bucketdata);
			if(BF_WriteBlock(header_info->fileDesc,(*index))<0)
			{
				HTD_Errno=BF_Errno;
				free(r_bucketdata);
				return HTD_Errno;
			}
			//Alocate a new block for right bucket
			if(BF_AllocateBlock(header_info->fileDesc)<0)
			{
				HTD_Errno=BF_Errno;
				free(r_bucketdata);
				return HTD_Errno;
			}
			if((newblock=BF_GetBlockCounter(header_info->fileDesc))<0)
			{
				HTD_Errno=BF_Errno;
				free(r_bucketdata);
				return HTD_Errno;
			}
			--newblock;
			if(BF_ReadBlock(header_info->fileDesc,newblock,&bucketdata)<0)
			{
				HTD_Errno=BF_Errno;
				free(r_bucketdata);
				return HTD_Errno;
			}
			++blocksread;
			//Copy right bucket to new block
			memcpy(bucketdata,r_bucketdata,BLOCK_SIZE);
			free(r_bucketdata);
			if(BF_WriteBlock(header_info->fileDesc,newblock)<0)
			{
				HTD_Errno=BF_Errno;
				return HTD_Errno;
			}
			//i now holds the number of indexes we need to change
			i=pow(2,header_info->depth-(tempbucket.depth));
			//temp now holds the position of the begining of the indexes we need to change
			temp=(reverseBits(hash,header_info->depth) & ((1<<header_info->depth)-1))/i;
			temp=(temp+1)*i;
			//For all the indexes we need to change
			for(int j=temp;j<temp+i;++j)
			{
				//Make them point to the new block
				if((tempindex=header_info->i_hash(j))==NULL)
				{
					return HTD_Errno;
				}
				*tempindex=newblock;
			}
			//Write index changes to disk
			if(header_info->i_update()<0)
			{
				return HTD_Errno;
			}
		}
		//If right
		else if(temp==1)
		{
			//The block pointed by index becomes the right bucket
			memcpy(bucketdata,r_bucketdata,BLOCK_SIZE);
			free(r_bucketdata);
			if(BF_WriteBlock(header_info->fileDesc,(*index))<0)
			{
				HTD_Errno=BF_Errno;
				free(l_bucketdata);
				return HTD_Errno;
			}
			//Alocate a new block for left bucket
			if(BF_AllocateBlock(header_info->fileDesc)<0)
			{
				HTD_Errno=BF_Errno;
				free(l_bucketdata);
				return HTD_Errno;
			}
			if((newblock=BF_GetBlockCounter(header_info->fileDesc))<0)
			{
				HTD_Errno=BF_Errno;
				free(l_bucketdata);
				return HTD_Errno;
			}
			--newblock;
			if(BF_ReadBlock(header_info->fileDesc,newblock,&bucketdata)<0)
			{
				HTD_Errno=BF_Errno;
				free(l_bucketdata);
				return HTD_Errno;
			}
			++blocksread;
			//Copy left bucket to new block
			memcpy(bucketdata,l_bucketdata,BLOCK_SIZE);
			free(l_bucketdata);
			if(BF_WriteBlock(header_info->fileDesc,newblock)<0)
			{
				HTD_Errno=BF_Errno;
				return HTD_Errno;
			}
			//i now holds the number of indexes we need to change
			i=pow(2,header_info->depth-(tempbucket.depth));
			//temp now holds the position of the begining of the indexes we need to change
			temp=(reverseBits(hash,header_info->depth) & ((1<<header_info->depth)-1))/i;
			temp=(temp-1)*i;
			//For all the indexes we need to change
			for(int j=temp;j<temp+i;++j)
			{
				//Make them point to the new block
				if((tempindex=header_info->i_hash(j))==NULL)
				{
					return HTD_Errno;
				}
				*tempindex=newblock;
			}
			//Write index changes to disk
			if(header_info->i_update()<0)
			{
				return HTD_Errno;
			}
		}
		else
		{
			cerr<<"Debug: Check your code."<<endl;
			cin.get();
			free(l_bucketdata);
			free(r_bucketdata);
			return -1;
		}
		//The function calls itself and the record is (hopefully) inserted to the correct block.
		HTD_InsertEntry(header_info,record);
		return HTD_Errno;
	}
	//If the bucket has reached its maximum depth
	else
	{
		//Copy the index for later
		temp=pow(2,header_info->depth);
		for(i=0;i<temp;++i)
		{
			if((tempindex=header_info->i_hash(i))==NULL)
			{
				return HTD_Errno;
			}
			q_index.push(*tempindex);
		}
		//Increment the maximum depth
		++(header_info->depth);
		temp=pow(2,header_info->depth);
		//First allocate all the necessary blocks if necessary
		if(!(temp<(BLOCK_SIZE-sizeof(char)-sizeof(HTD_FirstBlock))/sizeof(int)))
		{
			//Remove all the indexes we've already allocated
			temp=temp-((BLOCK_SIZE-sizeof(char)-sizeof(HTD_FirstBlock))/sizeof(int));
			//If there isn't a next block allocated, allocate one
			if(BF_ReadBlock(header_info->fileDesc,0,&indexdata)<0)
			{
				HTD_Errno=BF_Errno;
				return HTD_Errno;
			}
			++blocksread;
			if(header_info->nextIndex<=0)
			{
				if(BF_AllocateBlock(header_info->fileDesc)<0)
				{
					HTD_Errno=BF_Errno;
					return HTD_Errno;
				}
				if((i=BF_GetBlockCounter(header_info->fileDesc))<0)
				{
					HTD_Errno=BF_Errno;
					return HTD_Errno;
				}
				--i;
				header_info->nextIndex=i;
				((HTD_FirstBlock*)((char*)indexdata+sizeof(char)))->nextIndex=i;
				//Update first index block
				if(BF_WriteBlock(header_info->fileDesc,0)<0)
				{
					HTD_Errno=BF_Errno;
					return HTD_Errno;
				}
				//Read the newly allocated index block
				if(BF_ReadBlock(header_info->fileDesc,header_info->nextIndex,&indexdata)<0)
				{
					HTD_Errno=BF_Errno;
					return HTD_Errno;
				}
				++blocksread;
				memcpy(indexdata,&copyindex,sizeof(HTD_Index));
				
			}
			//Read the next index block
			if(BF_ReadBlock(header_info->fileDesc,header_info->nextIndex,&indexdata)<0)
			{
				HTD_Errno=BF_Errno;
				return HTD_Errno;
			}
			++blocksread;
			//Keep allocating blocks
			while(temp>=((BLOCK_SIZE-sizeof(HTD_Index))/sizeof(int)))
			{
				//Remove all the indexes we've already allocated
				temp=temp-((BLOCK_SIZE-sizeof(HTD_Index))/sizeof(int));
				if(((HTD_Index*)(indexdata))->next<=0)
				{
					if(BF_AllocateBlock(header_info->fileDesc)<0)
					{
						HTD_Errno=BF_Errno;
						return HTD_Errno;
					}
					if((i=BF_GetBlockCounter(header_info->fileDesc))<0)
					{
						HTD_Errno=BF_Errno;
						return HTD_Errno;
					}
					--i;
					((HTD_Index*)(indexdata))->next=i;
				}
				if(BF_ReadBlock(header_info->fileDesc,((HTD_Index*)(indexdata))->next,&indexdata)<0)
				{
					HTD_Errno=BF_Errno;
					return HTD_Errno;
				}
				++blocksread;
			}
		}
		//Extend index by duplicating all index entries
		temp=pow(2,header_info->depth);
		for(i=0;i<temp;++i)
		{
			if((tempindex=header_info->i_hash(i))==NULL)
			{
				return HTD_Errno;
			}
			*tempindex=q_index.front();
			if(i%2==1)
			{
				q_index.pop();
			}
		}
		//Update Index
		if(header_info->i_update()<0)
		{
			return HTD_Errno;
		}
		//Call self to continue with splitting the guilty block and inserting the record
		HTD_InsertEntry(header_info,record);
		return HTD_Errno;
	}
	
	return HTD_Errno;
}

//Returns the row of the first record that matches <value> and has row-id greater then or equal to <row>
//Returns negative for failure (failure includes HTDE_NOTFOUND which means that the requested data
//was not found)
int HTD_GetRow(HTD_info* header_info, const void* value, int row)
{
	HTD_Errno=HTDE_OK;
	int* index;
	void* data;
	HTD_Bucket* bucket;
	void* record;
	int hash;
	void* var;
	
	
	if((hash=header_info->m_hash(value))<0)
	{
		return HTD_Errno;
	}
	if((index=header_info->r_hash(hash))==NULL)
	{
		return HTD_Errno;
	}
	if(BF_ReadBlock(header_info->fileDesc,*index,&data)<0)
	{
		HTD_Errno=BF_Errno;
		return HTD_Errno;
	}
	++blocksread;
	bucket=(HTD_Bucket*)data;
	record=((char*)data+sizeof(HTD_Bucket));
	for(int j=0;j<bucket->records;++j)
	{
		var=(char*)record+j*(sizeof(int)+header_info->attrLength)+sizeof(int);
		if(header_info->attrType=='i')
		{
			if((*(int*)var)==(*(int*)value)&&(*(int*)((char*)var-sizeof(int)))>=row)
			{
				return *(int*)((char*)var-sizeof(int));
			}
		}
		else if(header_info->attrType=='c')
		{
			if(!strncmp((char*)var,(char*)value,header_info->attrLength)&&(*(int*)((char*)var-sizeof(int)))>=row)
			{
				return *(int*)((char*)var-sizeof(int));
			}
		}
		else
		{
			HTD_Errno=HTDE_UNKNOWNATTR;
			return HTD_Errno;
		}
	}
	
	return HTDE_NOTFOUND;
}


//Prints <message> and then the error.
void HTD_PrintError(const char * message)
{
	switch(HTD_Errno)
	{
		case HTDE_OK:
			cerr<<"HTDE ERROR: "<<message<<": No error"<<endl;
		break;
		case HTDE_NOTAHASHFILE:
			cerr<<"HTDE ERROR: "<<message<<": File is not a hash file"<<endl;
		break;
		case HTDE_UNKNOWNATTR:
			cerr<<"HTDE ERROR: "<<message<<": Unknown attrybute type"<<endl;
		break;
		case HTDE_MALLOCERR:
			cerr<<"HTDE ERROR: "<<message<<": Memory allocation error"<<endl;
		break;
		case HTDE_NOTFOUND:
			cerr<<"HTDE ERROR: "<<message<<": Requested data not found"<<endl;
		break;
		default:
			BF_Errno=HTD_Errno;
			BF_PrintError(message);
		break;
	}
}
