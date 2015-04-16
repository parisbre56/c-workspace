#include "myheap.h"
#include <stdlib.h>

struct Heap {
	bool Maxheap;
	Heapnode** data;
	long heapsize;
	size_t datasize;
};

Heap* HEAP_newHeap(bool Maxheap, size_t datasize){ 
	Heap* tempheap = malloc(sizeof(Heap));
	if(tempheap==NULL)
	{
		return NULL;
	}
	tempheap->Maxheap=Maxheap;
	tempheap->datasize=datasize;
	tempheap->heapsize=0;
	tempheap->data=NULL;
	return tempheap;
}

bool greaterThan(double a, double b)
{
	return a>b;
}

bool lessThan(double a, double b)
{
	return a<b;
}

void swap(Heapnode** a, Heapnode** b)
{
		Heapnode* temp=*a;
		*a=*b;
		*b=temp;
}

int HEAP_insertData (Heap* heap, void* data, double value){ 
	//Used to change between maxheap and minheap.
	//Maxheap uses greaterThan
	//Minheap uses lessThan
	bool (*comparison)(double,double);
	if(heap->Maxheap) {
		comparison=&greaterThan;
	}
	else {
		comparison=&lessThan;
	}
	//If first data node entered, create array.
	if(heap->data==NULL)
	{
		heap->data=malloc(sizeof(Heapnode*));
	}
	//Else enlarge array by one.
	else
	{
		heap->data=realloc(heap->data,(heap->heapsize+1)*sizeof(Heapnode*));
	}
	//If memory allocation failed, fail
	if(heap->data==NULL)
	{
		return -1;
	}
	//Heapsize increased
	heap->heapsize=heap->heapsize+1;
	//Allocate space for heapnode
	(heap->data)[heap->heapsize-1]=malloc(sizeof(Heapnode));
	if((heap->data)[heap->heapsize-1]==NULL)
	{
		return -1;
	}
	//Allocate space for data
	(heap->data)[heap->heapsize-1]->data=malloc(heap->datasize);
	//If memory allocation failed, fail
	if((heap->data)[heap->heapsize-1]->data==NULL)
	{
		return -1;
	}
	//Copy data to node
	memcpy((heap->data)[heap->heapsize-1]->data,data,heap->datasize);
	(heap->data)[heap->heapsize-1]->value=value;
	
	//If the inserted node has a parent and is (*comparison) its parent, then swap them
	int currentpos=heap->heapsize-1;
	while(currentpos!=0&&(*comparison)((heap->data)[currentpos]->value,(heap->data)[((currentpos+1)/2)-1]->value)) {
		swap(&((heap->data)[currentpos]),&((heap->data)[((currentpos+1)/2)-1]));
		currentpos=((currentpos+1)/2)-1;
	}
	
	return 0;
}

int HEAP_deleteHead(Heap* heap){
	//Used to change between maxheap and minheap.
	//Maxheap uses lessThan (we want the largest of the children on top)
	//Minheap uses greaterThan (we want the smallest of the children on top)
	bool (*comparison)(double,double);
	if(heap->Maxheap) {
		comparison=&lessThan;
	}
	else {
		comparison=&greaterThan;
	}
	//If this is the last node, just delete everything
	if(heap->heapsize==1)
	{
		free(heap->data[0]->data);
		free(heap->data[0]);
		free(heap->data);
		heap->data=NULL;
		heap->heapsize=0;
		return 0;
	}
	//Else
	//Make last node the new root
	swap(&((heap->data)[heap->heapsize-1]),&((heap->data)[0]));
	//Delete the new last node (which was the head)
	free((heap->data)[heap->heapsize-1]->data);
	free((heap->data)[heap->heapsize-1]);
	heap->heapsize=heap->heapsize-1;
	//While root is (*comparison) to one of its children (if it has them), 
	//swap with the child that is !(*comparison) the other child.
	int currentpos = 0;
	while(((currentpos+1)*2 <= heap->heapsize //Check for left child exists
		&& (*comparison)((heap->data)[currentpos]->value, (heap->data)[((currentpos+1)*2)-1]->value))
		|| (((currentpos+1)*2)+1 <= heap->heapsize //Check for right child exists
		&& (*comparison)((heap->data)[currentpos]->value, (heap->data)[((currentpos+1)*2)]->value))) 
	{
		//If there is only one child or left child is not (*comparison) right child
		//(That means left child is !lessThan right child for Maxheap
		//and left child is !greaterThan right child for Minheap)
		if(((currentpos+1)*2)+1 > heap->heapsize || 
			!(*comparison)((heap->data)[((currentpos+1)*2)-1]->value,(heap->data)[((currentpos+1)*2)]->value))
		{
			//Swap with left child
			swap(&((heap->data)[currentpos]),&((heap->data)[((currentpos+1)*2)-1]));
			currentpos=((currentpos+1)*2)-1;
		}
		else
		{
			//Swap with right child
			swap(&((heap->data)[currentpos]),&((heap->data)[((currentpos+1)*2)]));
			currentpos=((currentpos+1)*2);
		}
	}
	
	return 0;
}

Heapnode* HEAP_getHead(Heap* heap){
	if(heap->data==NULL)
	{
		return NULL;
	}
	return *(heap->data);
}

Heapnode ** HEAP_getVector(Heap* heap) {
	return heap->data;
}

int HEAP_getHeapSize(Heap* heap){ 
	return heap->heapsize;
}

size_t HEAP_getDataSize(Heap* heap)
{
	return heap->datasize;
}

bool HEAP_isMaxheap(Heap* heap)
{
	return heap->Maxheap;
}

void HEAP_deleteHeap(Heap* heap){ 
	int i;
	if(heap->data!=NULL)
	{
		for(i=0 ; i < heap->heapsize ; ++i)
		{
			free((heap->data)[i]->data);
			free((heap->data)[i]);
		}
		free(heap->data);
	}
	free(heap);
}