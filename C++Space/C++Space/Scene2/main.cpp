/* TODO:
 * INFO ON ICESKATER SHOULD BE DISPLAYED ON LAKE, NOT WHEN DELETED!!! (put it in private act function in lake)
 * THE RANDOM ID SELECTOR IS TOO RESOURCE INTENSIVE!!! BUILD A BETTER ONE!!! (did one, however it can become "more random")
 * USE DEFAULT ID VARIABLE IN CLONE_ICESKATER!!! (no way to do that now except with multiple functions, try again later)
 * SPLIT INTO MULTILE FILES!!!
 * THE TRY-CATCH IN SOME PLACES CAN BE REPLACED BY TAKING ADVANTAGE OF THE FACT THAT OUT-OF-SCOPE OBJECTS ARE DELETED. (this however will result in reduced performance)
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <climits>

using namespace std;

//Exceptions

class IMPOSSIBLE_ERR:public exception
{
public:
	const char* what() const throw()
	{
		return "IMPOSSIBLE ERROR: CHECK YOUR CODE!!!";
	}
};
class NULL_INPUT_ERR:public exception
{
public:
	const char* what() const throw()
	{
		return "NULL POINTER ERROR: NULL POINTER GIVEN AS INPUT!!!";
	}
};
class EMPTY_LIST_ERR:public exception
{
public:
	const char* what() const throw()
	{
		return "EMPTY LIST ERROR: UNABLE TO PERFORM OPERATION ON EMPTY LIST!!!";
	}
};

//Enumerators
#define COLOURNUM 2
#define SKILLNUM 2

enum CR {UNKNOWNC=-1,BLACK=0,WHITE=1};
enum SL {UNKNOWNS=-1,SPIN=0,JUMP=1};

class COLOUR
{
	CR c;
public:
	COLOUR(CR y=UNKNOWNC)
	{
		c=y;
	}
	COLOUR (int y)
	{
		*this=y;
	}
	operator int() const
	{
		switch(c)
		{
			case UNKNOWNC:
				return UNKNOWNC;
			break;
			case BLACK:
				return BLACK;
			break;
			case WHITE:
				return WHITE;
			break;
			default:
				return -1;
			break;
		}
	}
	operator CR() const
	{
		return c;
	}
	friend ostream& operator<<(ostream& os,const COLOUR& C)
	{
		switch(C.c)
		{
			case UNKNOWNC:
				os<<"UNKNOWN_COLOUR";
			break;
			case BLACK:
				os<<"Black";
			break;
			case WHITE:
				os<<"White";
			break;
			default:
				os<<"UNKNOWN_COLOUR";
			break;
		}
		return os;
	}
	COLOUR& operator=(int y)
	{
		switch(y)
		{
			case UNKNOWNC:
				c=UNKNOWNC;
			break;
			case BLACK:
				c=BLACK;
			break;
			case WHITE:
				c=WHITE;
			break;
			default:
				c=UNKNOWNC;
			break;
		}
		return *this;
	}
	COLOUR& operator=(CR y)
	{
		c=y;
		return *this;
	}
	/* The functions below are not necessary but are kept just in case 
	 * there is ambiguity in a conversion. (For example if COLOUR a,b and
	 * we do a==b then the compiler will not know whether to call CR==CR or int==int
	 * since both functions exist and both conversions are provided. Thus, by keeping
	 * the functions below we avoid having to cast like this int(a)==int(b) or 
	 * this CR(a)==CR(b))
	 */
	bool operator ==(const COLOUR& y) const
	{
		return c==y.c;
	}
	bool operator ==(CR y) const
	{
		return c==y;
	}
	bool operator ==(int y) const
	{
		return COLOUR(y)==c;
	}
	bool operator !=(const COLOUR& y) const
	{
		return c!=y.c;
	}
	bool operator !=(CR y) const
	{
		return c!=y;
	}
	bool operator !=(int y) const
	{
		return COLOUR(y)!=c;
	}
};

class SKILL
{
	SL c;
public:
	SKILL(SL y=UNKNOWNS)
	{
		c=y;
	}
	SKILL (int y)
	{
		*this=y;
	}
	operator int() const
	{
		switch(c)
		{
			case UNKNOWNS:
				return UNKNOWNS;
			break;
			case SPIN:
				return SPIN;
			break;
			case JUMP:
				return JUMP;
			break;
			default:
				return -1;
			break;
		}
	}
	operator SL() const
	{
		return c;
	}
	friend ostream& operator<<(ostream& os,const SKILL& C)
	{
		switch(C.c)
		{
			case UNKNOWNS:
				os<<"UNKNOWN_SKILL";
			break;
			case SPIN:
				os<<"Spinning";
			break;
			case JUMP:
				os<<"Jumping";
			break;
			default:
				os<<"UNKNOWN_SKILL";
			break;
		}
		return os;
	}
	SKILL& operator=(int y)
	{
		switch(y)
		{
			case UNKNOWNS:
				c=UNKNOWNS;
			break;
			case SPIN:
				c=SPIN;
			break;
			case JUMP:
				c=JUMP;
			break;
			default:
				c=UNKNOWNS;
			break;
		}
		return *this;
	}
	SKILL& operator=(SL y)
	{
		c=y;
		return *this;
	}
	//Same thing as in colour, functions to escape ambiguity
	bool operator ==(const SKILL& y) const
	{
		return c==y.c;
	}
	bool operator ==(SL y) const
	{
		return c==y;
	}
	bool operator ==(int y) const
	{
		return SKILL(y)==c;
	}
	bool operator !=(const SKILL& y) const
	{
		return c!=y.c;
	}
	bool operator !=(SL y) const
	{
		return c!=y;
	}
	bool operator !=(int y) const
	{
		return SKILL(y)!=c;
	}
};

//Classes
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
		catch (const exception& e)
		{
			delete start;
			start=finish=NULL;
			throw e;
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
	/* Remember that funtions with reference create copies of the object
	 * while functions with pointers take custody of the object, and make the list
	 * responsible for it. If the object is popped then custody of the object
	 * is given to the calling function.
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
		qnode* temp;
		if(itemcount==0)
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
		catch (const exception& e)
		{
			delete start;
			start=temp;
			throw e;
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
		if (inserted==NULL)
		{
			cerr<<"ERROR: Wrong input. No node inserted."<<endl;
			throw NULL_INPUT_ERR();
		}
		if(itemcount==0)
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
		if(itemcount==0)
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
		catch (const exception& e)
		{
			delete finish;
			finish=temp;
			throw e;
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
		if (inserted==NULL)
		{
			cerr<<"ERROR: Wrong input. No node inserted."<<endl;
			throw NULL_INPUT_ERR();
		}
		if(itemcount==0)
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
		if(itemcount==0)
		{
			//cout<<"No node to delete!"<<endl;
			return;
		}
		delete start->data;
		start=start->next;
		delete temp;
		if(--itemcount==0)
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
		if(itemcount==0)
		{
			//cout<<"No node to delete!"<<endl;
			return;
		}
		delete finish->data;
		finish=finish->previous;
		delete temp;
		if(--itemcount==0)
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
		if(itemcount==0)
		{
			cerr<<"ERROR: List empty!"<<endl;
			throw EMPTY_LIST_ERR();
		}
		return *(start->data);
	}
	TC get_start_object() const
	{
		if(itemcount==0)
		{
			cerr<<"ERROR: List empty!"<<endl;
			throw EMPTY_LIST_ERR();
		}
		return *(start->data);
	}
	TC pop_start_object()
	{
		if(itemcount==0)
		{
			cerr<<"ERROR: List empty!"<<endl;
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
		if(itemcount==0)
		{
			//cout<<"List empty: NULL pointer got!"<<endl;
			return NULL;
		}
		tempd=start->data;
		start=start->next;
		delete temp;
		if(--itemcount==0)
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
		if(itemcount==0)
		{
			cerr<<"ERROR: List empty!"<<endl;
			throw EMPTY_LIST_ERR();
		}
		return *(finish->data);
	}
	TC get_finish_object() const
	{
		TC* temp;
		if(itemcount==0)
		{
			cerr<<"ERROR: List empty!"<<endl;
			throw EMPTY_LIST_ERR();
		}
		return *(finish->data);
	}
	TC pop_finish_object()
	{
		if(itemcount==0)
		{
			cerr<<"ERROR: List empty!"<<endl;
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
		if(itemcount==0)
		{
			//cout<<"List empty: NULL pointer got!"<<endl;
			return NULL;
		}
		tempd=finish->data;
		finish=finish->previous;
		delete temp;
		if(--itemcount==0)
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
	unsigned int object_ammount() const
	{
		return itemcount;
	}
	~List()
	{
		while(itemcount!=0)
		{
			delete_start();
		}
		//cout<<"List destroyed!"<<endl;
	}
};

class IceSkater
{
	unsigned int distance;
	COLOUR colour;
	SKILL skill;
	unsigned int position;
	unsigned int skatenum;
public:
	const int id; //ID is made public for easy access and const since it should not change
	IceSkater(int Id, int Distance,int Colour,int Skill):id(Id)
	{
		distance=Distance;
		position=0;
		skatenum=0;
		colour=Colour;
		skill=Skill;
		cout<<"An IceSkater participates!"<<endl;
	}
	IceSkater(const IceSkater & other):id(other.id)
	{
		distance=other.distance;
		position=other.position;
		colour=other.colour;
		skill=other.skill;
		skatenum=other.skatenum;
		cout<<"An IceSkater was cloned!"<<endl;
	}
	IceSkater(const IceSkater & other,int Id):id(Id)
	{
		distance=other.distance;
		position=other.position;
		colour=other.colour;
		skill=other.skill;
		skatenum=other.skatenum;
		cout<<"An IceSkater was cloned and given a new ID!"<<endl;
	}
	void perform() const
	{
		cout<<"My skill is "<<skill<<'.'<<endl;;
	}
	void skate(unsigned int distance)
	{
		//unsigned int temp;
		skatenum+=/*temp=*/(((distance/this->distance)*(this->distance)<distance)?((distance/this->distance)+1):(distance/this->distance));//Round up division (only works for positive int)
		position+=distance;
		//cout<<"An IceSkater skates "<<temp<<" times."<<endl;
	}
	void rotate() const
	{
		cout<<"An IceSkater spins on position, "<<position<<" units away from the edge of the lake."<<endl;
	}
	void jump() const
	{
		cout<<"An IceSkater jumps on position, "<<position<<" units away from the edge of the lake."<<endl;
	}
	COLOUR getcolour() const
	{
		return colour;
	}
	SKILL getskill() const
	{
		return skill;
	}
	unsigned int getskatenum() const
	{
		return skatenum;
	}
	~IceSkater()
	{
		cout<<"An IceSkater will be discarted."<<endl;
	}
};

class Lake
{
	unsigned int length;
	List<IceSkater> UpperLeftStack;
	List<IceSkater> UpperRightStack;
	List<IceSkater> LowerLeftStack;
	List<IceSkater> LowerRightStack;
public:
	Lake(unsigned int Length)
	{
		length=Length;
		cout<<"A lake appears."<<endl;
	}
	List<IceSkater>& get_ul_referance()
	{
		return UpperLeftStack;
	}
	List<IceSkater>& get_ll_referance()
	{
		return LowerLeftStack;
	}
	List<IceSkater>& get_ur_referance()
	{
		return UpperRightStack;
	}
	List<IceSkater>& get_lr_referance()
	{
		return LowerRightStack;
	}
	inline int getlength()
	{
		return length;
	}
	~Lake()
	{
		cout<<"A lake will dissapear!"<<endl;
	}
};

class Scene
{
	Lake lake;
	List<IceSkater> mainqueue;
	List<IceSkater> secqueue;
	unsigned int skaternum;
	
	inline void fillqueue()
	{
		COLOUR maincolour=UNKNOWNC;
		COLOUR seccolour=UNKNOWNC;
		COLOUR curcolour=UNKNOWNC;
		IceSkater* tempis;
		unsigned int tempid=0-1; //The -1 is so that it will become either 0 or 1 when it next changes.
		unsigned int blacknum=0;
		unsigned int created=0; //the numbers of skaters created so far.
		//Begin filling the main queue (mainqueue) with the help of the secondary queue (secqueue)
		if (skaternum==1)
		{
			tempis=new IceSkater(rand()%2,((lake.getlength()==1)?1:((rand()%(lake.getlength()-1))+1)),curcolour=rand()%COLOURNUM,rand()%SKILLNUM);
			try
			{
				mainqueue.insert_start(tempis);
				cout<<"Inserted IceSkater "<<mainqueue.get_start_referance().id<<" to the MainQueue."<<endl;
			}
			catch (const exception& e)
			{
				delete tempis;
				throw e;
			}
			++created;
			if (curcolour==BLACK)
			{
				++blacknum;
			}
			maincolour=curcolour;
		}
		while (created!=skaternum)
		{
			if (maincolour!=seccolour&&seccolour!=UNKNOWNC)
			{
				tempis=secqueue.pop_finish_pointer();
				cout<<"Removed IceSkater "<<tempis->id<<" from the SecondaryQueue."<<endl;
				mainqueue.insert_start(tempis);
				cout<<"Inserted IceSkater "<<mainqueue.get_start_referance().id<<" to the MainQueue."<<endl;
				maincolour=seccolour;
				if(secqueue.object_ammount()==0)
				{
					seccolour=UNKNOWNC;
				}
				else
				{
					seccolour=secqueue.get_finish_referance().getcolour();
				}
				continue;
			}
			++tempid+=rand()%2;//Next_id=previous_id+1 or previous_id+2
			//All IDs are random. That they have a certain property (next id > previous id) does not efect their randomness.
			if(blacknum==skaternum/2)
			{
				tempis=new IceSkater(tempid,((lake.getlength()==1)?1:((rand()%(lake.getlength()-1))+1)),curcolour=WHITE,rand()%SKILLNUM);
			}
			else if(created-blacknum==skaternum/2)
			{
				tempis=new IceSkater(tempid,((lake.getlength()==1)?1:((rand()%(lake.getlength()-1))+1)),curcolour=BLACK,rand()%SKILLNUM);
				++blacknum;
			}
			else
			{
				tempis=new IceSkater(tempid,((lake.getlength()==1)?1:((rand()%(lake.getlength()-1))+1)),curcolour=rand()%COLOURNUM,rand()%SKILLNUM);
				if (curcolour==BLACK)
				{
					++blacknum;
				}
			}
			++created;
			if(curcolour==maincolour)
			{
				try //If there is any problem after this delete the temp IceSkater before exiting.
				{
					secqueue.insert_start(tempis);
				}
				catch (const exception& e)
				{
					delete tempis;
					throw e;
				} //tempis is secqueue's responsibility now, so no need to check.
				cout<<"Inserted IceSkater "<<secqueue.get_start_referance().id<<" to the SecondaryQueue."<<endl;
				seccolour=secqueue.get_finish_referance().getcolour();
			}
			else
			{
				maincolour=curcolour;
				try //Same thing as the previous try.
				{
					mainqueue.insert_start(tempis);
				}
				catch (const exception& e)
				{
					delete tempis;
					throw e;
				}
				cout<<"Inserted IceSkater "<<mainqueue.get_start_referance().id<<" to the MainQueue."<<endl;
			}
		}
		//If there are still skaters in secqueue empty it
		if (seccolour!=UNKNOWNC)
		{
			if (maincolour==seccolour)//Should only happen with odd number of skaters
			{
				cerr<<"WARNING: Same coloured skater pair! Inserting at finish..."<<endl;
				if(mainqueue.get_finish_referance().getcolour()==seccolour)//Should never happen
				{
					cerr<<"ERROR: Impossible! First skater also has same colour!"<<endl;
					throw IMPOSSIBLE_ERR();
				}
				tempis=secqueue.pop_finish_pointer();
				cout<<"Removed IceSkater "<<tempis->id<<" from the SecondaryQueue."<<endl;
				mainqueue.insert_finish(tempis);
				cout<<"Inserted IceSkater "<<mainqueue.get_finish_referance().id<<" at the first place of the MainQueue."<<endl;
			}
			else
			{
				tempis=secqueue.pop_finish_pointer();
				cout<<"Removed IceSkater "<<tempis->id<<" from the SecondaryQueue."<<endl;
				mainqueue.insert_start(tempis);
				cout<<"Inserted IceSkater "<<mainqueue.get_start_referance().id<<" to the MainQueue."<<endl;
			}
		}
		if (secqueue.object_ammount()!=0)//Should never happen
		{
			cerr<<"ERROR: Objects still in secqueue!"<<endl;
			throw IMPOSSIBLE_ERR();
		}
		if (mainqueue.object_ammount()!=skaternum||created!=skaternum)//Should never happen
		{
			cerr<<"ERROR: Wrong number of skaters in mainqueue!"<<endl;
			throw IMPOSSIBLE_ERR();
		}
	}
	inline void filllake()
	{
		List<IceSkater>* stacks[4];
		string stackname[4];
		IceSkater* tempis;
		
		stacks[0]=&lake.get_ll_referance();
		stackname[0]="LowerLeft";
		stacks[1]=&lake.get_lr_referance();
		stackname[1]="LowerRight";
		stacks[2]=&lake.get_ur_referance();
		stackname[2]="UpperRight";
		stacks[3]=&lake.get_ul_referance();
		stackname[3]="UpperLeft";
		
		
		//Remove from mainqueue and put into stacks, counter clockwise, starting from LowerLeft
		for(unsigned int i=0;mainqueue.object_ammount()!=0;++i%=4)
		{
			if (stacks[i]->object_ammount()==0||mainqueue.get_finish_referance().getcolour()!=stacks[i]->get_start_referance().getcolour())
			{
				cout<<"Removing IceSkater "<<mainqueue.get_finish_referance().id<< " from the MainQueue."<<endl;
				tempis=mainqueue.pop_finish_pointer();
				try
				{
					cout<<"Inserting IceSkater "<<tempis->id<<" to the "<<stackname[i]<<"Stack."<<endl;
					stacks[i]->insert_start(tempis);
				}
				catch (const exception &e)
				{
					delete tempis;
					throw e;
				}
			}
		}
	}
	inline void action()
	{
		List<IceSkater>* stacks[4];
		string stackname[4];
		IceSkater* tempis;
		
		stacks[0]=&lake.get_ll_referance();
		stackname[0]="LowerLeft";
		stacks[1]=&lake.get_lr_referance();
		stackname[1]="LowerRight";
		stacks[2]=&lake.get_ur_referance();
		stackname[2]="UpperRight";
		stacks[3]=&lake.get_ul_referance();
		stackname[3]="UpperLeft";
		
		for(unsigned int i=0,j=0;j!=skaternum;++i%=4)
		{
			if(stacks[i]->object_ammount()!=0)
			{
				cout<<"Removing IceSkater "<<stacks[i]->get_start_referance().id<<" from the "<<stackname[i]<<"Stack."<<endl;
				tempis=stacks[i]->pop_start_pointer();
				try
				{
					act(*tempis);
					++j;
				}
				catch (const exception &e)
				{
					delete tempis;
					throw e;
				}
				delete tempis;
			}
		}
	}
	inline void act(IceSkater& tempis)
	{
		tempis.perform();
		tempis.skate(lake.getlength());
		cout<<"IceSkater left the lake: ID: "<<tempis.id<<", COLOUR: "<<tempis.getcolour()<<", TIMES SKATED: "<<tempis.getskatenum()<<endl;
	}
public:
	Scene(unsigned int Length, unsigned int Number):lake(Length)
	{
		if (Number>(UINT_MAX/2))
		{
			cerr<<"WARNING: Number of iceskaters too large, ID overflow possible."<<endl;
		}
		skaternum=Number;
		cout<<"Scene was just created!"<<endl;
	}
	void plot()
	{
		this->fillqueue();
		this->filllake();
		this->action();
	}
	~Scene()
	{
		cout<<"Scene is about to be destroyed."<<endl;
	}
};

int main(int argc, char **argv)
{
	if (argc!=3)
	{
		cerr<<"ERROR: Need exactly 2 arguements."<<endl;
		exit(EXIT_FAILURE);
	}
	try
	{
		srand(time(NULL));
		
		unsigned int L = atoi(argv[1]);
		if(L==0)
		{
			cerr<<"ERROR: Lake length must be non-zero."<<endl;
			exit(EXIT_FAILURE);
		}
		unsigned int N = atoi(argv[2]);
		
		Scene scene(L,N);
		scene.plot();
	}
	catch(const exception &e)
	{
		cerr<<"ERROR: Unhandled Exception Caught: "<<e.what()<<endl;
		exit(EXIT_FAILURE);
	}
	catch(...)
	{
		cerr<<"ERROR: Unknown Exception Caught. Check your code."<<endl;
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
