#include "DB1_Hash.hpp"

#include <cstring>
#include <iostream>
#include <string>
#include <cmath>
#include <climits>
#include <cstdlib>
#include <queue>

int HT_Errno=HTE_OK;

//Counts the number of blocks read for the printing functions.
int rblocks=0;

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

HT_info::HT_info(void)
{
	fileDesc=-1;
	attrType='\0';
	attrName[0]='\0';
	attrLength=-1;
	depth=-1;
	nextIndex=-1;
}

HT_info::HT_info(int FileDesc, char AttrType, 
	const char* AttrName, int AttrLength, int Depth, int NextIndex)
{
	fileDesc=FileDesc;
	attrType=AttrType;
	strncpy(attrName,AttrName,sizeof(attrName));
	attrLength=AttrLength;
	depth=Depth;
	nextIndex=NextIndex;
}

void HT_info::update(int FileDesc, char AttrType, 
	const char* AttrName, int AttrLength, int Depth, int NextIndex)
{
	fileDesc=FileDesc;
	attrType=AttrType;
	strncpy(attrName,AttrName,sizeof(attrName));
	attrLength=AttrLength;
	depth=Depth;
	nextIndex=NextIndex;
}

//Takes a hash value and returns the appropriate index. Returns Null for error.
int* HT_info::r_hash(int hash)
{
	int temp;
	
	temp = hash & ((1<<depth)-1);
	temp = reverseBits(temp,depth);
	
	return i_hash(temp);
}
//Accesses the index as an array. Returns Null for error.
int* HT_info::i_hash(int pos)
{
	HT_Errno=HTE_OK;
	int temp = pos & ((1<<depth)-1);
	void* data;
	int* t_index;
	HT_Index* index;
	
	//Check if the requested index is in the first block
	if(temp<(BLOCK_SIZE-sizeof(char)-sizeof(HT_FirstBlock))/sizeof(int))
	{
		++rblocks;
		if(BF_ReadBlock(fileDesc,0,&data)<0)
		{
			HT_Errno=BF_Errno;
			return NULL;
		}
		t_index=(int*)((char*)data+sizeof(char)+sizeof(HT_FirstBlock));
		return &(t_index[temp]);
	}
	
	//If it isn't, remove all the indexes we have already searched from temp
	temp=temp-((BLOCK_SIZE-sizeof(char)-sizeof(HT_FirstBlock))/sizeof(int));
	++rblocks;
	if(BF_ReadBlock(fileDesc,nextIndex,&data)<0)
	{
		HT_Errno=BF_Errno;
		return NULL;
	}
	index=(HT_Index*)data;
	//Then start searching the next indexes
	while (temp>=(BLOCK_SIZE-sizeof(HT_Index))/sizeof(int))
	{
		temp=temp-((BLOCK_SIZE-sizeof(HT_Index))/sizeof(int));
		++rblocks;
		if(BF_ReadBlock(fileDesc,index->next,&data)<0)
		{
			HT_Errno=BF_Errno;
			return NULL;
		}
		index=(HT_Index*)data;
	}
	t_index=(int*)((char*)data+sizeof(HT_Index));
	return &(t_index[temp]);
}

int HT_info::m_hash(const void* value)
{
	HT_Errno=HTE_OK;
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
		HT_Errno=HTE_UNKNOWNATTR;
		return -1;
	}
}

int HT_info::i_update(void)
{
	HT_Errno=HTE_OK;
	void* data;
	
	//Write the first index
	if(BF_WriteBlock(fileDesc,0)<0)
	{
		HT_Errno=BF_Errno;
		return HT_Errno;
	}
	
	//If there are no other index blocks, we're done.
	if(nextIndex<0)
	{
		return HT_Errno;
	}
	
	//Write the second index
	if(BF_WriteBlock(fileDesc,nextIndex)<0)
	{
		HT_Errno=BF_Errno;
		return HT_Errno;
	}
	//Read the second index
	++rblocks;
	if(BF_ReadBlock(fileDesc,nextIndex,&data)<0)
	{
		HT_Errno=BF_Errno;
		return HT_Errno;
	}
	
	//Write the rest of the indexes
	while(((HT_Index*)data)->next>0)
	{
		//Write the next index
		if(BF_WriteBlock(fileDesc,((HT_Index*)data)->next)<0)
		{
			HT_Errno=BF_Errno;
			return HT_Errno;
		}
		//Read the next index
		++rblocks;
		if(BF_ReadBlock(fileDesc,((HT_Index*)data)->next,&data)<0)
		{
			HT_Errno=BF_Errno;
			return HT_Errno;
		}
	}
	
	return HT_Errno;
}

HT_FirstBlock::HT_FirstBlock(void)
{
	attrType='\0';
	attrName[0]='\0';
	attrLength=-1;
	depth=-1;
	nextIndex=-2;
}

HT_FirstBlock::HT_FirstBlock(int AttrType,std::string AttrName,int AttrLength,int Depth,int NextIndex)
{
	attrType=AttrType;
	strncpy(attrName,AttrName.c_str(),sizeof(attrName));
	attrLength=AttrLength;
	depth=Depth;
	nextIndex=NextIndex;
}

void HT_FirstBlock::update(int AttrType,std::string AttrName,int AttrLength,int Depth,int NextIndex)
{
	attrType=AttrType;
	strncpy(attrName,AttrName.c_str(),sizeof(attrName));
	attrLength=AttrLength;
	depth=Depth;
	nextIndex=NextIndex;
}

void HT_FirstBlock::update(HT_info* hinfo)
{
	attrType=hinfo->attrType;
	strncpy(attrName,hinfo->attrName,sizeof(attrName));
	attrLength=hinfo->attrLength;
	depth=hinfo->depth;
	nextIndex=hinfo->nextIndex;
}

HT_Bucket::HT_Bucket(void)
{
	depth=-1;
	records=0;
}

HT_Bucket::HT_Bucket(int Depth)
{
	depth=Depth;
	records=0;
}

HT_Index::HT_Index(void)
{
	next=-1;
}

//Initialises everything that needs initialising for the hash variables to work.
void HT_Init(void)
{
	BF_Init();
	HT_Errno=BF_Errno;
}


//Creates a new hash file with name <fileName>, which will use a key 
//of type <attrType> and name <attrName> of size <attrLength>
//The hash table will have a depth of <depth>
//Returns 0 for success or a negative number for failure
int HT_CreateIndex(char* fileName, 
	char attrType, char* attrName, int attrLength, int depth)
{
	HT_Errno=HTE_OK;
	int file;
	void* data;
	void* bucketdata;
	void* indexdata;
	int temp;
	int i;
	int sindex;
	int* tempindex;
	HT_FirstBlock firstblock;
	HT_Bucket bucket(0);
	HT_Index indexblock;
	HT_info info;
	
	//Open File
	if(BF_CreateFile(fileName)<0)
	{
		HT_Errno=BF_Errno;
		return HT_Errno;
	}
	if((file=BF_OpenFile(fileName))<0)
	{
		HT_Errno=BF_Errno;
		return HT_Errno;
	}
	
	//Allocate block
	if(BF_AllocateBlock(file)<0)
	{
		HT_Errno=BF_Errno;
		BF_CloseFile(file);
		return HT_Errno;
	}
	
	//Write data to block
	if(BF_ReadBlock(file,0,&data)<0)
	{
		HT_Errno=BF_Errno;
		BF_CloseFile(file);
		return HT_Errno;
	}
	*((char*)data)='t';
	
	//Create index
	sindex=pow(2,depth);
	if(sindex<(BLOCK_SIZE-sizeof(char)-sizeof(HT_FirstBlock))/sizeof(int))
	{
		firstblock.update(attrType,attrName,attrLength,depth,-1);
		memcpy((char*)data+sizeof(char),&firstblock,sizeof(HT_FirstBlock));
		if(BF_WriteBlock(file,0)<0)
		{
			HT_Errno=BF_Errno;
			BF_CloseFile(file);
			return HT_Errno;
		}
	}
	else
	{
		sindex=sindex-((BLOCK_SIZE-sizeof(char)-sizeof(HT_FirstBlock))/sizeof(int));
		if(BF_AllocateBlock(file)<0)
		{
			HT_Errno=BF_Errno;
			BF_CloseFile(file);
			return HT_Errno;
		}
		firstblock.update(attrType,attrName,attrLength,depth,(temp=BF_GetBlockCounter(file))-1);
		if(temp<0)
		{
			HT_Errno=BF_Errno;
			BF_CloseFile(file);
			return HT_Errno;
		}
		memcpy((char*)data+sizeof(char),&firstblock,sizeof(HT_FirstBlock));
		if(BF_WriteBlock(file,0)<0)
		{
			HT_Errno=BF_Errno;
			BF_CloseFile(file);
			return HT_Errno;
		}
		if(BF_ReadBlock(file,firstblock.nextIndex,&indexdata)<0)
		{
			HT_Errno=BF_Errno;
			BF_CloseFile(file);
			return HT_Errno;
		}
		memcpy(indexdata,&indexblock,sizeof(HT_Index));
		while(sindex>=((BLOCK_SIZE-sizeof(HT_Index))/sizeof(int)))
		{
			sindex=sindex-((BLOCK_SIZE-sizeof(HT_Index))/sizeof(int));
			if(BF_AllocateBlock(file)<0)
			{
				HT_Errno=BF_Errno;
				BF_CloseFile(file);
				return HT_Errno;
			}
			if((temp=BF_GetBlockCounter(file))<0)
			{
				HT_Errno=BF_Errno;
				BF_CloseFile(file);
				return HT_Errno;
			}
			((HT_Index*)indexdata)->next=temp-1;
			if(BF_ReadBlock(file,((HT_Index*)indexdata)->next,&indexdata)<0)
			{
				HT_Errno=BF_Errno;
				BF_CloseFile(file);
				return HT_Errno;
			}
			memcpy(indexdata,&indexblock,sizeof(HT_Index));
		}
	}
	
	//Create first bucket block
	if(BF_AllocateBlock(file)<0)
	{
		HT_Errno=BF_Errno;
		BF_CloseFile(file);
		return HT_Errno;
	}
	if((temp=BF_GetBlockCounter(file))<0)
	{
		HT_Errno=BF_Errno;
		BF_CloseFile(file);
		return HT_Errno;
	}
	--temp;
	if(BF_ReadBlock(file,temp,&bucketdata)<0)
	{
		HT_Errno=BF_Errno;
		BF_CloseFile(file);
		return HT_Errno;
	}
	memcpy(bucketdata,&bucket,sizeof(HT_Bucket));
	if(BF_WriteBlock(file,temp)<0)
	{
		HT_Errno=BF_Errno;
		BF_CloseFile(file);
		return HT_Errno;
	}
	
	//Make all indexes point to the first bucket block
	info.update(file,attrType,attrName,attrLength,depth,firstblock.nextIndex);
	for(i=0;(i<pow(2,depth));++i)
	{
		if((tempindex=info.i_hash(i))==NULL)
		{
			return HT_Errno;
		}
		*tempindex=temp;
	}
	//Write index changes to disk
	if(info.i_update())
	{
		return HT_Errno;
	}
	
	//Close file
	if(BF_CloseFile(file)<0)
	{
		HT_Errno=BF_Errno;
		return HT_Errno;
	}

	return HT_Errno;
}


//Opens an already existing hash file with name <filename>
//Returns an HT_info object that serves as the file descriptor 
//or NULL in case of error.
//The user is responsible for deleting the returned object, either
//manually or prefferably through the HT_CloseIndex function.
HT_info* HT_OpenIndex(char* fileName)
{
	HT_Errno=HTE_OK;
	HT_info* htinfo=NULL;
	HT_FirstBlock* firstblock;
	int file;
	void* data;
	
	if((file=BF_OpenFile(fileName))<0)
	{
		HT_Errno=BF_Errno;
		return NULL;
	}
	if(BF_ReadBlock(file,0,&data)<0)
	{
		HT_Errno=BF_Errno;
		BF_CloseFile(file);
		return NULL;
	}
	if(*(char*)data!='t')
	{
		HT_Errno=HTE_NOTAHASHFILE;
		BF_CloseFile(file);
		return NULL;
	}
	firstblock=(HT_FirstBlock*)((char*)data+sizeof(char));
	htinfo=new HT_info(file,firstblock->attrType,firstblock->attrName,firstblock->attrLength,firstblock->depth,firstblock->nextIndex);
	
	return htinfo;
}


//Closes the file and, if the file closes successfuly,
//also deletes the HT_info object
//Returns 0 for success or a negative number for failure
int HT_CloseIndex(HT_info* header_info)
{
	HT_Errno=HTE_OK;
	HT_FirstBlock block;
	void* data;
	
	//Write header_info to file and exit
	block.update(header_info);
	BF_ReadBlock(header_info->fileDesc,0,&data);
	memcpy((char*)data+sizeof(char),&block,sizeof(HT_FirstBlock));
	BF_WriteBlock(header_info->fileDesc,0);
	BF_CloseFile(header_info->fileDesc);
	delete header_info;
	
	return HT_Errno;
}


//Inserts the <record> into the hash file referred to by <header_info>
//Returns 0 for success or a negative number for failure
int HT_InsertEntry(HT_info* header_info, Record record)
{
	HT_Errno=HTE_OK;
	int hash;
	int* index;
	void* key;
	void* bucketdata;
	HT_Bucket* bucket;
	void* l_bucketdata;
	HT_Bucket* l_bucket;
	void* r_bucketdata;
	HT_Bucket* r_bucket;
	HT_Bucket tempbucket(0);
	int temp;
	int temphash;
	void* tempkey;
	int i;
	int newblock;
	int* tempindex;
	void* indexdata;
	queue<int> q_index;
	HT_Index copyindex;
	
	//Get the key variable
	record.getVariable(&key,header_info->attrName);
	if(key==NULL)
	{
		HT_Errno=HTE_UNKNOWNATTR;
		return HT_Errno;
	}
	//Hash the key variable
	if((hash=header_info->m_hash(key))<0)
	{
		return HT_Errno;
	}
	//Get the index pointing to the bucket for <hash>
	if((index=header_info->r_hash(hash))==NULL)
	{
		return HT_Errno;
	}
	//Get the bucket for hash
	if(BF_ReadBlock(header_info->fileDesc,*index,&bucketdata)<0)
	{
		HT_Errno=BF_Errno;
		return HT_Errno;
	}
	bucket=(HT_Bucket*)bucketdata;
	
	//If the bucket has room for this record just put it in and we're done
	if(sizeof(Record)<BLOCK_SIZE-(sizeof(HT_Bucket)+(bucket->records)*sizeof(Record)))
	{
		memcpy((char*)bucketdata+sizeof(HT_Bucket)+(bucket->records)*sizeof(Record),&record,sizeof(Record));
		++(bucket->records);
		if(BF_WriteBlock(header_info->fileDesc,*index)<0)
		{
			HT_Errno=BF_Errno;
			return HT_Errno;
		}
		
		return HT_Errno;
	}
	
	//If the bucket hasn't yet reached its maximum depth, split the bucket
	if(bucket->depth!=header_info->depth)
	{
		//Create two temporary buckets in the memory
		if((l_bucketdata=malloc(BLOCK_SIZE))==NULL)
		{
			HT_Errno=HTE_MALLOCERR;
			return HT_Errno;
		}
		if((r_bucketdata=malloc(BLOCK_SIZE))==NULL)
		{
			HT_Errno=HTE_MALLOCERR;
			free(l_bucketdata);
			return HT_Errno;
		}
		tempbucket.depth=bucket->depth+1;
		memcpy(l_bucketdata,&tempbucket,sizeof(HT_Bucket));
		memcpy(r_bucketdata,&tempbucket,sizeof(HT_Bucket));
		l_bucket=(HT_Bucket*)l_bucketdata;
		r_bucket=(HT_Bucket*)r_bucketdata;
		
		//For all records in the bucket being split
		for(i=0;i<bucket->records;++i)
		{
			//Get its hash
			((Record*)((char*)bucketdata+sizeof(HT_Bucket)+i*sizeof(Record)))->getVariable(&tempkey,header_info->attrName);
			if(tempkey==NULL)
			{
				HT_Errno=HTE_UNKNOWNATTR;
				free(l_bucketdata);
				free(r_bucketdata);
				return HT_Errno;
			}
			if((temphash = header_info->m_hash(tempkey))<0)
			{
				free(l_bucketdata);
				free(r_bucketdata);
				return HT_Errno;
			}
			//Get the digit that interests us
			temp = temphash & (1<<(tempbucket.depth-1));
			temp = temp >> (tempbucket.depth-1);
			//If the digit is 0 the element gets inserted into the left bucket
			if(temp==0)
			{
				memcpy((char*)l_bucketdata+sizeof(HT_Bucket)+l_bucket->records*sizeof(Record),
					(char*)bucketdata+sizeof(HT_Bucket)+i*sizeof(Record),sizeof(Record));
				l_bucket->records+=1;
			}
			else if(temp==1)
			{
				memcpy((char*)r_bucketdata+sizeof(HT_Bucket)+r_bucket->records*sizeof(Record),
					(char*)bucketdata+sizeof(HT_Bucket)+i*sizeof(Record),sizeof(Record));
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
				HT_Errno=BF_Errno;
				free(r_bucketdata);
				return HT_Errno;
			}
			//Alocate a new block for right bucket
			if(BF_AllocateBlock(header_info->fileDesc)<0)
			{
				HT_Errno=BF_Errno;
				free(r_bucketdata);
				return HT_Errno;
			}
			if((newblock=BF_GetBlockCounter(header_info->fileDesc))<0)
			{
				HT_Errno=BF_Errno;
				free(r_bucketdata);
				return HT_Errno;
			}
			--newblock;
			if(BF_ReadBlock(header_info->fileDesc,newblock,&bucketdata)<0)
			{
				HT_Errno=BF_Errno;
				free(r_bucketdata);
				return HT_Errno;
			}
			//Copy right bucket to new block
			memcpy(bucketdata,r_bucketdata,BLOCK_SIZE);
			free(r_bucketdata);
			if(BF_WriteBlock(header_info->fileDesc,newblock)<0)
			{
				HT_Errno=BF_Errno;
				return HT_Errno;
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
					return HT_Errno;
				}
				*tempindex=newblock;
			}
			//Write index changes to disk
			if(header_info->i_update()<0)
			{
				return HT_Errno;
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
				HT_Errno=BF_Errno;
				free(l_bucketdata);
				return HT_Errno;
			}
			//Alocate a new block for left bucket
			if(BF_AllocateBlock(header_info->fileDesc)<0)
			{
				HT_Errno=BF_Errno;
				free(l_bucketdata);
				return HT_Errno;
			}
			if((newblock=BF_GetBlockCounter(header_info->fileDesc))<0)
			{
				HT_Errno=BF_Errno;
				free(l_bucketdata);
				return HT_Errno;
			}
			--newblock;
			if(BF_ReadBlock(header_info->fileDesc,newblock,&bucketdata)<0)
			{
				HT_Errno=BF_Errno;
				free(l_bucketdata);
				return HT_Errno;
			}
			//Copy left bucket to new block
			memcpy(bucketdata,l_bucketdata,BLOCK_SIZE);
			free(l_bucketdata);
			if(BF_WriteBlock(header_info->fileDesc,newblock)<0)
			{
				HT_Errno=BF_Errno;
				return HT_Errno;
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
					return HT_Errno;
				}
				*tempindex=newblock;
			}
			//Write index changes to disk
			if(header_info->i_update()<0)
			{
				return HT_Errno;
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
		HT_InsertEntry(header_info,record);
		return HT_Errno;
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
				return HT_Errno;
			}
			q_index.push(*tempindex);
		}
		//Increment the maximum depth
		++(header_info->depth);
		temp=pow(2,header_info->depth);
		//First allocate all the necessary blocks if necessary
		if(!(temp<(BLOCK_SIZE-sizeof(char)-sizeof(HT_FirstBlock))/sizeof(int)))
		{
			//Remove all the indexes we've already allocated
			temp=temp-((BLOCK_SIZE-sizeof(char)-sizeof(HT_FirstBlock))/sizeof(int));
			//If there isn't a next block allocated, allocate one
			if(BF_ReadBlock(header_info->fileDesc,0,&indexdata)<0)
			{
				HT_Errno=BF_Errno;
				return HT_Errno;
			}
			if(header_info->nextIndex<=0)
			{
				if(BF_AllocateBlock(header_info->fileDesc)<0)
				{
					HT_Errno=BF_Errno;
					return HT_Errno;
				}
				if((i=BF_GetBlockCounter(header_info->fileDesc))<0)
				{
					HT_Errno=BF_Errno;
					return HT_Errno;
				}
				--i;
				header_info->nextIndex=i;
				((HT_FirstBlock*)((char*)indexdata+sizeof(char)))->nextIndex=i;
				//Update first index block
				if(BF_WriteBlock(header_info->fileDesc,0)<0)
				{
					HT_Errno=BF_Errno;
					return HT_Errno;
				}
				//Read the newly allocated index block
				if(BF_ReadBlock(header_info->fileDesc,header_info->nextIndex,&indexdata)<0)
				{
					HT_Errno=BF_Errno;
					return HT_Errno;
				}
				memcpy(indexdata,&copyindex,sizeof(HT_Index));
				
			}
			//Read the next index block
			if(BF_ReadBlock(header_info->fileDesc,header_info->nextIndex,&indexdata)<0)
			{
				HT_Errno=BF_Errno;
				return HT_Errno;
			}
			//Keep allocating blocks
			while(temp>=((BLOCK_SIZE-sizeof(HT_Index))/sizeof(int)))
			{
				//Remove all the indexes we've already allocated
				temp=temp-((BLOCK_SIZE-sizeof(HT_Index))/sizeof(int));
				if(((HT_Index*)(indexdata))->next<=0)
				{
					if(BF_AllocateBlock(header_info->fileDesc)<0)
					{
						HT_Errno=BF_Errno;
						return HT_Errno;
					}
					if((i=BF_GetBlockCounter(header_info->fileDesc))<0)
					{
						HT_Errno=BF_Errno;
						return HT_Errno;
					}
					--i;
					((HT_Index*)(indexdata))->next=i;
				}
				if(BF_ReadBlock(header_info->fileDesc,((HT_Index*)(indexdata))->next,&indexdata)<0)
				{
					HT_Errno=BF_Errno;
					return HT_Errno;
				}
			}
		}
		//Extend index by duplicating all index entries
		temp=pow(2,header_info->depth);
		for(i=0;i<temp;++i)
		{
			if((tempindex=header_info->i_hash(i))==NULL)
			{
				return HT_Errno;
			}
			*tempindex=q_index.front();
			if(i%2==1)
			{
				q_index.pop();
			}
		}
		//Update Index
		header_info->i_update();
		//Call self to continue with splitting the guilty block and inserting the record
		HT_InsertEntry(header_info,record);
		return HT_Errno;
	}
	
	return HT_Errno;
}

//Prints the entries with a key value of <value> (if such an entry exists) 
//from the heap file reffered to by <header_info>
//Also prints the number of blocks read
//The type of <value> must match the type of the key
void HT_GetAllEntries(HT_info* header_info, const void* value)
{
	HT_Errno=HTE_OK;
	rblocks=0;
	int temp;
	int prev;
	int* index;
	void* data;
	HT_Bucket* bucket;
	Record* record;
	int hash;
	void* var;
	
	if(value==NULL)
	{
		//Number of indexes
		temp=pow(2,header_info->depth);
		prev=-1;
		//For all indexes
		for(int i=0;i<temp;++i)
		{
			if((index=header_info->i_hash(i))==NULL)
			{
				return;
			}
			//If this bucket has already been read
			if(*index==prev)
			{
				//Skip it
				continue;
			}
			prev=*index;
			++rblocks;
			if(BF_ReadBlock(header_info->fileDesc,*index,&data)<0)
			{
				HT_Errno=BF_Errno;
				return;
			}
			bucket=(HT_Bucket*)data;
			record=(Record*)((char*)data+sizeof(HT_Bucket));
			//Print all records
			for(int j=0;j<bucket->records;++j)
			{
				record[j].print();
			}
		}
		cout<<"Blocks read: "<<rblocks<<endl;
		return;
	}
	
	
	if((hash=header_info->m_hash(value))<0)
	{
		return;
	}
	if((index=header_info->r_hash(hash))==NULL)
	{
		return;
	}
	++rblocks;
	if(BF_ReadBlock(header_info->fileDesc,*index,&data)<0)
	{
		HT_Errno=BF_Errno;
		return;
	}
	bucket=(HT_Bucket*)data;
	record=(Record*)((char*)data+sizeof(HT_Bucket));
	for(int j=0;j<bucket->records;++j)
	{
		record[j].getVariable(&var,header_info->attrName);
		if(header_info->attrType=='i')
		{
			if((*(int*)var)==(*(int*)value))
			{
				record[j].print();
			}
		}
		else if(header_info->attrType=='c')
		{
			if(!strncmp((char*)var,(char*)value,header_info->attrLength))
			{
				record[j].print();
			}
		}
		else
		{
			HT_Errno=HTE_UNKNOWNATTR;
			return;
		}
	}
	cout<<"Blocks read: "<<rblocks<<endl;
}


//Prints <message> and then the error.
void HT_PrintError(const char * message)
{
	switch(HT_Errno)
	{
		case HTE_OK:
			cerr<<"HTE ERROR: "<<message<<": No error"<<endl;
		break;
		case HTE_NOTAHASHFILE:
			cerr<<"HTE ERROR: "<<message<<": File is not a hash file"<<endl;
		break;
		case HTE_UNKNOWNATTR:
			cerr<<"HTE ERROR: "<<message<<": Unknown attrybute type"<<endl;
		break;
		case HTE_MALLOCERR:
			cerr<<"HTE ERROR: "<<message<<": Memory allocation error"<<endl;
		break;
		default:
			BF_Errno=HT_Errno;
			BF_PrintError(message);
		break;
	}
}
