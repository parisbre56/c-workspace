#include "DataObjectList.hpp"

using namespace std;

DataObjectListObject::DataObjectListObject(DataObject * cData) {
	this->next=NULL;
	this->dataObject=cData;
}

DataObjectList::DataObjectList()
{
	this->root=NULL;
}

void DataObjectList::push(DataObject* data) {
	if(root==NULL) {
		root=new DataObjectListObject(data);
		return;
	}
	DataObjectListObject* temp=root;
	while(temp->next!=NULL) {
		temp=temp->next;
	}
	temp->next=new DataObjectListObject(data);
}

DataObject* DataObjectList::pop() {
	if(root==NULL) {
		return NULL;
	}
	DataObject* retThread=root->dataObject;
	DataObjectListObject* temp=root;
	root=root->next;
	delete temp;
	return retThread;
}

DataObjectList::~DataObjectList()
{
	//Delete all thread objects to clear memory
	DataObject * thread=NULL;
	while((thread=this->pop())!=NULL) {
		delete thread;
	}
}

