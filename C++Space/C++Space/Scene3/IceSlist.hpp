//Since templates are compiled on demand the entire template class must be present in the header file.

 /* Note to self: Although the list can support many more then 6GB data worth of members, 
  * it still has an ultimate limit of UINT_MAX members due to its itemcounter. 
  * To enable support for more, simply remove all lines containing the variable itemcount,
  * the function object_ammount() and the if-then blocks containing itemcount.
  */ 
#ifndef __IceSIceSlist__
#define __IceSIceSlist__

#include "exceptions.hpp"
#include <cstddef>
#include <climits>
#include <iostream>

//using namespace std;

template <class TC>
class List
{
	struct qnode{
		TC* data;
		qnode* next;
		qnode* previous;
	};
	qnode* start;
	qnode* finish;
	unsigned int itemcount;
	void insert_first(const TC & inserted)
	{
		start=finish=new qnode;
		try
		{
			start->data=new TC(inserted);
		}
		catch (const std::exception& e)
		{
			delete start;
			start=finish=NULL;
			throw;
		}
		start->previous=NULL;
		start->next=NULL;
		++itemcount;
		//cout<<"First node inserted."<<endl;
		return;
	}
	void insert_first(TC * const inserted)
	{
		start=finish=new qnode;
		start->data=inserted;
		start->previous=NULL;
		start->next=NULL;
		++itemcount;
		//cout<<"First node inserted."<<endl;
		return;
	}
public:
	/* Remember that insert funtions with reference create copies of the object
	 * while functions with pointers take custody of the object, and make the list
	 * responsible for it. (So don't delete it outside of the list unless you like segfaults.)
	 * (Or better yet simply NULL the pointer immediatly after giving it to the list.)
	 * If the object is popped with a pointer then custody of the object is given to 
	 * the calling function.
	 */
	List()
	{
		start=NULL;
		finish=NULL;
		itemcount=0;
		//cout<<"List created."<<endl;
	}
	void insert_start(const TC & inserted)
	{
		if(itemcount==UINT_MAX)
		{
			//cerr<<"ERROR: List full"<<endl;
			throw OVERFLOW_LIST_ERR();
		}
		qnode* temp;
		if(start==NULL)
		{
				insert_first(inserted);
				return;
		}
		temp=start;
		start=new qnode;
		try
		{
			start->data= new TC(inserted);
		}
		catch (const std::exception& e)
		{
			delete start;
			start=temp;
			throw;
		}
		start->next=temp;
		start->previous=NULL;
		start->next->previous=start;
		++itemcount;
		//cout<<"Node inserted at start."<<endl;
		return;
	}
  	void insert_start(TC * const inserted)
	{
		qnode* temp;
		if(itemcount==UINT_MAX)
		{
			//cerr<<"ERROR: List full"<<endl;
			throw OVERFLOW_LIST_ERR();
		}
		if (inserted==NULL)
		{
			//cerr<<"ERROR: Wrong input. No node inserted."<<endl;
			throw NULL_INPUT_ERR();
		}
		if(start==NULL)
		{
			insert_first(inserted);
			return;
		}
		temp=start;
		start=new qnode;
		start->data=inserted;
		start->next=temp;
		start->previous=NULL;
		start->next->previous=start;
		++itemcount;
		//cout<<"Node inserted at start."<<endl;
		return;
	}
	  
	void insert_finish(const TC & inserted)
	{
		qnode* temp;
		if(itemcount==UINT_MAX)
		{
			//cerr<<"ERROR: List full"<<endl;
			throw OVERFLOW_LIST_ERR();
		}
		if(start==NULL)
		{
			insert_first(inserted);
			return;
		}
		temp=finish;
		finish=new qnode;
		try
		{
			finish->data= new TC(inserted);
		}
		catch (const std::exception& e)
		{
			delete finish;
			finish=temp;
			throw;
		}
		finish->previous=temp;
		finish->next=NULL;
		finish->previous->next=finish;
		++itemcount;
		//cout<<"Node inserted at finish."<<endl;
		return;
	}
	  
	void insert_finish(TC * const inserted)
	{
		qnode* temp;
		if(itemcount==UINT_MAX)
		{
			//cerr<<"ERROR: List full"<<endl;
			throw OVERFLOW_LIST_ERR();
		}
		if (inserted==NULL)
		{
			//cerr<<"ERROR: Wrong input. No node inserted."<<endl;
			throw NULL_INPUT_ERR();
		}
		if(start==NULL)
		{
			insert_first(inserted);
			return;
		}
		temp=finish;
		finish=new qnode;
		finish->data=inserted;
		finish->previous=temp;
		finish->next=NULL;
		finish->previous->next=finish;
		++itemcount;
		//cout<<"Node inserted at finish."<<endl;
		return;
	}
	  
	void delete_start()
	{
		qnode* temp=start;
		if(start==NULL)
		{
			//cout<<"No node to delete!"<<endl;
			return;
		}
		delete start->data;
		start=start->next;
		delete temp;
		--itemcount;
		if(start==NULL)
		{
			finish=NULL;
		}
		else
		{
			start->previous=NULL;
		}
		//cout<<"Node removed!"<<endl;
	}
	  
	void delete_finish()
	{
		qnode* temp=finish;
		if(start==NULL)
		{
			//cout<<"No node to delete!"<<endl;
			return;
		}
		delete finish->data;
		finish=finish->previous;
		delete temp;
		--itemcount;
		if(finish==NULL)
		{
			start=NULL;
		}
		else
		{
			finish->next=NULL;
		}
		//cout<<"Node removed!"<<endl;
	}
	  
	TC& get_start_referance()
	/* This isn't declared as a const function since it has the potential
	 * to alter members of the list.
	 */
	{
		if(start==NULL)
		{
			//cerr<<"ERROR: List empty!"<<endl;
			throw EMPTY_LIST_ERR();
		}
		return *(start->data);
	}
	  
	TC get_start_object() const
	{
		if(start==NULL)
		{
			//cerr<<"ERROR: List empty!"<<endl;
			throw EMPTY_LIST_ERR();
		}
		return *(start->data);
	}
	  
	TC pop_start_object()
	{
		if(start==NULL)
		{
			//cerr<<"ERROR: List empty!"<<endl;
			throw EMPTY_LIST_ERR();
		}
		TC temp(*(start->data));
		this->delete_start();
		return temp;
	}
	  
	TC* pop_start_pointer()
	{
		TC* tempd;
		qnode* temp=start;
		if(start==NULL)
		{
			//cout<<"List empty: NULL pointer got!"<<endl;
			return NULL;
		}
		tempd=start->data;
		start=start->next;
		delete temp;
		--itemcount;
		if(start==NULL)
		{
			finish=NULL;
		}
		else
		{
			start->previous=NULL;
		}
		//cout<<"Node removed!"<<endl;
		return tempd;
	}
	  
	TC& get_finish_referance()
	{
		if(start==NULL)
		{
			//cerr<<"ERROR: List empty!"<<endl;
			throw EMPTY_LIST_ERR();
		}
		return *(finish->data);
	}
	  
	TC get_finish_object() const
	{
		if(start==NULL)
		{
			//cerr<<"ERROR: List empty!"<<endl;
			throw EMPTY_LIST_ERR();
		}
		return *(finish->data);
	}
	  
	TC pop_finish_object()
	{
		if(start==NULL)
		{
			//cerr<<"ERROR: List empty!"<<endl;
			throw EMPTY_LIST_ERR();
		}
		TC temp(*(finish->data));
		this->delete_finish();
		return temp;
	}
	  
	TC* pop_finish_pointer()
	{
		TC* tempd;
		qnode* temp=finish;
		if(start==NULL)
		{
			//cout<<"List empty: NULL pointer got!"<<endl;
			return NULL;
		}
		tempd=finish->data;
		finish=finish->previous;
		delete temp;
		--itemcount;
		if(finish==NULL)
		{
			start=NULL;
		}
		else
		{
			finish->next=NULL;
		}
		//cout<<"Node removed!"<<endl;
		return tempd;
	}
	  
	unsigned int  object_ammount() const
	{
		return itemcount;
	}
	  
	TC& get_referance(unsigned int i)//Remember, it's base 1
	{
		qnode* temp;
		if(i>itemcount||i==0)
		{
			throw MEMBER_DOES_NOT_EXIST_ERR();
		}
		if(i<itemcount-i)
		{
			temp=start;
			for(unsigned int j=1;j<i;++j)
			{
				temp=temp->next;
			}
		}
		else
		{
			temp=finish;
			for(unsigned int j=itemcount;j>i;--j)
			{
				temp=temp->previous;
			}
		}
		return *(temp->data);
	}
	
	TC* pop_pointer(unsigned int i)//Remember, it's base 1
	{
		qnode* temp;
		TC* tempobj;
		if(i==1)
		{
			return pop_start_pointer();
		}
		else if(i==itemcount)
		{
			return pop_finish_pointer();
		}
		if(i>itemcount||i==0)
		{
			throw MEMBER_DOES_NOT_EXIST_ERR();
		}
		if(i<itemcount-i)
		{
			temp=start;
			for(unsigned int j=1;j<i;++j)
			{
				temp=temp->next;
			}
		}
		else
		{
			temp=finish;
			for(unsigned int j=itemcount;j>i;--j)
			{
				temp=temp->previous;
			}
		}
		temp->next->previous=temp->previous;
		temp->previous->next=temp->next;
		tempobj=temp->data;
		delete temp;
		return tempobj;
	}
	  
	~List()
	{
		while(start!=NULL)
		{
			delete_start();
		}
		//cout<<"List destroyed!"<<endl;
	}
};

#endif
