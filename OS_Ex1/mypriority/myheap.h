#ifndef __MYHEAP__
#define __MYHEAP__

/* A heap that stores data based on their value. 
 * The object with the largest or smallest value can be accessed.
 * It is implemented as a vector and can be accessed as such.
 * Position of  heap in memory may chenge during execution. Be careful with pointers.
 */

#include <stdbool.h>
#include <string.h>

typedef struct {
	double value;
	void* data;
} Heapnode;

typedef struct Heap Heap;

/* Creates a new heap for storing data of size <datasize> and returns a pointer to it.
 * If Maxheap==true it will be a maxheap, else a minheap.
 * Returns NULL for failure.
 */
Heap* HEAP_newHeap(bool Maxheap, size_t datasize);
 
/* Inserts the data on on the heap. 
 * Returns 0 for success and negative for failure.
 */
int HEAP_insertData (Heap* heap, void* data, double value);
 
/* Deletes the first node.
 * Returns 0 for success and negative for failure.
 */
int HEAP_deleteHead(Heap* heap);
 
/* Returns the first node. 
 * Returns NULL for empty.
 */
Heapnode* HEAP_getHead(Heap* heap);

/* Returns the vector of (Heapnode*) used to implement the tree.
 * Returns NULL for empty.
 */
Heapnode** HEAP_getVector(Heap* heap);

/* Returns the size of the heap (the number of entries contained in it).
 */ 
int HEAP_getHeapSize(Heap* heap);

/*Returns the size of the data contained in the heap.
 */
size_t HEAP_getDataSize(Heap* heap);

/* Returns true for maxheap and false for minheap.
 */
bool HEAP_isMaxheap(Heap* heap);
 
/* Deletes the entire heap.
 */
void HEAP_deleteHeap(Heap* heap);
 
#endif