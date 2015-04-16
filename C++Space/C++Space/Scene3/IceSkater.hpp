#ifndef __X3IceSkater__
#define __X3IceSkater__

#include "lakegrid.hpp"
#include "pos2D.hpp"

#define __ICESMAXTRIES__ 9001
//The maximum number of tries before the IceSkater moves to another tile after a collision without checking if it is empty.

enum IceStype {UNKNOWNS=-1,ICESKATER=1, ICESNORMAL=2, ICESBEGINNER=3, ICESCHILD=4, ICESEXPERT=5};

enum direction {UNKNOWN=-1,NORTH=1,NORTHEAST=2,EAST=3,SOUTHEAST=4,SOUTH=5,SOUTHWEST=6,WEST=7,NORTHWEST=8};
//Notice that if i=<some direction other than unknown> then i%8+1=<i + 45 degrees> and (i+6)%8+1=<i - 45 degrees>
//So if i=NORTH then i%8+1=NORTTHEAST and (i+6)%8+1=NORTHWEST

  ///////////////////////////////////////
 //All IceSkater are derivatives here.//
///////////////////////////////////////

//Πατέρας
class IceSkater
{
	const unsigned long id;//Constant and unique
protected:
	Lakegrid<IceSkater>& lakegrid;	//Where is the IceSkater?
	pos2D start;		//The route's start
	pos2D end;			//The route's end
	pos2D current;		//Where the IceSkater is in the end.
	//Remember, adding makes it more east, subtracting more west //The numbers after 1 are not neccessary, but look nicer
	direction heading;	//Which direction the IceSkater is going
	bool hasmoved;		//Has the IceSkater completed its movement?
	unsigned int time;	//How much longer can the IceSkater stay.
	IceSkater* pair;	//Its pair
	unsigned int speed;	//How many tiles per turn does it move? (Can become double if non-integer speeds are needed. If you do, remember to change the constructor.)
	bool check_pos_for_pair_OK(IceSkater& caller, pos2D tocheck) const; //Used to check if the pair can move next to the caller, true if yes
public:
	IceSkater(Lakegrid<IceSkater>& Lakegrid,unsigned long Id,unsigned int Speed, unsigned int Time, pos2D pos);
	IceSkater(Lakegrid<IceSkater>& Lakegrid,unsigned long Id,unsigned int Speed, unsigned int Time, unsigned int x, unsigned int y);	//Remember x:east:right, y:south:down
	unsigned long getid() const;			//Just give the id.
	bool hasPair() const;					//Does it have a pair?
	bool has_Moved() const;					//Has it moved?
	pos2D getpos() const;					//Returns the objects' current position
	void setpair(IceSkater* apair);			//Give it a pair. Change speed to match the slowest one. Anything else.
	//Done
	virtual void collision();				//First check wether or not the IceSkater has moved. Then fall acordingly. (If he has moved fall on an adjacent square. If not, fall somewhere along the way) Set heading as Unknown. Set hasMoved.		
	//Done, maybe needs check for infinite loop.
	virtual void paircollision();			//Called to indicate that the IceSkater's pair has fallen.															
	//Done, maybe needs check for infinite loop.
	virtual void move();					//Move, check along the way for others, if move is completed sucessfuly update current position and set has moved. Also check if the pair is going to stop sooner.
	//(Maybe doesn't need to be virtual?)
	//Done
	virtual void prepare_move();			//Has moved=false Set startpos, endpos and heading. Turn pairs if necessary. Ensure that endpos is within the boundaries.
	//(Maybe doesn't need to be virtual?)					
	//Done, but possible infinite loop in the first few lines. Check again.
	virtual direction propose_direction() const;	//Proposes a direction for movement.
	//Done, but can be made more random.
	virtual void end_move();				//Check if it is time to leave.
	//Done
	virtual void goodbye_msg() const;		//When the object is deleted, it prints this message.
	virtual bool posOK(pos2D pos) const;	//Is the position acceptabe?
	virtual IceStype givetype() const;		//Returns the object's type (ICESKATER for this one)
	void force_move(pos2D pos);				//Teleports the IceSkater
	void sethasmoved(bool flag);			//Sets the has moved flag
	void unpair();
	direction getheading() const;			//Returns the heading var;
	pos2D getstartpos() const;				//Returns the starting position
	pos2D getendpos() const;				//Returns the ending position
	virtual bool isfaller() const;			//Returns true if the object falls after a collision (in this case always true)
	virtual ~IceSkater();					//Also deletes the pair if necessary.
};

//Κοινός
class IceSnormal:public IceSkater
{
protected:
	double maxDratio;	//maxDratio can't be greater than 0.3	//The maximum possible distance from the side this one is moving parallelly to is maxDratio*side vertical to the one this one is moving parallel to
public:
	IceSnormal(Lakegrid<IceSkater>& Lakegrid,unsigned long Id,unsigned int Speed, unsigned int Time, unsigned int x, unsigned int y, unsigned short MaxDratio);
	IceSnormal(Lakegrid<IceSkater>& Lakegrid,unsigned long Id,unsigned int Speed, unsigned int Time, pos2D pos, unsigned short MaxDratio);
	virtual direction propose_direction() const;	//Proposes a direction for movement. No diagonals. Check for turns
	virtual bool posOK(pos2D pos) const;
	virtual void collision();//Needs to check that the resulting position is OK
	//Done
	virtual void paircollision();//Needs to check the the resulting position is OK
	//Done
	virtual void prepare_move();//Needs to check the the resulting position is OK
	//Done
	virtual IceStype givetype() const;		//Returns the object's type (ICESNORMAL for this one)
	~IceSnormal();
};

//Αρχάριος
class IceSbeginner:public IceSkater//The speed will be one Luke, always.
{
protected:
	double maxDratio;	//maxDratio is always 0.1	//The maximum possible distance from the side this one is moving parallelly to is maxDratio*side vertical to the one this one is moving parallel to
public:
	IceSbeginner(Lakegrid<IceSkater>& Lakegrid,unsigned long Id, unsigned int Time, unsigned int x, unsigned int y, unsigned short MaxDratio);
	IceSbeginner(Lakegrid<IceSkater>& Lakegrid,unsigned long Id, unsigned int Time, pos2D pos, unsigned short MaxDratio);
	virtual direction propose_direction() const;	//Proposes a direction for movement. Check for turns
	virtual bool posOK(pos2D pos) const;
	virtual void collision();//Needs to check that the resulting position is OK, same as IceSnormal
	//Done
	virtual void paircollision();//Needs to check the the resulting position is OK, same as IceSnormal
	//Done
	virtual void prepare_move();//Needs to check the the resulting position is OK, same as IceSnormal
	//Not completed, need to make extra directions and ensure it's within bounds.
	virtual IceStype givetype() const;		//Returns the object's type (ICESBEGINNER for this one)
	~IceSbeginner();
};

//Παιδί
class IceSchild:public IceSbeginner
{
protected:
	unsigned int falls;	//After maxfalls reaches zero, the kid leaves (essentially a different time counter)
public:
	IceSchild(Lakegrid<IceSkater>& Lakegrid,unsigned long Id, unsigned int Time, unsigned int x, unsigned int y, unsigned short MaxDratio, int Maxfalls);
	IceSchild(Lakegrid<IceSkater>& Lakegrid,unsigned long Id, unsigned int Time, pos2D pos, unsigned short MaxDratio, int Maxfalls);
	virtual void collision();	//Needs to decrement falls and display custom message, same as IceSbeginner
	//Done
	virtual void paircollision();//Needs to decrement falls and display custom message, same as IceSbeginner
	//Done
	virtual void end_move();	//Needs to check falls
	//Done
	virtual void goodbye_msg() const;	//Needs to display custom message
	virtual IceStype givetype() const;		//Returns the object's type (ICESCHILD for this one)
	~IceSchild();
};

//Προχωρημένος
class IceSexpert:public IceSkater
{
public:
	IceSexpert(Lakegrid<IceSkater>& Lakegrid,unsigned long Id,unsigned int Speed, unsigned int Time, pos2D pos);
	IceSexpert(Lakegrid<IceSkater>& Lakegrid,unsigned long Id,unsigned int Speed, unsigned int Time, unsigned int x, unsigned int y);
	virtual void collision();	//Doesn't call paircollision, continues on
	//Done
	virtual void move();		//Always completes its move
	//Done
	virtual IceStype givetype() const;		//Returns the object's type (ICESEXPERT for this one)
	virtual bool isfaller() const;			//Returns false
	~IceSexpert();
};

#endif
