#ifndef __DATAOBJECTLIST__INCLUDED__
#define __DATAOBJECTLIST__INCLUDED__

#include "DataObject.hpp"

class DataObjectListObject {
public:
	DataObject * dataObject;
	DataObjectListObject * next;
	/* Constructs a node that contains <data> and points to NULL
	 */
	DataObjectListObject(DataObject * cData);
};

class DataObjectList
{
private:
	DataObjectListObject * root;
public:
	DataObjectList();
	/* Inserts a DataObject at the end of the list.
	 */
	void push(DataObject * data);
	/* Removes a DataObject from the beginning of the list and returns it.
	 * Returns NULL for empty.
	 */
	DataObject * pop();
	~DataObjectList();

};

#endif // __DATAOBJECTLIST__INCLUDED__
