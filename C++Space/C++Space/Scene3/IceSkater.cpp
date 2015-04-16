//MAKE ALL posOK= AND ALL COLLISIONS AND ALL PREPARE MOVE TURNS DO THIS FOR THE PAIR pairOK=check_pos_for_pair_OK(*this,pos2D((unsigned int)current.x+pair->getstartpos().x-(unsigned int)pair->getstartpos().x,(unsigned int)current.y+pair->getstartpos().y-(unsigned int)pair->getstartpos().y+1));
//Maybe remove the edge check for pair for all prepare_move. Move's check will take care of it
//Some checks in prepare_move unnecessary
//Maybe make some propose_direction checks use unsigned ints or check if it has a pair or something like that to avoid having IceSkaters stay in the same place
//Maybe use a temp variable or more actions to supress warnings.
//Maybe fix some probable infinite loops.
//Fix collisions
//Put default in switch

#include "IceSkater.hpp"
#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;

template <typename T> inline int sgn(T val)
//Simple template function to save some space/typing.
{
    return (T(0) < val) - (val < T(0));
}

  /////////////////////////////////
 //			IceSkater			//
/////////////////////////////////

inline bool IceSkater::check_pos_for_pair_OK(IceSkater& caller,pos2D tocheck) const
{
	return caller.pair->posOK(pos2D(tocheck.x-1,tocheck.y))||
		caller.pair->posOK(pos2D(tocheck.x,tocheck.y-1))||
		caller.pair->posOK(pos2D(tocheck.x+1,tocheck.y))||
		caller.pair->posOK(pos2D(tocheck.x,tocheck.y+1));
}

void IceSkater::force_move(pos2D pos)
{
	unsigned int i;
	pos2D temppos;
	for(i=1;&(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->get_referance(i))!=this;++i)//Find this one in the list
	{
		
	}
	lakegrid.grid[(unsigned int)(temppos.x=(pos.x))][(unsigned int)(temppos.y=(pos.y))]
		->insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
	current=temppos;
}

IceSkater::IceSkater(Lakegrid<IceSkater>& Lakegrid,unsigned long Id,unsigned int Speed, unsigned int Time, unsigned int x, unsigned int y)
	:id(Id),lakegrid(Lakegrid),start(x,y),current(start),heading(UNKNOWN),hasmoved(false),time(Time),pair(NULL),speed(Speed)
{
	cerr<<"Created IceSkater with id"<<id<<" in position ("<<current.x<<','<<current.y<<")."<<endl;
}

IceSkater::IceSkater(Lakegrid<IceSkater>& Lakegrid,unsigned long Id,unsigned int Speed, unsigned int Time, pos2D pos)
	:id(Id),lakegrid(Lakegrid),start(pos),current(start),heading(UNKNOWN),hasmoved(false),time(Time),pair(NULL),speed(Speed)
{
	cerr<<"Created IceSkater with id "<<id<<" in position ("<<current.x<<","<<current.y<<")."<<endl;
}

unsigned long IceSkater::getid() const
{
	return id;
}

bool IceSkater::hasPair() const
{
	return pair!=NULL;
}

bool IceSkater::has_Moved() const
{
	return hasmoved;
}

pos2D IceSkater::getpos() const
{
	return current;
}

void IceSkater::setpair(IceSkater* apair)
{
	if((hasPair())||apair->hasPair()||apair==NULL)
	{
		cerr<<"Impossible error at IceSkater::setpair. Check your code."<<endl;
		cin.get();
	}
	if((abs((long int)apair->getpos().x-(long int)current.x)==1)==
		(abs((long int)apair->getpos().y-(long int)current.y)==1))
	//If they aren't in adjacent squares
	{
		cerr<<"Impossible error at IceSkater::setpair. Check your code."<<endl;
		cin.get();
	}
	pair=apair;
	pair->pair=this;
	speed=pair->speed=(speed<pair->speed)?(speed):(pair->speed);
	//set speed to the speed of the slowest
	cerr<<"Pairing "<<getid()<<" and "<<pair->getid()<<'.'<<endl;
}

void IceSkater::collision()
{
	long int tempnum;
	int maxtries=__ICESMAXTRIES__;
	unsigned int i;
	bool pairOK=true;
	if(hasPair())
	{
		pairOK=false;
	}
	cout<<getid()<<" ("<<getpos().x<<','<<getpos().y<<')'<<" says:\"I just fell, but nice to meet you!\""<<endl;
	heading=UNKNOWN;
	if(hasmoved)
	//If the collision was triggered by two IceSkaters ending up in the same square
	{
		for(i=1;&(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->get_referance(i))!=this;++i)//Find the object in the list
		{
			
		}
		while(true)
		//Chose a random, adjacent, unoccupied spot that the pair accepts and place the IceSkater there.
		{
			tempnum=rand()%8;
			if(tempnum==0&&(unsigned int)current.x!=0&&(unsigned int)current.y!=0&&
				(lakegrid.grid[(unsigned int)(current.x-1)][(unsigned int)(current.y-1)]->object_ammount()==0||maxtries<0))
			//0=up_left
			{
				if(hasPair())
				{
					pairOK=check_pos_for_pair_OK(*this,
						pos2D((unsigned int)current.x+pair->getstartpos().x-(unsigned int)pair->getstartpos().x-1,
						(unsigned int)current.y+pair->getstartpos().y-(unsigned int)pair->getstartpos().y-1));
				}
				if(pairOK)
				{
					lakegrid.grid[(unsigned int)(current.x-1)][(unsigned int)(current.y-1)]->
						insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					--current.x;
					--current.y;
					break;
				}
			}
			else if(tempnum==1&&(unsigned int)current.y!=0&&
				(lakegrid.grid[(unsigned int)(current.x)][(unsigned int)(current.y-1)]->object_ammount()==0||maxtries<0))
			//1=up
			{
				if(hasPair())
				{
					pairOK=check_pos_for_pair_OK(*this,
						pos2D((unsigned int)current.x+pair->getstartpos().x-(unsigned int)pair->getstartpos().x,
						(unsigned int)current.y+pair->getstartpos().y-(unsigned int)pair->getstartpos().y-1));
				}
				if(pairOK)
				{
					lakegrid.grid[(unsigned int)(current.x)][(unsigned int)(current.y-1)]->
						insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					--current.y;
					break;
				}
			}
			else if(tempnum==2&&(unsigned int)current.y!=0&&((unsigned int)current.x!=lakegrid.hside)&&
				(lakegrid.grid[(unsigned int)(current.x+1)][(unsigned int)(current.y-1)]->object_ammount()==0||maxtries<0))
			//2=up_right
			{
				if(hasPair())
				{
					pairOK=check_pos_for_pair_OK(*this,
						pos2D((unsigned int)current.x+pair->getstartpos().x-(unsigned int)pair->getstartpos().x+1,
						(unsigned int)current.y+pair->getstartpos().y-(unsigned int)pair->getstartpos().y-1));
				}
				lakegrid.grid[(unsigned int)(current.x+1)][(unsigned int)(current.y-1)]->
					insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
				++current.x;
				--current.y;
				break;
			}
			else if(tempnum==3&&(unsigned int)current.x!=0&&
				(lakegrid.grid[(unsigned int)(current.x-1)][(unsigned int)(current.y)]->object_ammount()==0||maxtries<0))
			//3=left
			{
				if(hasPair())
				{
					pairOK=check_pos_for_pair_OK(*this,
						pos2D((unsigned int)current.x+pair->getstartpos().x-(unsigned int)pair->getstartpos().x-1,
						(unsigned int)current.y+pair->getstartpos().y-(unsigned int)pair->getstartpos().y));
				}
				if(pairOK)
				{
					lakegrid.grid[(unsigned int)(current.x-1)][(unsigned int)(current.y)]->
						insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					--current.x;
					break;
				}
			}
			else if(tempnum==4&&((unsigned int)current.x!=lakegrid.hside)&&
				(lakegrid.grid[(unsigned int)(current.x+1)][(unsigned int)(current.y)]->object_ammount()==0||maxtries<0))
			//4=right
			{
				if(hasPair())
				{
					pairOK=check_pos_for_pair_OK(*this,
						pos2D((unsigned int)current.x+pair->getstartpos().x-(unsigned int)pair->getstartpos().x+1,
						(unsigned int)current.y+pair->getstartpos().y-(unsigned int)pair->getstartpos().y));
				}
				if(pairOK)
				{
					lakegrid.grid[(unsigned int)(current.x+1)][(unsigned int)(current.y)]->
						insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					++current.x;
					break;
				}
			}
			else if(tempnum==5&&(unsigned int)current.x!=0&&((unsigned int)current.y!=lakegrid.vside)&&
				(lakegrid.grid[(unsigned int)(current.x-1)][(unsigned int)(current.y+1)]->object_ammount()==0||maxtries<0))
			//5=down_left
			{
				if(hasPair())
				{
					pairOK=check_pos_for_pair_OK(*this,
						pos2D((unsigned int)current.x+pair->getstartpos().x-(unsigned int)pair->getstartpos().x-1,
						(unsigned int)current.y+pair->getstartpos().y-(unsigned int)pair->getstartpos().y+1));
				}
				if(pairOK)
				{
					lakegrid.grid[(unsigned int)(current.x-1)][(unsigned int)(current.y+1)]->
						insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					--current.x;
					++current.y;
					break;
				}
			}
			else if(tempnum==6&&((unsigned int)current.y!=lakegrid.vside)&&
				(lakegrid.grid[(unsigned int)(current.x)][(unsigned int)(current.y+1)]->object_ammount()==0||maxtries<0))
			//6=down
			{
				if(hasPair())
				{
					pairOK=check_pos_for_pair_OK(*this,
						pos2D((unsigned int)current.x+pair->getstartpos().x-(unsigned int)pair->getstartpos().x,
						(unsigned int)current.y+pair->getstartpos().y-(unsigned int)pair->getstartpos().y+1));
				}
				if(pairOK)
				{
					lakegrid.grid[(unsigned int)(current.x)][(unsigned int)(current.y+1)]->
						insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					++current.y;
					break;
				}
			}
			else if(tempnum==7&&((unsigned int)current.x!=lakegrid.hside)&&((unsigned int)current.y!=lakegrid.vside)&&
				(lakegrid.grid[(unsigned int)(current.x+1)][(unsigned int)(current.y+1)]->object_ammount()==0||maxtries<0))
			//7=down_right
			{
				if(hasPair())
				{
					pairOK=check_pos_for_pair_OK(*this,
						pos2D((unsigned int)current.x+pair->getstartpos().x-(unsigned int)pair->getstartpos().x+1,
						(unsigned int)current.y+pair->getstartpos().y-(unsigned int)pair->getstartpos().y+1));
				}
				if(pairOK)
				{
					lakegrid.grid[(unsigned int)(current.x+1)][(unsigned int)(current.y+1)]->
						insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					++current.x;
					++current.y;
					break;
				}
			}
			--maxtries;
		}
	}
	else//has overtaken someone
	{
		if(start.x==end.x&&start.y==end.y)
		{
			cerr<<"Impossible error at IceSkater::collision. Check your code."<<endl;
			cin.get();
		}
		if(start.y==end.y)//Need this check to escape zero.
		{
			do//Find an empty position along the way, that the pair accepts
			{
				tempnum=(rand()%((unsigned int)abs(end.x-start.x)+1))*sgn(end.x-start.x);
				//The offset of a random position along the x route
				if(hasPair())
				{
					pairOK=check_pos_for_pair_OK(*this,
						pos2D((unsigned int)current.x+pair->getstartpos().x-(unsigned int)pair->getstartpos().x+tempnum,
						(unsigned int)current.y+pair->getstartpos().y-(unsigned int)pair->getstartpos().y));
				}
				--maxtries;
			}
			while((lakegrid.grid[(unsigned int)(start.x+tempnum)][(unsigned int)start.y]->object_ammount()!=0&&maxtries>0)||!pairOK);
			for(i=1;&(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->get_referance(i))!=this;++i)//Find the object in the list
			{
				
			}
			lakegrid.grid[(unsigned int)(start.x+tempnum)][(unsigned int)start.y]->
				insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
			current.x=start.x+tempnum;
			current.y=start.y;
			//change position and update current position
		}
		else//All moves are symmetrical. No more checks required.
		{
			do//Find an empty position along the way
			{
				tempnum=(rand()%((unsigned int)abs(end.y-start.y)+1))*sgn(end.y-start.y);
				//The offset of a random position along the y axis
				if(hasPair())
				{
					pairOK=check_pos_for_pair_OK(*this,
						pos2D((unsigned int)current.x+pair->getstartpos().x-(unsigned int)pair->getstartpos().x+abs(tempnum)*sgn(end.x-start.x),
						(unsigned int)current.y+pair->getstartpos().y-(unsigned int)pair->getstartpos().y+tempnum));
				}
				--maxtries;
			}
			while((lakegrid.grid[(unsigned int)(start.x+abs(tempnum)*sgn(end.x-start.x))][(unsigned int)(start.y+tempnum)]->object_ammount()!=0&&maxtries>0)||!pairOK);
			for(i=1;(&lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->get_referance(i))!=this;++i)//Find the object in the list
			{
				
			}
			lakegrid.grid[(unsigned int)(start.x+abs(tempnum)*sgn(end.x-start.x))][(unsigned int)(start.y+tempnum)]->
				insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
			current.x=start.x+abs(tempnum)*sgn(end.x-start.x);
			//Since all moves are symetrical we simply need to add or subtract how much the object has moved along the y axis to the x position.
			//The sign takes care of wether the move is left or right. If end.x==start.x sgn returns 0 so no movement happens
			current.y=start.y+tempnum;
			//change position and update current position
		}
		hasmoved=true;
	}
	for(i=1;&(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->get_referance(i))!=this;++i)//Find the object in the list
	{
		
	}
	if(hasPair())//Pair also falls
	{
		pair->paircollision();
	}
	for(i=1;&(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->get_referance(i))!=this;++i)//Find the object in the list
	{
		
	}
}

void IceSkater::paircollision()
{
	int tempnum;
	int maxtries=__ICESMAXTRIES__;
	unsigned int i;
	for(i=1;&(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->get_referance(i))!=this;++i)//Find the object in the list
	{
		
	}
	lakegrid.grid[(unsigned int)((unsigned int)pair->current.x+current.x-(unsigned int)current.x)][
		(unsigned int)((unsigned int)pair->current.y+current.y-(unsigned int)current.y)]->
		insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
	current.x=(unsigned int)pair->current.x+current.x-(unsigned int)current.x;
	current.y=(unsigned int)pair->current.y+current.y-(unsigned int)current.y;
	//Move the object to the pairs location
	hasmoved=true;
	heading=UNKNOWN;
	switch(propose_direction())
	{
		case EAST:
		case SOUTHEAST:
		
		case WEST:
		case NORTHWEST:
			while(true)
			{
				tempnum=rand()%2;
				if(tempnum==0&&((unsigned int)current.y!=lakegrid.vside)&&
					(lakegrid.grid[(unsigned int)(current.x)][(unsigned int)(current.y+1)]->object_ammount()==0||maxtries<0))
				//0=down
				{
					lakegrid.grid[(unsigned int)(current.x)][(unsigned int)(current.y)]->
						insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					++current.y;
					break;
				}
				else if(tempnum==1&&(unsigned int)current.y!=0&&
					(lakegrid.grid[(unsigned int)(current.x)][(unsigned int)(current.y-1)]->object_ammount()==0||maxtries<0))
				//1=up
				{
					lakegrid.grid[(unsigned int)(current.x)][(unsigned int)(current.y)]->
						insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					--current.y;
					break;
				}
				--maxtries;
			}
		break;
		case SOUTH:
		case SOUTHWEST:
		
		case NORTH:
		case NORTHEAST:
			while(true)
			{
				tempnum=rand()%2;
				if(tempnum==0&&(unsigned int)current.x!=0&&
					(lakegrid.grid[(unsigned int)(current.x-1)][(unsigned int)(current.y)]->object_ammount()==0||maxtries<0))
				//0=left
				{
					lakegrid.grid[(unsigned int)(current.x)][(unsigned int)(current.y)]->
						insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					--current.x;
					break;
				}
				else if(tempnum==1&&((unsigned int)current.x!=lakegrid.hside)&&
					(lakegrid.grid[(unsigned int)(current.x+1)][(unsigned int)(current.y)]->object_ammount()==0||maxtries<0))
				//1=right
				{
					lakegrid.grid[(unsigned int)(current.x)][(unsigned int)(current.y)]->
						insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					++current.x;
					break;
				}
				--maxtries;
			}
		break;
		default://UNKNOWN ie, it's dead center.
			collision();//Let the collision mechanics handle it and give it a random position.
			return;
		break;//break is unnecessary but looks nicer. this also happens in other places
	}
	cout<<id<<" ("<<current.x<<','<<current.y<<')'<<" says:\"I just fell, but nice to meet you!\""<<endl;
	//Output here, just in case collision is called.
}

void IceSkater::move()
{
	unsigned int i;
	const short signx=sgn(end.x-start.x);//If it goes right it's 1; if it goes left -1; does not move in this axis 0;
	const short signy=sgn(end.y-start.y);//If it goes down it's 1; if it goes up -1; does not move int this axis 0;
	if (start.x==end.x&&start.y==end.y)
	{
		cerr<<"Impossible error at IceSkater::move. Check your code."<<endl;
		cin.get();
	}
	if(hasPair())
	{
		//If the pair is going to stop sooner
		if((signx>0&&(unsigned int)pair->getendpos().x<(unsigned int)end.x+(unsigned int)pair->getstartpos().x-(unsigned int)start.x)||
			//If going right and the pairs' endposition is left. This bit "+pair->getstartpos().x-start.x" adds the pairs offset to the endposition.
			(signx<0&&(unsigned int)pair->getendpos().x>(unsigned int)end.x+(unsigned int)pair->getstartpos().x-(unsigned int)start.x))
			//If going left and the pairs' endposition is right
		{
			end.x=(unsigned int)pair->getendpos().x+(unsigned int)start.x-(unsigned int)pair->getstartpos().x+start.x-(unsigned int)start.x;
			//Stop sooner
		}
		//Same thing for y
		if((signy>0&&pair->getendpos().y<end.y+pair->getstartpos().y-start.y)||
			(signy<0&&pair->getendpos().y>end.y+pair->getstartpos().y-start.y))
		{
			end.y=(unsigned int)pair->getendpos().y+(unsigned int)start.y-(unsigned int)pair->getstartpos().y+start.y-(unsigned int)start.y;
		}
	}
	if (start.x==end.x&&start.y==end.y)
	{
		cerr<<"Ice Skater "<<id<<" remained immobile."<<endl;
	}
	if(start.x==end.x)//To escape 0
	{
		for(long int i=start.y;i!=(long int)end.y+signy;i+=signy)
		//Start checking along the way
		{
			for(unsigned int k=1;k<=lakegrid.grid[(unsigned int)start.x][i]->object_ammount();k++)
			//All objects in all squares
			{
				if(lakegrid.grid[(unsigned int)start.x][i]->get_referance(k).heading==heading&&
					signy*lakegrid.grid[(unsigned int)start.x][i]->get_referance(k).end.y<signy*end.y)
				//If the object overtakes another object
				{
					if(lakegrid.grid[(unsigned int)start.x][i]->get_referance(k).isfaller())
					//If the object is going to fall
					{
						lakegrid.grid[(unsigned int)start.x][i]->get_referance(k).sethasmoved(false);
						//We want the overtake function to work, not the same tile one.
					}
					lakegrid.grid[(unsigned int)start.x][i]->get_referance(k).collision();
					collision();
					//fall
					return;
				}
			}
		}
	}
	else
	{
		for(long int i=start.x;i!=(long int)end.x+signx;i+=signx)
		{
			for(unsigned int k=1;k<=lakegrid.grid[i][(unsigned int)(start.y+abs((unsigned int)start.x-i)*signy)]->object_ammount();k++)
			{
				if(lakegrid.grid[i][(unsigned int)(start.y+abs((unsigned int)start.x-i)*signy)]->get_referance(k).heading==heading&&
					signy*lakegrid.grid[i][(unsigned int)(start.y+abs((unsigned int)start.x-i)*signy)]->get_referance(k).end.y<signy*end.y)
				{
					if(lakegrid.grid[i][(unsigned int)(start.y+abs((unsigned int)start.x-i)*signy)]->get_referance(k).isfaller())
					{
						lakegrid.grid[i][(unsigned int)(start.y+abs((unsigned int)start.x-i)*signy)]->get_referance(k).sethasmoved(false);//We want the overtake function to work, not the same tile one.
					}
					lakegrid.grid[i][(unsigned int)(start.y+abs((unsigned int)start.x-i)*signy)]->get_referance(k).collision();
					collision();
					return;
				}
			}
		}
	}
	//After this we conclude that the no obstacles were encountered, so the move is the successful
	for(i=1;&(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->get_referance(i))!=this;++i)//Find the object in the list
	{
		
	}
	lakegrid.grid[(unsigned int)(end.x)][(unsigned int)(end.y)]->
		insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
	current.x=end.x;
	current.y=end.y;
	hasmoved=true;
	if(hasPair()&&!(pair->has_Moved()))
	//If it has a pair that has not move
	{
		pair->move();
		//Move the pair
	}
	if(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->object_ammount()!=1&&lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->get_finish_referance().has_Moved())
	{
		lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->get_finish_referance().collision();
	collision();
	}
}

void IceSkater::prepare_move()
//Πρέπει να μην μένει ακίνητος.
{
	unsigned int i;
	hasmoved=false;
	start=current;
	heading=propose_direction();
	if(hasPair())
	{
		if(heading==UNKNOWN)
		//If it's in the center
		{
			heading=pair->propose_direction();
			//They can't both be at the center. Follow the pair.
		}
		else if(heading!=pair->propose_direction()&&pair->propose_direction()!=UNKNOWN)
		//If the pair doesn't know where to go (pair->propose direction()==UNKNOWN), it will pick this's direction.
		{
			if((pair->propose_direction()==(heading)%8+1||pair->propose_direction()==(heading+1)%8+1||pair->propose_direction()==(heading+6)%8+1)&&
				pair->propose_direction()!=SOUTHEAST&&pair->propose_direction()!=NORTHEAST&&pair->propose_direction()!=SOUTHWEST&&pair->propose_direction()!=NORTHWEST)
			//check if the pair's proposed direction is adjacent to this ones or a 90 degrees right turn. If so then pick this one.
			{
				heading=pair->propose_direction();
			}
			else
			{
				//cerr<<"Warning: Different directions with "<<id<<" and "<<pair->getid()<<endl;
			}
		}
		if(distance2Dint(current,lakegrid.center)<distance2Dint(pair->current,lakegrid.center))//If this is the skater closer to the center
		{
			for(i=1;&(lakegrid.grid[(unsigned int)pair->current.x][(unsigned int)pair->current.y]->get_referance(i))!=pair;++i)//Find the pair in the list
			{
				
			}
			switch(heading)//Make sure the pair has room
			{
				case SOUTHEAST:
					if((unsigned int)current.x==lakegrid.hside-1)
					{
						heading=SOUTH;
					}
				break;
				case NORTHWEST:
					if((unsigned int)current.x==1)
					{
						heading=NORTH;
					}
				break;
				case NORTHEAST:
					if((unsigned int)current.y==1)
					{
						heading=EAST;
					}
				break;
				case SOUTHWEST:
					if((unsigned int)current.y==lakegrid.vside-1)
					{
						heading=WEST;
					}
				break;
				default:
				break;
			}
			switch(heading)//Reposition pair for turn if necessary.
			{
				case EAST:
				case SOUTHEAST:
					if((unsigned int)(pair->current.x)!=(unsigned int)(current.x)||(unsigned int)(pair->current.y)!=(unsigned int)(current.y-1))
					//If this one is going east and the pair is not in the tile above
					{
						if((unsigned int)(current.y)!=0)
						{
							lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y-1]->
								insert_start(lakegrid.grid[(unsigned int)pair->getpos().x][(unsigned int)pair->getpos().y]->pop_pointer(i));
							pair->current.x=(unsigned int)current.x+pair->getpos().x-(unsigned int)pair->getpos().x;
							pair->current.y=(unsigned int)current.y+pair->getpos().y-(unsigned int)pair->getpos().y-1;
							//This "+pair->getpos().y-(unsigned int)pair->getpos().y" gives the pairs' position the correct decimal value;
						}
						else
						{
							cerr<<"WARNING:Substituting "<<id<<" and "<<pair->id<<" ! Check your code!"<<endl;
							cin.get();
							lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y+1]->
								insert_start(lakegrid.grid[(unsigned int)pair->getpos().x][(unsigned int)pair->getpos().y]->pop_pointer(i));
							pair->current.x=(unsigned int)current.x+pair->getpos().x-(unsigned int)pair->getpos().x;
							pair->current.y=(unsigned int)current.y+pair->getpos().y-(unsigned int)pair->getpos().y+1;
						}
						pair->prepare_move();//Since the pair has been altered, it's moves must be replotted
						prepare_move();
						return;
					}
				break;
				case SOUTH:
				case SOUTHWEST:
					if((unsigned int)(pair->current.x)!=(unsigned int)(current.x+1)||(unsigned int)(pair->current.y)!=(unsigned int)(current.y))
					{
						if((unsigned int)(current.x)!=lakegrid.hside)
						{
							lakegrid.grid[(unsigned int)current.x+1][(unsigned int)current.y]->
								insert_start(lakegrid.grid[(unsigned int)pair->getpos().x][(unsigned int)pair->getpos().y]->pop_pointer(i));
							pair->current.x=(unsigned int)current.x+pair->getpos().x-(unsigned int)pair->getpos().x+1;
							pair->current.y=(unsigned int)current.y+pair->getpos().y-(unsigned int)pair->getpos().y;
						}
						else
						{
							cerr<<"WARNING:Substituting "<<id<<" and "<<pair->id<<" ! Check your code!"<<endl;
							cin.get();
							lakegrid.grid[(unsigned int)current.x-1][(unsigned int)current.y]->
								insert_start(lakegrid.grid[(unsigned int)pair->getpos().x][(unsigned int)pair->getpos().y]->pop_pointer(i));
							pair->current.x=(unsigned int)current.x+pair->getpos().x-(unsigned int)pair->getpos().x-1;
							pair->current.y=(unsigned int)current.y+pair->getpos().y-(unsigned int)pair->getpos().y;
						}
						pair->prepare_move();//Since the pair has been altered, it's moves must be replotted
						prepare_move();
						return;
					}
				break;
				case WEST:
				case NORTHWEST:
					if((unsigned int)(pair->current.x)!=(unsigned int)(current.x)||(unsigned int)(pair->current.y)!=(unsigned int)(current.y+1))
					{
						if((unsigned int)(current.y)!=lakegrid.vside)
						{
							lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y+1]->
								insert_start(lakegrid.grid[(unsigned int)pair->getpos().x][(unsigned int)pair->getpos().y]->pop_pointer(i));
							pair->current.x=(unsigned int)current.x+pair->getpos().x-(unsigned int)pair->getpos().x;
							pair->current.y=(unsigned int)current.y+pair->getpos().y-(unsigned int)pair->getpos().y+1;
						}
						else
						{
							cerr<<"WARNING:Substituting "<<id<<" and "<<pair->id<<" ! Check your code!"<<endl;
							cin.get();
							lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y-1]->
								insert_start(lakegrid.grid[(unsigned int)pair->getpos().x][(unsigned int)pair->getpos().y]->pop_pointer(i));
							pair->current.x=(unsigned int)current.x+pair->getpos().x-(unsigned int)pair->getpos().x;
							pair->current.y=(unsigned int)current.y+pair->getpos().y-(unsigned int)pair->getpos().y-1;
						}
						pair->prepare_move();//Since the pair has been altered, it's moves must be replotted
						prepare_move();
						return;
					}
				break;
				case NORTH:
				case NORTHEAST:
					if((unsigned int)(pair->current.x)!=(unsigned int)(current.x-1)||(unsigned int)(pair->current.y)!=(unsigned int)(current.y))
					{
						if((unsigned int)(current.x)!=0)
						{
							lakegrid.grid[(unsigned int)current.x-1][(unsigned int)current.y]->
								insert_start(lakegrid.grid[(unsigned int)pair->getpos().x][(unsigned int)pair->getpos().y]->pop_pointer(i));
							pair->current.x=(unsigned int)current.x+pair->getpos().x-(unsigned int)pair->getpos().x-1;
							pair->current.y=(unsigned int)current.y+pair->getpos().y-(unsigned int)pair->getpos().y;
						}
						else
						{
							cerr<<"WARNING:Substituting "<<id<<" and "<<pair->id<<" ! Check your code!"<<endl;
							cin.get();
							lakegrid.grid[(unsigned int)current.x+1][(unsigned int)current.y]->
								insert_start(lakegrid.grid[(unsigned int)pair->getpos().x][(unsigned int)pair->getpos().y]->pop_pointer(i));
							pair->current.x=(unsigned int)current.x+pair->getpos().x-(unsigned int)pair->getpos().x+1;
							pair->current.y=(unsigned int)current.y+pair->getpos().y-(unsigned int)pair->getpos().y;
						}
						pair->prepare_move();//Since the pair has been altered, it's moves must be replotted
						prepare_move();
						return;
					}
				break;
				default:
					cerr<<"ERROR: Impossible error at IceSkater::prepare_move! Check your code!"<<endl;
					cin.get();
				break;
			}
		}
	}
	else//If it is unpaired
	{
		if(heading==UNKNOWN)//In the improbable case that it is dead center
		{
			heading=static_cast<direction>(rand()%8+1);//Choose a random heading. 
			//(There are 8 possible headings, the first being 1=North and the last 8=Northwest)
			//This is always going to be an acceptable conversion, so no need to make extra conversion functions
		}
	}
	//Set endpoint
	switch(heading)
	{
		case EAST:
			end.y=start.y;
			if(start.x+speed<=lakegrid.hside)
			//If this skater is going to end up inside the lake.
			{
				if(hasPair()&&start.x+speed==lakegrid.hside&&
					distance2Dint(current,lakegrid.center)<distance2Dint(pair->current,lakegrid.center))
				//If this skater is going to end up at the edge, has a pair and is on the inner circle (ie closer to the center than the pair).
				{
					end.x=start.x+speed-1;
					//Leave some room for the pair to turn.
				}
				else
				{
					end.x=start.x+speed;
				}
			}
			else
			//If this skater is going to end up outside the lake
			{
				if(hasPair()&&distance2Dint(current,lakegrid.center)<distance2Dint(pair->current,lakegrid.center))
				//If this skater has a pair and is on the inner circle (ie closer to the center than the pair)
				{
					end.x=lakegrid.hside-1;
					//Leave some room for the pair.
				}
				else
				{
					end.x=lakegrid.hside;
				}
			}
		break;
		case SOUTHEAST:
			if(start.x+speed<=lakegrid.hside&&start.y+speed<=lakegrid.vside)
			//If this skater is going to end up inside the lake.
			{
				if(hasPair()&&((unsigned int)(start.x+speed)==lakegrid.hside||(unsigned int)(start.y+speed)==lakegrid.vside)&&
					distance2Dint(current,lakegrid.center)<distance2Dint(pair->current,lakegrid.center))
				//If this skater is going to end up at the edge, has a pair and is on the inner circle (ie closer to the center than the pair).
				{
					end.x=start.x+speed-1;
					end.y=start.y+speed-1;
					//Leave some room for the pair to turn.
				}
				else
				{
					end.x=start.x+speed;
					end.y=start.y+speed;
				}
			}
			else
			//If this skater is going to end up outside the lake
			{
				if(lakegrid.hside-start.x<lakegrid.vside-start.y)
				{
					if(hasPair()&&distance2Dint(current,lakegrid.center)<distance2Dint(pair->current,lakegrid.center))
					//If this skater has a pair and is on the inner circle (ie closer to the center than the pair)
					{
						end.x=lakegrid.hside-1;
						//Leave some room for the pair.
					}
					else
					{
						end.x=lakegrid.hside;
					}
					end.y=start.y+end.x-start.x;
				}
				else
				{
					if(hasPair()&&distance2Dint(current,lakegrid.center)<distance2Dint(pair->current,lakegrid.center))
					//If this skater has a pair and is on the inner circle (ie closer to the center than the pair)
					{
						end.y=lakegrid.vside-1;
						//Leave some room for the pair.
					}
					else
					{
						end.y=lakegrid.vside;
					}
					end.x=start.x+end.y-start.y;
				}
			}
		break;
		case WEST:
			end.y=start.y;
			if(start.x-speed>=0)
			//If this skater is going to end up inside the lake.
			{
				if(hasPair()&&(unsigned int)(start.x-speed)==0&&distance2Dint(current,lakegrid.center)<distance2Dint(pair->current,lakegrid.center))
				//If this skater is going to end up at the edge, has a pair and is on the inner circle (ie closer to the center than the pair).
				{
					end.x=start.x-speed+1;
					//Leave some room for the pair to turn.
				}
				else
				{
					end.x=start.x-speed;
				}
			}
			else
			//If this skater is going to end up outside the lake
			{
				if(hasPair()&&distance2Dint(current,lakegrid.center)<distance2Dint(pair->current,lakegrid.center))
				//If this skater has a pair and is on the inner circle (ie closer to the center than the pair)
				{
					end.x=1;
					//Leave some room for the pair.
				}
				else
				{
					end.x=0;
				}
			}
		break;
		case NORTHWEST:
			if(start.x-speed>=0&&start.y-speed>=0)
			//If this skater is going to end up inside the lake.
			{
				if(hasPair()&&((unsigned int)(start.y-speed)==0||(unsigned int)(start.x-speed)==0)&&
					distance2Dint(current,lakegrid.center)<distance2Dint(pair->current,lakegrid.center))
				//If this skater is going to end up at the edge, has a pair and is on the inner circle (ie closer to the center than the pair).
				{
					end.x=start.x-speed+1;
					end.y=start.y-speed+1;
					//Leave some room for the pair to turn.
				}
				else
				{
					end.x=start.x-speed;
					end.y=start.y-speed;
				}
			}
			else
			//If this skater is going to end up outside the lake
			{
				if(start.x<start.y)
				{
					if(hasPair()&&distance2Dint(current,lakegrid.center)<distance2Dint(pair->current,lakegrid.center))
					//If this skater has a pair and is on the inner circle (ie closer to the center than the pair)
					{
						end.x=1;
						//Leave some room for the pair.
					}
					else
					{
						end.x=0;
					}
					end.y=start.y-(start.x-end.x);
				}
				else
				{
					if(hasPair()&&distance2Dint(current,lakegrid.center)<distance2Dint(pair->current,lakegrid.center))
					//If this skater has a pair and is on the inner circle (ie closer to the center than the pair)
					{
						end.y=1;
						//Leave some room for the pair.
					}
					else
					{
						end.y=0;
					}
					end.x=start.x-start.y+end.y;
				}
			}
		break;
		case SOUTH:
			end.x=start.x;
			if(start.y+speed<=lakegrid.vside)
			//If the skater is going to end up inside the lake
			{
				if((unsigned int)(start.y+speed)==lakegrid.vside&&hasPair()&&
					distance2Dint(current,lakegrid.center)<distance2Dint(pair->current,lakegrid.center))
				//If this skater is going to end up at the edge, has a pair and is on the inner circle (ie closer to the center than the pair).
				{
					end.y=start.y+speed-1;
				}
				else
				{
					end.y=start.y+speed;
				}
			}
			else
			//If the skater is going to end up outside the lake.
			{
				if(hasPair()&&distance2Dint(current,lakegrid.center)<distance2Dint(pair->current,lakegrid.center))
				//If this skater has a pair and is on the inner circle (ie closer to the center than the pair)
				{
					end.y=lakegrid.vside-1;
				}
				else
				{
					end.y=lakegrid.vside;
				}
			}
		break;
		case SOUTHWEST:
			if(start.y+speed<=lakegrid.vside&&(start.x-speed)>=0)
			//If the skater is going to end up inside the lake
			{
				if(hasPair()&&((unsigned int)(start.y+speed)==lakegrid.vside||(unsigned int)(start.x-speed)==0)&&distance2Dint(current,lakegrid.center)<distance2Dint(pair->current,lakegrid.center))
				//If this skater is going to end up at the edge, has a pair and is on the inner circle (ie closer to the center than the pair).
				{
					end.y=start.y+speed-1;
					end.x=start.x-speed+1;
				}
				else
				{
					end.y=start.y+speed;
					end.x=start.x-speed;
				}
			}
			else
			//If the skater is going to end up outside the lake.
			{
				if(start.x<lakegrid.vside-start.y)
				//If the skater is going to cross the west border more than it's going to cross the south border
				{
					if(hasPair()&&distance2Dint(current,lakegrid.center)<distance2Dint(pair->current,lakegrid.center))
					//If this skater has a pair and is on the inner circle (ie closer to the center than the pair)
					{
						end.x=1;
					}
					else
					{
						end.x=0;
					}
					end.y=start.y+start.x-end.x;
				}
				else
				{
					if(hasPair()&&distance2Dint(current,lakegrid.center)<distance2Dint(pair->current,lakegrid.center))
					//If this skater has a pair and is on the inner circle (ie closer to the center than the pair)
					{
						end.y=lakegrid.vside-1;
					}
					else
					{
						end.y=lakegrid.vside;
					}
					end.x=start.x-end.y+start.y;
				}
			}
		break;
		case NORTH:
			end.x=start.x;
			if((start.y-speed)>=0)
			//If the skater is going to end up inside the lake
			{
				if(hasPair()&&(unsigned int)(start.y-speed)==0&&
					distance2Dint(current,lakegrid.center)<distance2Dint(pair->current,lakegrid.center))
				//If this skater is going to end up at the edge, has a pair and is on the inner circle (ie closer to the center than the pair).
				{
					end.y=start.y-speed+1;
				}
				else
				{
					end.y=start.y-speed;
				}
			}
			else
			//If the skater is going to end up outside the lake.
			{
				if(hasPair()&&distance2Dint(current,lakegrid.center)<distance2Dint(pair->current,lakegrid.center))
				//If this skater has a pair and is on the inner circle (ie closer to the center than the pair)
				{
					end.y=1;
				}
				else
				{
					end.y=0;
				}
			}
		break;
		case NORTHEAST:
			if((start.y-speed)>=0&&(start.x+speed)<=lakegrid.hside)
			//If the skater is going to end up inside the lake
			{
				if(hasPair()&&((unsigned int)(start.y-speed)==0||(unsigned int)(start.x+speed)==lakegrid.hside)&&
					distance2Dint(current,lakegrid.center)<distance2Dint(pair->current,lakegrid.center))
				//If this skater is going to end up at the edge, has a pair and is on the inner circle (ie closer to the center than the pair).
				{
					end.y=start.y-speed+1;
					end.x=start.x+speed-1;
				}
				else
				{
					end.y=start.y-speed;
					end.x=start.x+speed;
				}
			}
			else
			//If the skater is going to end up outside the lake.
			{
				if(lakegrid.hside-start.x<start.y)
				{
					if(hasPair()&&distance2Dint(current,lakegrid.center)<distance2Dint(pair->current,lakegrid.center))
					//If this skater has a pair and is on the inner circle (ie closer to the center than the pair)
					{
						end.x=lakegrid.hside-1;
					}
					else
					{
						end.x=lakegrid.hside;
					}
					end.y=start.y-end.x+start.x;
				}
				else
				{
					if(hasPair()&&distance2Dint(current,lakegrid.center)<distance2Dint(pair->current,lakegrid.center))
					//If this skater has a pair and is on the inner circle (ie closer to the center than the pair)
					{
						end.y=1;
					}
					else
					{
						end.y=0;
					}
					end.x=start.x+(start.y-end.y);
				}
			}
		break;
		default:
			cerr<<"ERROR: Impossible error at IceSkater::prepare_move! Check your code!"<<endl;
			cin.get();
		break;
	}
	if(start==end)
	{
		cerr<<"Warning: IceSkater "<<getid()<<" will remain immobile."<<endl;
	}
}

direction IceSkater::propose_direction() const
{
	if(current.x>lakegrid.center.x)
	{
		if(current.y>lakegrid.center.y)
		{
			if(current.y<lakegrid.vside)
			{
				return SOUTHWEST;
			}
			else
			{
				return WEST;
			}
		}
		else if(current.y<lakegrid.center.y)
		{
			if(current.x<lakegrid.hside)
			{
				return SOUTHEAST;
			}
			else
			{
				return SOUTH;
			}
		}
		else//equal
		{
			return SOUTH;
		}
	}
	else if(current.x<lakegrid.center.x)
	{
		if(current.y>lakegrid.center.y)
		{
			if(current.x>0)
			{
				return NORTHWEST;
			}
			else
			{
				return NORTH;
			}
		}
		else if(current.y<lakegrid.center.y)
		{
			if(current.y>0)
			{
				return NORTHEAST;
			}
			else
			{
				return EAST;
			}
		}
		else//equal
		{
			return NORTH;
		}
	}
	else//equal
	{
		if(current.y>lakegrid.center.y)
		{
			return WEST;
		}
		else if(current.y<lakegrid.center.y)
		{
			return EAST;
		}
		else//equal, ie dead center
		{
			return UNKNOWN;
		}
	}
}

void IceSkater::end_move()
{
	int i;
	if((--time)==0)
	{
		for(i=1;&(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->get_referance(i))!=this;++i)//Find this one in the list
		{
			
		}
		goodbye_msg();
		if(hasPair())
		{
			pair->goodbye_msg();
		}
		delete lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i);
	}
}

void IceSkater::goodbye_msg() const
{
	cout<<id<<" ("<<current.x<<','<<current.y<<')'<<" says:\"Bye Bye...\""<<endl;
}

bool IceSkater::posOK(pos2D pos) const
{
	if(pos.x>lakegrid.hside||pos.x<0||pos.y>lakegrid.vside||pos.y<0)
	{
		return false;
	}
	return true;
}

inline void IceSkater::sethasmoved(bool to)
{
	hasmoved=to;
}

inline void IceSkater::unpair()
{
	if(hasPair())
	{
		pair->pair=NULL;
		pair=NULL;
	}
}

inline direction IceSkater::getheading() const
{
	return heading;
}

inline pos2D IceSkater::getendpos() const
{
	return end;
}

inline pos2D IceSkater::getstartpos() const
{
	return start;
}

inline IceStype IceSkater::givetype() const
{
	return ICESKATER;
}

inline bool IceSkater::isfaller() const
{
	return true;
}

IceSkater::~IceSkater()
{
	int i;
	if (hasPair())
	{
		for(i=1;&(lakegrid.grid[(unsigned int)pair->current.x][(unsigned int)pair->current.y]->get_referance(i))!=pair;++i)//Find the pair in the list
		{
			
		}
		pair->pair=NULL;//We don't want an infinite loop, now do we?
		delete lakegrid.grid[(unsigned int)pair->current.x][(unsigned int)pair->current.y]->pop_pointer(i);
		pair=NULL;
	}
}

  /////////////////////////////////
 //			IceSnormal			//
/////////////////////////////////

IceSnormal::IceSnormal(Lakegrid<IceSkater>& Lakegrid,unsigned long Id,unsigned int Speed, unsigned int Time, unsigned int x, unsigned int y, unsigned short MaxDratio):IceSkater(Lakegrid,Id,Speed,Time,x,y),maxDratio(((double)(MaxDratio))/100)
{
	if(MaxDratio>30||!posOK(current))
	{
		cerr<<"ERROR: Impossible error at IceSnormal constructor! Check your code!"<<endl;
		cin.get();
	}
}

IceSnormal::IceSnormal(Lakegrid<IceSkater>& Lakegrid,unsigned long Id,unsigned int Speed, unsigned int Time, pos2D pos, unsigned short MaxDratio):IceSkater(Lakegrid,Id,Speed,Time,pos),maxDratio(double(MaxDratio)/100)
{
	if(MaxDratio>30||!posOK(current))
	{
		cerr<<"ERROR: Impossible error at IceSnormal constructor! Check your code!"<<endl;
		cin.get();
	}
}

direction IceSnormal::propose_direction() const
{
	if(current.x>lakegrid.center.x)//is east
	{
		if(current.y>lakegrid.center.y)//is south
		{
			if(current.y>=lakegrid.vside-maxDratio*lakegrid.vside)
			//Is closer to the side then 30% of the parallels length
			{
				return WEST;
			}
			else
			{
				return SOUTH;
			}
		}
		else if(current.y<lakegrid.center.y)
		{
			if(current.x>=lakegrid.hside-maxDratio*lakegrid.hside)
			{
				return SOUTH;
			}
			else
			{
				return EAST;
			}
		}
		else//equal
		{
			return SOUTH;
		}
	}
	else if(current.x<lakegrid.center.x)
	{
		if(current.y>lakegrid.center.y)
		{
			if(current.x<=lakegrid.hside*maxDratio)
			{
				return NORTH;
			}
			else
			{
				return WEST;
			}
		}
		else if(current.y<lakegrid.center.y)
		{
			if(current.y<=lakegrid.vside*maxDratio)
			{
				return EAST;
			}
			else
			{
				return NORTH;
			}
		}
		else//equal
		{
			return NORTH;
		}
	}
	else//equal
	{
		if(current.y>lakegrid.center.y)
		{
			return WEST;
		}
		else if(current.y<lakegrid.center.y)
		{
			return EAST;
		}
		else//equal, ie dead center
		{
			return UNKNOWN;
		}
	}
}

bool IceSnormal::posOK(pos2D pos) const
{
	double temp1;
	double temp2;
	double temp3;
	double temp4;
	if(pos.x>lakegrid.hside||pos.x<0||pos.y>lakegrid.vside||pos.y<0)//Is within bounds
	{
		return false;
	}
	if(pos.x!=(temp1=lakegrid.hside*maxDratio)&&pos.x!=(temp2=lakegrid.hside-lakegrid.hside*maxDratio)&&
		pos.y!=(temp3=lakegrid.vside*maxDratio)&&pos.y!=(temp4=lakegrid.vside-lakegrid.vside*maxDratio))
	//Is within the maxDratio ring
	{
		return false;
	}
	return true;
}

void IceSnormal::prepare_move()
{
	unsigned int i;
	hasmoved=false;
	start=current;
	heading=propose_direction();
	if(hasPair())
	{
		if(distance2Dint(current,lakegrid.center)<distance2Dint(pair->getpos(),lakegrid.center))//If this is the skater closer to the center
		{
			for(i=1;&(lakegrid.grid[(unsigned int)pair->getpos().x][(unsigned int)pair->getpos().y]->get_referance(i))!=pair;++i)//Find the pair in the list
			{
				
			}
			switch(heading)//Reposition pair for turn if necessary.
			{
				case EAST:
					if((unsigned int)(pair->getpos().x)!=(unsigned int)(getpos().x)||
						(unsigned int)(pair->getpos().y)!=(unsigned int)(getpos().y-1))
					{
						if((unsigned int)(getpos().y)!=0)
						{
							pair->force_move(pos2D((unsigned int)getpos().x+pair->getpos().x-(unsigned int)pair->getpos().x,
								(unsigned int)getpos().y+pair->getpos().y-(unsigned int)pair->getpos().y-1));
						}
						else//Should never happen
						{
							cerr<<"WARNING:Substituting "<<getid()<<" and "<<pair->getid()<<" ! Check your code!"<<endl;
							cin.get();
							pair->force_move(pos2D((unsigned int)getpos().x+pair->getpos().x-(unsigned int)pair->getpos().x,
								(unsigned int)getpos().y+pair->getpos().y-(unsigned int)pair->getpos().y+1));
						}
						pair->prepare_move();//Since the pair has been altered, it's moves must be replotted
						prepare_move();
						return;
					}
				break;
				case SOUTH:
					if((unsigned int)(pair->getpos().x)!=(unsigned int)(getpos().x+1)||
						(unsigned int)(pair->getpos().y)!=(unsigned int)(getpos().y))
					{
						if((unsigned int)(getpos().x)!=lakegrid.hside)
						{
							pair->force_move(pos2D((unsigned int)getpos().x+pair->getpos().x-(unsigned int)pair->getpos().x+1,
								(unsigned int)getpos().y+pair->getpos().y-(unsigned int)pair->getpos().y));
						}
						else
						{
							cerr<<"WARNING:Substituting "<<getid()<<" and "<<pair->getid()<<" ! Check your code!"<<endl;
							cin.get();
							pair->force_move(pos2D((unsigned int)getpos().x+pair->getpos().x-(unsigned int)pair->getpos().x-1,
								(unsigned int)getpos().y+pair->getpos().y-(unsigned int)pair->getpos().y));
						}
						pair->prepare_move();//Since the pair has been altered, it's moves must be replotted
						prepare_move();
						return;
					}
				break;
				case WEST:
					if((unsigned int)(pair->getpos().x)!=(unsigned int)(getpos().x)||(unsigned int)(pair->getpos().y)!=(unsigned int)(getpos().y+1))
					{
						if((unsigned int)(getpos().y)!=lakegrid.vside)
						{
							pair->force_move(pos2D((unsigned int)getpos().x+pair->getpos().x-(unsigned int)pair->getpos().x,
								(unsigned int)getpos().y+pair->getpos().y-(unsigned int)pair->getpos().y+1));
						}
						else
						{
							cerr<<"WARNING:Substituting "<<getid()<<" and "<<pair->getid()<<" ! Check your code!"<<endl;
							cin.get();
							pair->force_move(pos2D((unsigned int)getpos().x+pair->getpos().x-(unsigned int)pair->getpos().x,
								(unsigned int)getpos().y+pair->getpos().y-(unsigned int)pair->getpos().y-1));
						}
						pair->prepare_move();//Since the pair has been altered, it's moves must be replotted
						prepare_move();
						return;
					}
				break;
				case NORTH:
					if((unsigned int)(pair->getpos().x)!=(unsigned int)(getpos().x-1)||(unsigned int)(pair->getpos().y)!=(unsigned int)(getpos().y))
					{
						if((unsigned int)(getpos().x)!=0)
						{
							pair->force_move(pos2D((unsigned int)getpos().x+pair->getpos().x-(unsigned int)pair->getpos().x-1,
								(unsigned int)getpos().y+pair->getpos().y-(unsigned int)pair->getpos().y));
						}
						else
						{
							cerr<<"WARNING:Substituting "<<getid()<<" and "<<pair->getid()<<" ! Check your code!"<<endl;
							cin.get();
							pair->force_move(pos2D((unsigned int)getpos().x+pair->getpos().x-(unsigned int)pair->getpos().x+1,
								(unsigned int)getpos().y+pair->getpos().y-(unsigned int)pair->getpos().y));
						}
						pair->prepare_move();//Since the pair has been altered, it's moves must be replotted
						prepare_move();
						return;
					}
				break;
				default:
					cerr<<"ERROR: Impossible error at IceSnormal::prepare_move! Check your code!"<<endl;
					cin.get();
				break;
			}
		}
	}
	//Set endpoint.
	switch(heading)
	{
		case EAST:
			end.y=start.y;
			if(start.x+speed>lakegrid.hside-lakegrid.hside*maxDratio)
			//If the skater is not going to stay in his "ring".
			{
				end.x=lakegrid.hside-lakegrid.hside*maxDratio;
				//Force him
			}
			else
			{
				end.x=start.x+speed;
				//Move normally;
			}
		break;
		case WEST:
			end.y=start.y;
			if(start.x-speed<lakegrid.hside*maxDratio)
			{
				end.x=lakegrid.hside*maxDratio;
			}
			else
			{
				end.x=start.x-speed;
			}
		break;
		case SOUTH:
			end.x=start.x;
			if(start.y+speed>lakegrid.vside-lakegrid.vside*maxDratio)
			{
				end.y=lakegrid.vside-lakegrid.vside*maxDratio;
			}
			else
			{
				end.y=start.y+speed;
			}
		break;
		case NORTH:
			end.x=start.x;
			if(start.y-speed<lakegrid.vside*maxDratio)
			{
				end.y=lakegrid.vside*maxDratio;
			}
			else
			{
				end.y=start.y-speed;
			}
		break;
		default:
			cerr<<"ERROR: Impossible error at IceSkater::prepare_move! Check your code!"<<endl;
			cin.get();
		break;
	}
}

void IceSnormal::collision()
{
	long int tempnum;
	unsigned int i;
	int maxtries=__ICESMAXTRIES__;
	bool pairOK=true;
	if(hasPair())
	{
		pairOK=false;
	}
	cout<<getid()<<" ("<<getpos().x<<','<<getpos().y<<')'<<" says:\"I just fell, but nice to meet you!\""<<endl;
	heading=UNKNOWN;
	if(hasmoved)
	//If the collision was triggered by two IceSkaters ending up in the same square
	{
		for(i=1;&(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->get_referance(i))!=this;++i)//Find this one in the list
		{
			
		}
		while(true)//(Potential infinite loop? Check again later.)
		//Chose a random, adjacent, unoccupied spot and place the IceSkater there.
		{
			tempnum=rand()%4;
			if(tempnum==0&&posOK(pos2D(current.x,current.y-1))&&
				(lakegrid.grid[(unsigned int)(current.x)][(unsigned int)(current.y-1)]->object_ammount()==0||maxtries<0))
			//1=up
			{
				if(hasPair())
				{
					pairOK=check_pos_for_pair_OK(*this,
						pos2D((unsigned int)current.x+pair->getstartpos().x-(unsigned int)pair->getstartpos().x,
						(unsigned int)current.y+pair->getstartpos().y-(unsigned int)pair->getstartpos().y-1));
				}
				if(pairOK)
				{
					lakegrid.grid[(unsigned int)(current.x)][(unsigned int)(current.y-1)]->
						insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					--current.y;
					break;
				}
			}
			else if(tempnum==1&&posOK(pos2D(current.x-1,current.y))&&
				(lakegrid.grid[(unsigned int)(current.x-1)][(unsigned int)(current.y)]->object_ammount()==0||maxtries<0))
			//1=left
			{
				if(hasPair())
				{
					pairOK=check_pos_for_pair_OK(*this,
						pos2D((unsigned int)current.x+pair->getstartpos().x-(unsigned int)pair->getstartpos().x-1,
						(unsigned int)current.y+pair->getstartpos().y-(unsigned int)pair->getstartpos().y));
				}
				if(pairOK)
				{
					lakegrid.grid[(unsigned int)(current.x-1)][(unsigned int)(current.y)]->
						insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					--current.x;
					break;
				}
			}
			else if(tempnum==2&&posOK(pos2D(current.x+1,current.y))&&
				(lakegrid.grid[(unsigned int)(current.x+1)][(unsigned int)(current.y)]->object_ammount()==0||maxtries<0))
			//2=right
			{
				if(hasPair())
				{
					pairOK=check_pos_for_pair_OK(*this,
						pos2D((unsigned int)current.x+pair->getstartpos().x-(unsigned int)pair->getstartpos().x+1,
						(unsigned int)current.y+pair->getstartpos().y-(unsigned int)pair->getstartpos().y));
				}
				if(pairOK)
				{
					lakegrid.grid[(unsigned int)(current.x+1)][(unsigned int)(current.y)]->
						insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					++current.x;
					break;
				}
			}
			else if(tempnum==3&&posOK(pos2D(current.x,current.y+1))&&
				(lakegrid.grid[(unsigned int)(current.x)][(unsigned int)(current.y+1)]->object_ammount()==0||maxtries<0))
			//3=down
			{
				if(hasPair())
				{
					pairOK=check_pos_for_pair_OK(*this,
						pos2D((unsigned int)current.x+pair->getstartpos().x-(unsigned int)pair->getstartpos().x,
						(unsigned int)current.y+pair->getstartpos().y-(unsigned int)pair->getstartpos().y+1));
				}
				if(pairOK)
				{
					lakegrid.grid[(unsigned int)(current.x)][(unsigned int)(current.y+1)]->
						insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					++current.y;
					break;
				}
			}
			--maxtries;
		}
	}
	else//has overtaken someone
	{
		if(start.x==end.x&&start.y==end.y)
		{
			cerr<<"Impossible error at IceSkater::collision. Check your code."<<endl;
			cin.get();
		}
		if(start.y==end.y)//Need this check to escape zero.
		{
			do//Find an empty position along the way, that the pair accepts
			{
				tempnum=(rand()%((unsigned int)abs(end.x-start.x)+1))*sgn(end.x-start.x);
				//Chooses a random empty position. The "+1" is because rand % something = {0...something-1} and we 
				//want {1...something}. The "sgn(end.x-start.x)" is positive for right and negative for left.
				if(hasPair())
				{
					pairOK=check_pos_for_pair_OK(*this,
						pos2D((unsigned int)getstartpos().x+pair->getpos().x-(unsigned int)pair->getpos().x+tempnum,
						(unsigned int)getstartpos().y+pair->getpos().y-(unsigned int)pair->getpos().y));
				}
				--maxtries;
			}
			while((lakegrid.grid[(unsigned int)(start.x+tempnum)][(unsigned int)start.y]->object_ammount()!=0&&maxtries>0)||!pairOK);
			for(i=1;&(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->get_referance(i))!=this;++i)//Find this one in the list
			{
				
			}
			lakegrid.grid[(unsigned int)(start.x+tempnum)][(unsigned int)(start.y)]->insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
			current.x=(start.x+tempnum);
			current.y=start.y;
			//change position and update current position
		}
		else//All moves are symmetrical. No more checks required.
		{
			do//Find an empty position along the way
			{
				tempnum=(rand()%((unsigned int)abs(end.y-start.y)+1))*sgn(end.y-start.y);
				if(hasPair())
				{
					pairOK=check_pos_for_pair_OK(*this,
						pos2D((unsigned int)start.x+pair->getpos().x-(unsigned int)pair->getpos().x+abs(tempnum)*sgn(end.x-start.x),
						(unsigned int)start.y+pair->getpos().y-(unsigned int)pair->getpos().y+tempnum));
				}
				--maxtries;
			}
			while((lakegrid.grid[(unsigned int)(start.x+abs(tempnum)*sgn(end.x-start.x))][(unsigned int)(start.y+tempnum)]->object_ammount()!=0&&maxtries>0)||!pairOK);
			for(i=1;(&lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->get_referance(i))!=this;++i)//Find this one in the list
			{
				
			}
			lakegrid.grid[(unsigned int)(start.x+abs(tempnum)*sgn(end.x-start.x))][(unsigned int)(start.y+tempnum)]->
				insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
			current.x=(start.x+abs(tempnum)*sgn(end.x-start.x));
			current.y=(start.y+tempnum);
			//change position and update current position
		}
		hasmoved=true;
	}
	if(hasPair())//Pair also falls
	{
		pair->paircollision();
	}
}

void IceSnormal::paircollision()
{
	int tempnum;
	int maxtries=__ICESMAXTRIES__;
	unsigned int i;
	for(i=1;&(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->get_referance(i))!=this;++i)//Find the object in the list
	{
		
	}
	lakegrid.grid[(unsigned int)pair->getpos().x][(unsigned int)pair->getpos().y]->insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
	current.x=(unsigned int)pair->getpos().x+current.x-(unsigned int)current.x;
	current.y=(unsigned int)pair->getpos().y+current.y-(unsigned int)current.y;
	//Move the object to the pairs location
	hasmoved=true;
	heading=UNKNOWN;
	/*switch(propose_direction())
	{
		case EAST:
		case SOUTHEAST:
		
		case WEST:
		case NORTHWEST:*/
			while(true)
			{
				tempnum=rand()%4;
				if(tempnum==0&&posOK(pos2D(current.x,current.y+1))&&(lakegrid.grid[(unsigned int)(current.x)][(unsigned int)(current.y+1)]->object_ammount()==0||maxtries<0))//0=down
				{
					lakegrid.grid[(unsigned int)(current.x)][(unsigned int)(current.y+1)]->insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					++current.y;
					break;
				}
				else if(tempnum==1&&posOK(pos2D(current.x,current.y-1))&&(lakegrid.grid[(unsigned int)(current.x)][(unsigned int)(current.y-1)]->object_ammount()==0||maxtries<0))//1=up
				{
					lakegrid.grid[(unsigned int)(current.x)][(unsigned int)(current.y-1)]->insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					--current.y;
					break;
				}
				/*--maxtries;
			}
		break;
		case SOUTH:
		case SOUTHWEST:
		
		case NORTH:
		case NORTHEAST:
			while(true)
			{
				tempnum=rand()%2;*/
				else if(tempnum==2&&posOK(pos2D(current.x-1,current.y))&&(lakegrid.grid[(unsigned int)(current.x-1)][(unsigned int)(current.y)]->object_ammount()==0||maxtries<0))//0=left
				{
					lakegrid.grid[(unsigned int)(current.x-1)][(unsigned int)(current.y)]->insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					--current.x;
					break;
				}
				else if(tempnum==3&&posOK(pos2D(current.x+1,current.y))&&(lakegrid.grid[(unsigned int)(current.x+1)][(unsigned int)(current.y)]->object_ammount()==0||maxtries<0))//1=right
				{
					lakegrid.grid[(unsigned int)(current.x+1)][(unsigned int)(current.y)]->insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					++current.x;
					break;
				}
				--maxtries;
			}
		/*break;
		default://UNKNOWN ie, it's dead center.
			collision();//Let the collision mechanics handle it and give it a random position.
			return;
		break;
	}*/
	cout<<getid()<<" ("<<getpos().x<<','<<getpos().y<<')'<<" says:\"I just fell, but nice to meet you!\""<<endl;
	//Output here, just in case collision is called.
}

IceStype IceSnormal::givetype() const
{
	return ICESNORMAL;
}

IceSnormal::~IceSnormal()
{
	
}

  /////////////////////////////////
 //			IceSbeginner		//
/////////////////////////////////

IceSbeginner::IceSbeginner(Lakegrid<IceSkater>& Lakegrid,unsigned long Id, unsigned int Time, unsigned int x, unsigned int y, unsigned short MaxDratio):IceSkater(Lakegrid,Id,1,Time,x,y),maxDratio(double(MaxDratio)/100)
{
	if(MaxDratio>10||!posOK(current))
	{
		cerr<<"ERROR: Impossible error at IceSbeginner constructor! Check your code!"<<endl;
		cin.get();
	}
}

IceSbeginner::IceSbeginner(Lakegrid<IceSkater>& Lakegrid,unsigned long Id, unsigned int Time, pos2D pos, unsigned short MaxDratio):IceSkater(Lakegrid,Id,1,Time,pos),maxDratio(double(MaxDratio)/100)
{
	if(MaxDratio>10)
	{
		cerr<<"ERROR: Impossible error at IceSbeginner constructor! Check your code!"<<endl;
		cin.get();
	}
}

direction IceSbeginner::propose_direction() const
{
	if(current.x>lakegrid.center.x)//is east
	{
		if(current.y>lakegrid.center.y)//is south
		{
			if(current.y>=lakegrid.vside-maxDratio*lakegrid.vside)//Is closer to the side then 30% of its length
			{
				if (current.y<lakegrid.hside)//If it hasn't reached the edge
				{
					return SOUTHWEST;
				}
				else
				{
					return WEST;
				}
			}
			else
			{
				if ((current.x)>lakegrid.hside-maxDratio*lakegrid.hside)//If it's still not close to the limit
				{
					return SOUTHWEST;
				}
				else
				{
					return SOUTH;
				}
			}
		}
		else if(current.y<lakegrid.center.y)
		{
			if(current.x>=lakegrid.hside-maxDratio*lakegrid.hside)
			{
				if(current.x<lakegrid.hside)
				{
					return SOUTHEAST;
				}
				else
				{
					return SOUTH;
				}
			}
			else
			{
				if(current.y<lakegrid.vside*maxDratio)
				{
					return SOUTHEAST;
				}
				else
				{
					return EAST;
				}
			}
		}
		else//equal
		{
			return SOUTH;
		}
	}
	else if(current.x<lakegrid.center.x)
	{
		if(current.y>lakegrid.center.y)
		{
			if(current.x<=lakegrid.vside*maxDratio)
			{
				if(current.x>0)
				{
					return NORTHWEST;
				}
				else
				{
					return NORTH;
				}
			}
			else
			{
				if(current.y>lakegrid.vside-lakegrid.vside*maxDratio)
				{
					return NORTHWEST;
				}
				else
				{
					return WEST;
				}
			}
		}
		else if(current.y<lakegrid.center.y)
		{
			if(current.y<=lakegrid.hside*maxDratio)
			{
				if(current.y>0)
				{
					return NORTHEAST;
				}
				else
				{
					return EAST;
				}
			}
			else
			{
				if(current.x<lakegrid.hside*maxDratio)
				{
					return NORTHEAST;
				}
				else
				{
					return NORTH;
				}
			}
		}
		else//equal
		{
			return NORTH;
		}
	}
	else//equal
	{
		if(current.y>lakegrid.center.y)
		{
			return WEST;
		}
		else if(current.y<lakegrid.center.y)
		{
			return EAST;
		}
		else//equal, ie dead center
		{
			return UNKNOWN;
		}
	}
}

bool IceSbeginner::posOK(pos2D pos) const
{
	if(pos.x>lakegrid.hside||pos.x<0||pos.y>lakegrid.vside||pos.y<0)//Is within bounds
	{
		return false;
	}
	if(pos.x>lakegrid.hside*maxDratio&&pos.x<lakegrid.hside-lakegrid.hside*maxDratio&&pos.y>lakegrid.vside*maxDratio&&pos.y<lakegrid.vside-lakegrid.vside*maxDratio)//Distance grater than maxDratio allows
	{
		return false;
	}
	return true;
}

void IceSbeginner::collision()
{
	int tempnum;
	int maxtries=__ICESMAXTRIES__;
	unsigned int i;
	bool pairOK=true;
	if(hasPair())
	{
		pairOK=false;
	}
	cout<<getid()<<" ("<<getpos().x<<','<<getpos().y<<')'<<" says:\"I just fell, but nice to meet you!\""<<endl;
	heading=UNKNOWN;
	if(hasmoved)
	//If the collision was triggered by two IceSkaters ending up in the same square
	{
		for(i=1;&(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->get_referance(i))!=this;++i)//Find the object in the list
		{
			
		}
		while(true)//(Potential infinite loop? Check again later.)
		//Chose a random, adjacent, unoccupied spot and place the IceSkater there.
		{
			tempnum=rand()%8;
			if(tempnum==0&&posOK(pos2D(current.x-1,current.y-1))&&(lakegrid.grid[(unsigned int)(current.x-1)][(unsigned int)(current.y-1)]->object_ammount()==0||maxtries<0))
			//0=up_left
			{
				if(hasPair())
				{
					pairOK=check_pos_for_pair_OK(*this,pos2D((unsigned int)current.x+pair->getstartpos().x-(unsigned int)pair->getstartpos().x-1,(unsigned int)current.y+pair->getstartpos().y-(unsigned int)pair->getstartpos().y-1));
				}
				if(pairOK)
				{
					lakegrid.grid[(unsigned int)(current.x-1)][(unsigned int)(current.y-1)]->insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					--current.x;
					--current.y;
					break;
				}
			}
			else if(tempnum==1&&posOK(pos2D(current.x,current.y-1))&&(lakegrid.grid[(unsigned int)(current.x)][(unsigned int)(current.y-1)]->object_ammount()==0||maxtries<0))
			//1=up
			{
				if(hasPair())
				{
					pairOK=check_pos_for_pair_OK(*this,pos2D((unsigned int)current.x+pair->getstartpos().x-(unsigned int)pair->getstartpos().x,(unsigned int)current.y+pair->getstartpos().y-(unsigned int)pair->getstartpos().y-1));
					//check if the pair is ok with that position
				}
				if(pairOK)
				{
					lakegrid.grid[(unsigned int)(current.x)][(unsigned int)(current.y-1)]->insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					--current.y;
					break;
				}
			}
			else if(tempnum==2&&posOK(pos2D(current.x+1,current.y-1))&&(lakegrid.grid[(unsigned int)(current.x+1)][(unsigned int)(current.y-1)]->object_ammount()==0||maxtries<0))
			//2=up_right
			{
				if(hasPair())
				{
					pairOK=check_pos_for_pair_OK(*this,pos2D((unsigned int)current.x+pair->getstartpos().x-(unsigned int)pair->getstartpos().x+1,(unsigned int)current.y+pair->getstartpos().y-(unsigned int)pair->getstartpos().y-1));
				}
				lakegrid.grid[(unsigned int)(current.x+1)][(unsigned int)(current.y-1)]->insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
				++current.x;
				--current.y;
				break;
			}
			else if(tempnum==3&&posOK(pos2D(current.x-1,current.y))&&(lakegrid.grid[(unsigned int)(current.x-1)][(unsigned int)(current.y)]->object_ammount()==0||maxtries<0))
			//3=left
			{
				if(hasPair())
				{
					pairOK=check_pos_for_pair_OK(*this,pos2D((unsigned int)current.x+pair->getstartpos().x-(unsigned int)pair->getstartpos().x-1,(unsigned int)current.y+pair->getstartpos().y-(unsigned int)pair->getstartpos().y));
				}
				if(pairOK)
				{
					lakegrid.grid[(unsigned int)(current.x-1)][(unsigned int)(current.y)]->insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					--current.x;
					break;
				}
			}
			else if(tempnum==4&&posOK(pos2D(current.x+1,current.y))&&(lakegrid.grid[(unsigned int)(current.x+1)][(unsigned int)(current.y)]->object_ammount()==0||maxtries<0))
			//4=right
			{
				if(hasPair())
				{
					pairOK=check_pos_for_pair_OK(*this,pos2D((unsigned int)current.x+pair->getstartpos().x-(unsigned int)pair->getstartpos().x+1,(unsigned int)current.y+pair->getstartpos().y-(unsigned int)pair->getstartpos().y));
				}
				if(pairOK)
				{
					lakegrid.grid[(unsigned int)(current.x+1)][(unsigned int)(current.y)]->insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					++current.x;
					break;
				}
			}
			else if(tempnum==5&&posOK(pos2D(current.x-1,current.y+1))&&(lakegrid.grid[(unsigned int)(current.x-1)][(unsigned int)(current.y+1)]->object_ammount()==0||maxtries<0))
			//5=down_left
			{
				if(hasPair())
				{
					pairOK=check_pos_for_pair_OK(*this,pos2D((unsigned int)current.x+pair->getstartpos().x-(unsigned int)pair->getstartpos().x-1,(unsigned int)current.y+pair->getstartpos().y-(unsigned int)pair->getstartpos().y+1));
				}
				if(pairOK)
				{
					lakegrid.grid[(unsigned int)(current.x-1)][(unsigned int)(current.y+1)]->insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					--current.x;
					++current.y;
					break;
				}
			}
			else if(tempnum==6&&posOK(pos2D(current.x,current.y+1))&&(lakegrid.grid[(unsigned int)(current.x)][(unsigned int)(current.y+1)]->object_ammount()==0||maxtries<0))
			//6=down
			{
				if(hasPair())
				{
					pairOK=check_pos_for_pair_OK(*this,pos2D((unsigned int)current.x+pair->getstartpos().x-(unsigned int)pair->getstartpos().x,(unsigned int)current.y+pair->getstartpos().y-(unsigned int)pair->getstartpos().y+1));
				}
				if(pairOK)
				{
					lakegrid.grid[(unsigned int)(current.x)][(unsigned int)(current.y+1)]->insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					++current.y;
					break;
				}
			}
			else if(tempnum==7&&posOK(pos2D(current.x+1,current.y+1))&&(lakegrid.grid[(unsigned int)(current.x+1)][(unsigned int)(current.y+1)]->object_ammount()==0||maxtries<0))
			//7=down_right
			{
				if(hasPair())
				{
					pairOK=check_pos_for_pair_OK(*this,pos2D((unsigned int)current.x+pair->getstartpos().x-(unsigned int)pair->getstartpos().x+1,(unsigned int)current.y+pair->getstartpos().y-(unsigned int)pair->getstartpos().y+1));
				}
				if(pairOK)
				{
					lakegrid.grid[(unsigned int)(current.x+1)][(unsigned int)(current.y+1)]->insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					++current.x;
					++current.y;
					break;
				}
			}
			--maxtries;
		}
	}
	else//has overtaken someone
	{
		if(start.x==end.x&&start.y==end.y)
		{
			cerr<<"Impossible error at IceSkater::collision. Check your code."<<endl;
			cin.get();
		}
		if(start.y==end.y)//Need this check to escape zero.
		{
			do//Find an empty position along the way, that the pair accepts
			{
				tempnum=(rand()%((unsigned int)abs(end.x-start.x)+1))*sgn(end.x-start.x);
				if(hasPair())
				{
					pairOK=check_pos_for_pair_OK(*this,pos2D((unsigned int)start.x+pair->getpos().x-(unsigned int)pair->getpos().x+tempnum,(unsigned int)start.y+pair->getpos().y-(unsigned int)pair->getpos().y));
				}
				--maxtries;
			}
			while((lakegrid.grid[(unsigned int)(start.x+tempnum)][(unsigned int)start.y]->object_ammount()!=0&&maxtries>0)||!pairOK);
			for(i=1;&(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->get_referance(i))!=this;++i)//Find the object in the list
			{
				
			}
			lakegrid.grid[(unsigned int)(start.x+tempnum)][(unsigned int)(start.y)]->insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
			current.x=(start.x+tempnum);
			current.y=start.y;
			//change position and update current position
		}
		else//All moves are symmetrical. No more checks required.
		{
			do//Find an empty position along the way
			{
				tempnum=(rand()%((unsigned int)abs(end.y-start.y)+1))*sgn(end.y-start.y);
				if(hasPair())
				{
					pairOK=check_pos_for_pair_OK(*this,pos2D((unsigned int)start.x+pair->getpos().x-(unsigned int)pair->getpos().x+abs(tempnum)*sgn(end.x-start.x),(unsigned int)start.y+pair->getpos().y-(unsigned int)pair->getpos().y+tempnum));
				}
				--maxtries;
			}
			while((lakegrid.grid[(unsigned int)(start.x+abs(tempnum)*sgn(end.x-start.x))][(unsigned int)(start.y+tempnum)]->object_ammount()!=0&&maxtries>0)||!pairOK);
			for(i=1;(&lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->get_referance(i))!=this;++i)//Find the object in the list
			{
				
			}
			lakegrid.grid[(unsigned int)(start.x+abs(tempnum)*sgn(end.x-start.x))][(unsigned int)(start.y+tempnum)]->insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
			current.x=start.x+abs(tempnum)*sgn(end.x-start.x);
			current.y=start.y+tempnum;
			//change position and update current position
		}
		hasmoved=true;
	}
	if(hasPair())//Pair also falls
	{
		pair->paircollision();
	}
}

void IceSbeginner::paircollision()
{
	int tempnum;
	int maxtries=__ICESMAXTRIES__;
	unsigned int i;
	for(i=1;&(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->get_referance(i))!=this;++i)//Find the object in the list
	{
		
	}
	lakegrid.grid[(unsigned int)pair->getpos().x][(unsigned int)pair->getpos().y]->insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
	current.x=(unsigned int)pair->getpos().x+current.x-(unsigned int)current.x;
	current.y=(unsigned int)pair->getpos().y+current.y-(unsigned int)current.y;
	//Move the object to the pairs location
	hasmoved=true;
	heading=UNKNOWN;
	switch(propose_direction())
	{
		case EAST:
		case SOUTHEAST:
		
		case WEST:
		case NORTHWEST:
			while(true)
			{
				tempnum=rand()%2;
				if(tempnum==0&&posOK(pos2D(current.x,current.y+1))&&(lakegrid.grid[(unsigned int)(current.x)][(unsigned int)(current.y+1)]->object_ammount()==0||maxtries<0))//0=down
				{
					lakegrid.grid[(unsigned int)(current.x)][(unsigned int)(current.y+1)]->insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					++current.y;
					break;
				}
				else if(tempnum==1&&posOK(pos2D(current.x,current.y-1))&&(lakegrid.grid[(unsigned int)(current.x)][(unsigned int)(current.y-1)]->object_ammount()==0||maxtries<0))//1=up
				{
					lakegrid.grid[(unsigned int)(current.x)][(unsigned int)(current.y-1)]->insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					--current.y;
					break;
				}
				--maxtries;
			}
		break;
		case SOUTH:
		case SOUTHWEST:
		
		case NORTH:
		case NORTHEAST:
			while(true)
			{
				tempnum=rand()%2;
				if(tempnum==0&&posOK(pos2D(current.x-1,current.y))&&(lakegrid.grid[(unsigned int)(current.x-1)][(unsigned int)(current.y)]->object_ammount()==0||maxtries<0))//0=left
				{
					lakegrid.grid[(unsigned int)(current.x-1)][(unsigned int)(current.y)]->insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					--current.x;
					break;
				}
				else if(tempnum==1&&posOK(pos2D(current.x+1,current.y))&&(lakegrid.grid[(unsigned int)(current.x+1)][(unsigned int)(current.y)]->object_ammount()==0||maxtries<0))//1=right
				{
					lakegrid.grid[(unsigned int)(current.x+1)][(unsigned int)(current.y)]->insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					++current.x;
					break;
				}
				--maxtries;
			}
		break;
		default://UNKNOWN ie, it's dead center.
			collision();//Let the collision mechanics handle it and give it a random position.
			return;
		break;
	}
	cout<<getid()<<" ("<<getpos().x<<','<<getpos().y<<')'<<" says:\"I just fell, but nice to meet you!\""<<endl;
	//Output here, just in case collision is called.
}

void IceSbeginner::prepare_move()
{
	unsigned int i;
	hasmoved=false;
	start=current;
	heading=propose_direction();
	if(hasPair())
	{
		if(heading!=pair->propose_direction()&&pair->propose_direction()!=UNKNOWN)
		//If the pair doesn't know where to go (pair->propose direction()==UNKNOWN), it will pick this's direction.
		{
			if(pair->propose_direction()==(heading+6)%8+1)
			//if the pair wants to keep going foreward while this one wants to turn, then keep going foreward
			{
				heading=pair->propose_direction();
			}
			else
			{
				//cerr<<"Warning: Different directions for "<<getid()<<" and "<<pair->getid();
			}
		}
		if(distance2Dint(current,lakegrid.center)<distance2Dint(pair->getpos(),lakegrid.center))//If this is the skater closer to the center
		{
			for(i=1;&(lakegrid.grid[(unsigned int)pair->getpos().x][(unsigned int)pair->getpos().y]->get_referance(i))!=pair;++i)//Find the pair in the list
			{
				
			}
			switch(heading)//Make sure the pair has room
			{
				case SOUTHEAST:
					if((unsigned int)current.x==lakegrid.hside-1)
					{
						heading=SOUTH;
					}
				break;
				case NORTHWEST:
					if((unsigned int)current.x==1)
					{
						heading=NORTH;
					}
				break;
				case NORTHEAST:
					if((unsigned int)current.y==1)
					{
						heading=EAST;
					}
				break;
				case SOUTHWEST:
					if((unsigned int)current.y==lakegrid.vside-1)
					{
						heading=WEST;
					}
				break;
				default:
					cerr<<"ERROR: Impossible error at IceSbeginner::prepare_move! Check your code!"<<endl;
					cin.get();
				break;
			}
			switch(heading)//Reposition pair for turn if necessary.
			{
				case EAST:
				case SOUTHEAST:
					if((unsigned int)(pair->getpos().x)!=(unsigned int)(getpos().x)||(unsigned int)(pair->getpos().y)!=(unsigned int)(getpos().y-1))
					{
						if((unsigned int)(getpos().y)!=0)
						{
							pair->force_move(pos2D((unsigned int)getpos().x+pair->getpos().x-(unsigned int)pair->getpos().x,(unsigned int)getpos().y+pair->getpos().y-(unsigned int)pair->getpos().y-1));
						}
						else
						{
							cerr<<"WARNING:Substituting "<<getid()<<" and "<<pair->getid()<<" ! Check your code!"<<endl;
							cin.get();
							pair->force_move(pos2D((unsigned int)getpos().x+pair->getpos().x-(unsigned int)pair->getpos().x,(unsigned int)getpos().y+pair->getpos().y-(unsigned int)pair->getpos().y+1));
						}
						pair->prepare_move();//Since the pair has been altered, it's moves must be replotted
						prepare_move();
						return;
					}
				break;
				case SOUTH:
				case SOUTHWEST:
					if((unsigned int)(pair->getpos().x)!=(unsigned int)(getpos().x+1)||(unsigned int)(pair->getpos().y)!=(unsigned int)(getpos().y))
					{
						if((unsigned int)(getpos().x)!=lakegrid.hside)
						{
							pair->force_move(pos2D((unsigned int)getpos().x+pair->getpos().x-(unsigned int)pair->getpos().x+1,(unsigned int)getpos().y+pair->getpos().y-(unsigned int)pair->getpos().y));
						}
						else
						{
							cerr<<"WARNING:Substituting "<<getid()<<" and "<<pair->getid()<<" ! Check your code!"<<endl;
							cin.get();
							pair->force_move(pos2D((unsigned int)getpos().x+pair->getpos().x-(unsigned int)pair->getpos().x-1,(unsigned int)getpos().y+pair->getpos().y-(unsigned int)pair->getpos().y));
						}
						pair->prepare_move();//Since the pair has been altered, it's moves must be replotted
						prepare_move();
						return;
					}
				break;
				case WEST:
				case NORTHWEST:
					if((unsigned int)(pair->getpos().x)!=(unsigned int)(getpos().x)||(unsigned int)(pair->getpos().y)!=(unsigned int)(getpos().y+1))
					{
						if((unsigned int)(getpos().y)!=lakegrid.vside)
						{
							pair->force_move(pos2D((unsigned int)getpos().x+pair->getpos().x-(unsigned int)pair->getpos().x,(unsigned int)getpos().y+pair->getpos().y-(unsigned int)pair->getpos().y+1));
						}
						else
						{
							cerr<<"WARNING:Substituting "<<getid()<<" and "<<pair->getid()<<" ! Check your code!"<<endl;
							cin.get();
							pair->force_move(pos2D((unsigned int)getpos().x+pair->getpos().x-(unsigned int)pair->getpos().x,(unsigned int)getpos().y+pair->getpos().y-(unsigned int)pair->getpos().y-1));
						}
						pair->prepare_move();//Since the pair has been altered, it's moves must be replotted
						prepare_move();
						return;
					}
				break;
				case NORTH:
				case NORTHEAST:
					if((unsigned int)(pair->getpos().x)!=(unsigned int)(getpos().x-1)||(unsigned int)(pair->getpos().y)!=(unsigned int)(getpos().y))
					{
						if((unsigned int)(getpos().x)!=0)
						{
							pair->force_move(pos2D((unsigned int)getpos().x+pair->getpos().x-(unsigned int)pair->getpos().x-1,(unsigned int)getpos().y+pair->getpos().y-(unsigned int)pair->getpos().y));
						}
						else
						{
							cerr<<"WARNING:Substituting "<<getid()<<" and "<<pair->getid()<<" ! Check your code!"<<endl;
							cin.get();
							pair->force_move(pos2D((unsigned int)getpos().x+pair->getpos().x-(unsigned int)pair->getpos().x+1,(unsigned int)getpos().y+pair->getpos().y-(unsigned int)pair->getpos().y));
						}
						pair->prepare_move();//Since the pair has been altered, it's moves must be replotted
						prepare_move();
						return;
					}
				break;
				default:
					cerr<<"ERROR: Impossible error at IceSbeginner::prepare_move! Check your code!"<<endl;
					cin.get();
				break;
			}
		}
	}
	//Set endpoint
	switch(heading)
	{
		case EAST:
			end.y=start.y;
			if((start.x+speed)<=lakegrid.hside)
			//If this skater is going to end up inside the lake.
			{
				if((unsigned int)(start.x+speed)==lakegrid.hside&&hasPair()&&distance2Dint(current,lakegrid.center)<distance2Dint(pair->getpos(),lakegrid.center))
				//If this skater is going to end up at the edge, has a pair and is on the inner circle (ie closer to the center than the pair).
				{
					end.x=start.x+speed-1;
					//Leave some room for the pair to turn.
				}
				else
				{
					end.x=start.x+speed;
				}
			}
			else
			//If this skater is going to end up outside the lake
			{
				if(hasPair()&&distance2Dint(current,lakegrid.center)<distance2Dint(pair->getpos(),lakegrid.center))
				//If this skater has a pair and is on the inner circle (ie closer to the center than the pair)
				{
					end.x=lakegrid.hside-1;
					//Leave some room for the pair.
				}
				else
				{
					end.x=lakegrid.hside;
				}
			}
		break;
		case SOUTHEAST:
			if(start.y+speed>lakegrid.vside*maxDratio&&start.x<lakegrid.hside-lakegrid.hside*maxDratio-(lakegrid.vside*maxDratio-start.y))
			//If it's going to cross the "forbidden region" (ie its distance is going to be grater than maxDratio sometime during its movement)
			{
				end.y=lakegrid.vside*maxDratio;
				end.x=start.x+end.y-start.y;
				//Stop at the limit
			}
			else if(start.x+speed<=lakegrid.hside&&start.y+speed<=lakegrid.vside)
			//If this skater is going to end up inside the lake.
			{
				if(((unsigned int)(start.x+speed)==lakegrid.hside||(unsigned int)(start.y+speed)==lakegrid.vside)&&hasPair()&&distance2Dint(current,lakegrid.center)<distance2Dint(pair->getpos(),lakegrid.center))
				//If this skater is going to end up at the edge, has a pair and is on the inner circle (ie closer to the center than the pair).
				{
					end.x=start.x+speed-1;
					end.y=start.y+speed-1;
					//Leave some room for the pair to turn.
				}
				else
				{
					end.x=start.x+speed;
					end.y=start.y+speed;
				}
			}
			else
			//If this skater is going to end up outside the lake
			{
				if(hasPair()&&distance2Dint(current,lakegrid.center)<distance2Dint(pair->getpos(),lakegrid.center))
				//If this skater has a pair and is on the inner circle (ie closer to the center than the pair)
				{
					end.x=lakegrid.hside-1;
					//Leave some room for the pair.
				}
				else
				{
					end.x=lakegrid.hside;
				}
				end.y=start.y+end.x-start.x;
			}
		break;
		case SOUTH:
			end.x=start.x;
			if((start.y+speed)<=lakegrid.vside)
			//If the skater is going to end up inside the lake
			{
				if((unsigned int)(start.y+speed)==lakegrid.vside&&hasPair()&&distance2Dint(current,lakegrid.center)<distance2Dint(pair->getpos(),lakegrid.center))
				//If this skater is going to end up at the edge, has a pair and is on the inner circle (ie closer to the center than the pair).
				{
					end.y=start.y+speed-1;
				}
				else
				{
					end.y=start.y+speed;
				}
			}
			else
			//If the skater is going to end up outside the lake.
			{
				if(hasPair()&&distance2Dint(current,lakegrid.center)<distance2Dint(pair->getpos(),lakegrid.center))
				//If this skater has a pair and is on the inner circle (ie closer to the center than the pair)
				{
					end.y=lakegrid.vside-1;
				}
				else
				{
					end.y=lakegrid.vside;
				}
			}
		break;
		case SOUTHWEST:
			if(start.x-speed<lakegrid.hside-lakegrid.hside*maxDratio&&start.y<lakegrid.vside-lakegrid.vside*maxDratio-(start.x-(lakegrid.hside-lakegrid.hside*maxDratio)))
			//If it's going to cross the "forbidden region" (ie its distance is going to be grater than maxDratio sometime during its movement)
			{
				end.x=lakegrid.hside-lakegrid.hside*maxDratio;
				end.y=start.y+start.x-end.x;
				//Stop at the limit
			}
			else if(start.y+speed<=lakegrid.vside&&(start.x-speed)>=0)
			//If the skater is going to end up inside the lake
			{
				if(((unsigned int)(start.y+speed)==lakegrid.vside||(unsigned int)(start.x-speed)==0)&&hasPair()&&distance2Dint(current,lakegrid.center)<distance2Dint(pair->getpos(),lakegrid.center))
				//If this skater is going to end up at the edge, has a pair and is on the inner circle (ie closer to the center than the pair).
				{
					end.y=start.y+speed-1;
					end.x=start.x-speed+1;
				}
				else
				{
					end.y=start.y+speed;
					end.x=start.x-speed;
				}
			}
			else
			//If the skater is going to end up outside the lake.
			{
				if(hasPair()&&distance2Dint(current,lakegrid.center)<distance2Dint(pair->getpos(),lakegrid.center))
				//If this skater has a pair and is on the inner circle (ie closer to the center than the pair)
				{
					end.y=lakegrid.vside-1;
				}
				else
				{
					end.y=lakegrid.vside;
				}
				end.x=start.x-(end.y-start.y);
			}
		break;
		case WEST:
			end.y=start.y;
			if((start.x-speed)>=0)
			//If this skater is going to end up inside the lake.
			{
				if((unsigned int)(start.x-speed)==0&&hasPair()&&distance2Dint(current,lakegrid.center)<distance2Dint(pair->getpos(),lakegrid.center))
				//If this skater is going to end up at the edge, has a pair and is on the inner circle (ie closer to the center than the pair).
				{
					end.x=start.x-speed+1;
					//Leave some room for the pair to turn.
				}
				else
				{
					end.x=start.x-speed;
				}
			}
			else
			//If this skater is going to end up outside the lake
			{
				if(hasPair()&&distance2Dint(current,lakegrid.center)<distance2Dint(pair->getpos(),lakegrid.center))
				//If this skater has a pair and is on the inner circle (ie closer to the center than the pair)
				{
					end.x=1;
					//Leave some room for the pair.
				}
				else
				{
					end.x=0;
				}
			}
		break;
		case NORTHWEST:
			if(start.y-speed<lakegrid.vside-lakegrid.vside*maxDratio&&start.x>lakegrid.hside*maxDratio+(lakegrid.vside*maxDratio-start.y))
			//If it's going to cross the "forbidden region" (ie its distance is going to be grater than maxDratio sometime during its movement)
			{
				end.y=lakegrid.vside-lakegrid.vside*maxDratio;
				end.x=start.x-(start.y-end.y);
				//Stop at the limit
			}
			else if(start.x-speed>=0&&start.y-speed>=0)
			//If this skater is going to end up inside the lake.
			{
				if(((unsigned int)(start.y-speed)==0||(unsigned int)(start.x-speed)==lakegrid.hside)&&hasPair()&&distance2Dint(current,lakegrid.center)<distance2Dint(pair->getpos(),lakegrid.center))
				//If this skater is going to end up at the edge, has a pair and is on the inner circle (ie closer to the center than the pair).
				{
					end.x=start.x-speed+1;
					end.y=start.y-speed+1;
					//Leave some room for the pair to turn.
				}
				else
				{
					end.x=start.x-speed;
					end.y=start.y-speed;
				}
			}
			else
			//If this skater is going to end up outside the lake
			{
				if(hasPair()&&distance2Dint(current,lakegrid.center)<distance2Dint(pair->getpos(),lakegrid.center))
				//If this skater has a pair and is on the inner circle (ie closer to the center than the pair)
				{
					end.x=1;
					//Leave some room for the pair.
				}
				else
				{
					end.x=0;
				}
				end.y=start.y-(start.x-end.x);
			}
		break;
		case NORTH:
			end.x=start.x;
			if((start.y-speed)>=0)
			//If the skater is going to end up inside the lake
			{
				if((unsigned int)(start.y-speed)==0&&hasPair()&&distance2Dint(current,lakegrid.center)<distance2Dint(pair->getpos(),lakegrid.center))
				//If this skater is going to end up at the edge, has a pair and is on the inner circle (ie closer to the center than the pair).
				{
					end.y=start.y-speed+1;
				}
				else
				{
					end.y=start.y-speed;
				}
			}
			else
			//If the skater is going to end up outside the lake.
			{
				if(hasPair()&&distance2Dint(current,lakegrid.center)<distance2Dint(pair->getpos(),lakegrid.center))
				//If this skater has a pair and is on the inner circle (ie closer to the center than the pair)
				{
					end.y=1;
				}
				else
				{
					end.y=0;
				}
			}
		break;
		case NORTHEAST:
			if(start.x+speed>lakegrid.hside*maxDratio&&start.y>lakegrid.vside*maxDratio+(lakegrid.hside*maxDratio-start.x))
			//If it's going to cross the "forbidden region" (ie its distance is going to be grater than maxDratio sometime during its movement)
			{
				end.x=lakegrid.hside-lakegrid.hside*maxDratio;
				end.y=start.y+start.x-end.x;
				//Stop at the limit
			}
			else if((start.y-speed)>=0&&(start.x+speed)<=lakegrid.hside)
			//If the skater is going to end up inside the lake
			{
				if(((unsigned int)(start.y-speed)==0||(unsigned int)(start.x+speed)==lakegrid.hside)&&hasPair()&&distance2Dint(current,lakegrid.center)<distance2Dint(pair->getpos(),lakegrid.center))
				//If this skater is going to end up at the edge, has a pair and is on the inner circle (ie closer to the center than the pair).
				{
					end.y=start.y-speed+1;
					end.x=start.x+speed-1;
				}
				else
				{
					end.y=start.y-speed;
					end.x=start.x+speed;
				}
			}
			else
			//If the skater is going to end up outside the lake.
			{
				if(hasPair()&&distance2Dint(current,lakegrid.center)<distance2Dint(pair->getpos(),lakegrid.center))
				//If this skater has a pair and is on the inner circle (ie closer to the center than the pair)
				{
					end.y=1;
				}
				else
				{
					end.y=0;
				}
				end.x=start.x+(start.y-end.y);
			}
		break;
		default:
			cerr<<"ERROR: Impossible error at IceSkater::prepare_move! Check your code!"<<endl;
			cin.get();
		break;
	}
}

IceStype IceSbeginner::givetype() const
{
	return ICESBEGINNER;
}

IceSbeginner::~IceSbeginner()
{
	
}

  /////////////////////////////////
 //			IceSchild			//
/////////////////////////////////

IceSchild::IceSchild(Lakegrid<IceSkater>& Lakegrid,unsigned long Id,unsigned int Time, pos2D pos,unsigned short MaxDratio,int Maxfalls):IceSbeginner(Lakegrid,Id,Time,pos,MaxDratio),falls(Maxfalls)
{
	
}
IceSchild::IceSchild(Lakegrid<IceSkater>& Lakegrid,unsigned long Id,unsigned int Time, unsigned int x, unsigned int y,unsigned short MaxDratio,int Maxfalls):IceSbeginner(Lakegrid,Id,Time,x,y,MaxDratio),falls(Maxfalls)
{
	
}

void IceSchild::collision()
{
	int tempnum;
	int maxtries=__ICESMAXTRIES__;
	unsigned int i;
	bool pairOK=true;
	if(hasPair())
	{
		pairOK=false;
	}
	cout<<getid()<<" ("<<getpos().x<<','<<getpos().y<<')'<<" is crying..."<<endl;
	--falls;
	heading=UNKNOWN;
	if(hasmoved)
	//If the collision was triggered by two IceSkaters ending up in the same square
	{
		for(i=1;&(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->get_referance(i))!=this;++i)//Find the object in the list
		{
			
		}
		while(true)//(Potential infinite loop? Check again later.)
		//Chose a random, adjacent, unoccupied spot and place the IceSkater there.
		{
			tempnum=rand()%8;
			if(tempnum==0&&(unsigned int)current.x!=0&&(unsigned int)current.y!=0&&(lakegrid.grid[(unsigned int)(current.x-1)][(unsigned int)(current.y-1)]->object_ammount()==0||maxtries<0))
			//0=up_left
			{
				if(hasPair())
				{
					pairOK=check_pos_for_pair_OK(*this,pos2D((unsigned int)current.x+pair->getstartpos().x-(unsigned int)pair->getstartpos().x-1,(unsigned int)current.y+pair->getstartpos().y-(unsigned int)pair->getstartpos().y-1));
				}
				if(pairOK)
				{
					lakegrid.grid[(unsigned int)(current.x-1)][(unsigned int)(current.y-1)]->insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					--current.x;
					--current.y;
					break;
				}
			}
			else if(tempnum==1&&(unsigned int)current.y!=0&&(lakegrid.grid[(unsigned int)(current.x)][(unsigned int)(current.y-1)]->object_ammount()==0||maxtries<0))
			//1=up
			{
				if(hasPair())
				{
					pairOK=check_pos_for_pair_OK(*this,pos2D((unsigned int)current.x+pair->getstartpos().x-(unsigned int)pair->getstartpos().x,(unsigned int)current.y+pair->getstartpos().y-(unsigned int)pair->getstartpos().y-1));
				}
				if(pairOK)
				{
					lakegrid.grid[(unsigned int)(current.x)][(unsigned int)(current.y-1)]->insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					--current.y;
					break;
				}
			}
			else if(tempnum==2&&(unsigned int)current.y!=0&&((unsigned int)current.x!=lakegrid.hside)&&(lakegrid.grid[(unsigned int)(current.x+1)][(unsigned int)(current.y-1)]->object_ammount()==0||maxtries<0))
			//2=up_right
			{
				if(hasPair())
				{
					pairOK=check_pos_for_pair_OK(*this,pos2D((unsigned int)current.x+pair->getstartpos().x-(unsigned int)pair->getstartpos().x+1,(unsigned int)current.y+pair->getstartpos().y-(unsigned int)pair->getstartpos().y-1));
				}
				lakegrid.grid[(unsigned int)(current.x+1)][(unsigned int)(current.y-1)]->insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
				++current.x;
				--current.y;
				break;
			}
			else if(tempnum==3&&(unsigned int)current.x!=0&&(lakegrid.grid[(unsigned int)(current.x-1)][(unsigned int)(current.y)]->object_ammount()==0||maxtries<0))
			//3=left
			{
				if(hasPair())
				{
					pairOK=check_pos_for_pair_OK(*this,pos2D((unsigned int)current.x+pair->getstartpos().x-(unsigned int)pair->getstartpos().x-1,(unsigned int)current.y+pair->getstartpos().y-(unsigned int)pair->getstartpos().y));
				}
				if(pairOK)
				{
					lakegrid.grid[(unsigned int)(current.x-1)][(unsigned int)(current.y)]->insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					--current.x;
					break;
				}
			}
			else if(tempnum==4&&((unsigned int)current.x!=lakegrid.hside)&&(lakegrid.grid[(unsigned int)(current.x+1)][(unsigned int)(current.y)]->object_ammount()==0||maxtries<0))
			//4=right
			{
				if(hasPair())
				{
					pairOK=check_pos_for_pair_OK(*this,pos2D((unsigned int)current.x+pair->getstartpos().x-(unsigned int)pair->getstartpos().x+1,(unsigned int)current.y+pair->getstartpos().y-(unsigned int)pair->getstartpos().y));
				}
				if(pairOK)
				{
					lakegrid.grid[(unsigned int)(current.x+1)][(unsigned int)(current.y)]->insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					++current.x;
					break;
				}
			}
			else if(tempnum==5&&(unsigned int)current.x!=0&&((unsigned int)current.y!=lakegrid.vside)&&(lakegrid.grid[(unsigned int)(current.x-1)][(unsigned int)(current.y+1)]->object_ammount()==0||maxtries<0))
			//5=down_left
			{
				if(hasPair())
				{
					pairOK=check_pos_for_pair_OK(*this,pos2D((unsigned int)current.x+pair->getstartpos().x-(unsigned int)pair->getstartpos().x-1,(unsigned int)current.y+pair->getstartpos().y-(unsigned int)pair->getstartpos().y+1));
				}
				if(pairOK)
				{
					lakegrid.grid[(unsigned int)(current.x-1)][(unsigned int)(current.y+1)]->insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					--current.x;
					++current.y;
					break;
				}
			}
			else if(tempnum==6&&((unsigned int)current.y!=lakegrid.vside)&&(lakegrid.grid[(unsigned int)(current.x)][(unsigned int)(current.y+1)]->object_ammount()==0||maxtries<0))
			//6=down
			{
				if(hasPair())
				{
					pairOK=check_pos_for_pair_OK(*this,pos2D((unsigned int)current.x+pair->getstartpos().x-(unsigned int)pair->getstartpos().x,(unsigned int)current.y+pair->getstartpos().y-(unsigned int)pair->getstartpos().y+1));
				}
				if(pairOK)
				{
					lakegrid.grid[(unsigned int)(current.x)][(unsigned int)(current.y+1)]->insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					++current.y;
					break;
				}
			}
			else if(tempnum==7&&((unsigned int)current.x!=lakegrid.hside)&&((unsigned int)current.y!=lakegrid.vside)&&(lakegrid.grid[(unsigned int)(current.x+1)][(unsigned int)(current.y+1)]->object_ammount()==0||maxtries<0))
			//7=down_right
			{
				if(hasPair())
				{
					pairOK=check_pos_for_pair_OK(*this,pos2D((unsigned int)current.x+pair->getstartpos().x-(unsigned int)pair->getstartpos().x+1,(unsigned int)current.y+pair->getstartpos().y-(unsigned int)pair->getstartpos().y+1));
				}
				if(pairOK)
				{
					lakegrid.grid[(unsigned int)(current.x+1)][(unsigned int)(current.y+1)]->insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					++current.x;
					++current.y;
					break;
				}
			}
			--maxtries;
		}
	}
	else//has overtaken someone
	{
		if(start.x==end.x&&start.y==end.y)
		{
			cerr<<"Impossible error at IceSkater::collision. Check your code."<<endl;
			cin.get();
		}
		if(start.y==end.y)//Need this check to escape zero.
		{
			do//Find an empty position along the way, that the pair accepts
			{
				tempnum=(rand()%((unsigned int)abs(end.x-start.x)+1))*sgn(end.x-start.x);
				if(hasPair())
				{
					pairOK=check_pos_for_pair_OK(*this,pos2D((unsigned int)start.x+pair->getpos().x-(unsigned int)pair->getpos().x+tempnum,(unsigned int)start.y+pair->getpos().y-(unsigned int)pair->getpos().y));
				}
				--maxtries;
			}
			while((lakegrid.grid[(unsigned int)(start.x+tempnum)][(unsigned int)start.y]->object_ammount()!=0&&maxtries>0)||!pairOK);
			for(i=1;&(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->get_referance(i))!=this;++i)//Find the object in the list
			{
				
			}
			lakegrid.grid[(unsigned int)(start.x+tempnum)][(unsigned int)(start.y)]->insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
			current.x=start.x+tempnum;
			current.y=start.y;
			//change position and update current position
		}
		else//All moves are symmetrical. No more checks required.
		{
			do//Find an empty position along the way
			{
				tempnum=(rand()%((unsigned int)abs(end.y-start.y)+1))*sgn(end.y-start.y);
				if(hasPair())
				{
					pairOK=check_pos_for_pair_OK(*this,pos2D((unsigned int)start.x+pair->getpos().x-(unsigned int)pair->getpos().x+abs(tempnum)*sgn(end.x-start.x),(unsigned int)start.y+pair->getpos().y-(unsigned int)pair->getpos().y+tempnum));
				}
				--maxtries;
			}
			while((lakegrid.grid[(unsigned int)(start.x+abs(tempnum)*sgn(end.x-start.x))][(unsigned int)(start.y+tempnum)]->object_ammount()!=0&&maxtries>0)||!pairOK);
			for(i=1;(&lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->get_referance(i))!=this;++i)//Find the object in the list
			{
				
			}
			lakegrid.grid[(unsigned int)(start.x+abs(tempnum)*sgn(end.x-start.x))][(unsigned int)(start.y+tempnum)]->insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
			current.x=start.x+abs(tempnum)*sgn(end.x-start.x);
			current.y=start.y+tempnum;
			//change position and update current position
		}
		hasmoved=true;
	}
	if(hasPair())//Pair also falls
	{
		pair->paircollision();
	}
}

void IceSchild::paircollision()
{
	int tempnum;
	int maxtries=__ICESMAXTRIES__;
	unsigned int i;
	for(i=1;&(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->get_referance(i))!=this;++i)//Find the object in the list
	{
		
	}
	lakegrid.grid[(unsigned int)pair->getpos().x][(unsigned int)pair->getpos().y]->insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
	current.x=(unsigned int)pair->getpos().x+current.x-(unsigned int)current.x;
	current.y=(unsigned int)pair->getpos().y+current.y-(unsigned int)current.y;
	//Move the object to the pairs location
	hasmoved=true;
	heading=UNKNOWN;
	switch(propose_direction())
	{
		case EAST:
		case SOUTHEAST:
		
		case WEST:
		case NORTHWEST:
			while(true)
			{
				tempnum=rand()%2;
				if(tempnum==0&&posOK(pos2D(current.x,current.y+1))&&(lakegrid.grid[(unsigned int)(current.x)][(unsigned int)(current.y+1)]->object_ammount()==0||maxtries<0))//0=down
				{
					lakegrid.grid[(unsigned int)(current.x)][(unsigned int)(current.y+1)]->insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					++current.y;
					break;
				}
				else if(tempnum==1&&posOK(pos2D(current.x,current.y-1))&&(lakegrid.grid[(unsigned int)(current.x)][(unsigned int)(current.y-1)]->object_ammount()==0||maxtries<0))//1=up
				{
					lakegrid.grid[(unsigned int)(current.x)][(unsigned int)(current.y-1)]->insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					--current.y;
					break;
				}
				--maxtries;
			}
		break;
		case SOUTH:
		case SOUTHWEST:
		
		case NORTH:
		case NORTHEAST:
			while(true)
			{
				tempnum=rand()%2;
				if(tempnum==0&&posOK(pos2D(current.x-1,current.y))&&(lakegrid.grid[(unsigned int)(current.x-1)][(unsigned int)(current.y)]->object_ammount()==0||maxtries<0))//0=left
				{
					lakegrid.grid[(unsigned int)(current.x-1)][(unsigned int)(current.y)]->insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					--current.x;
					break;
				}
				else if(tempnum==1&&posOK(pos2D(current.x+1,current.y))&&(lakegrid.grid[(unsigned int)(current.x+1)][(unsigned int)(current.y)]->object_ammount()==0||maxtries<0))//1=right
				{
					lakegrid.grid[(unsigned int)(current.x+1)][(unsigned int)(current.y)]->insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
					++current.x;
					break;
				}
				--maxtries;
			}
		break;
		default://UNKNOWN ie, it's dead center.
			collision();//Let the collision mechanics handle it and give it a random position.
			return;
		break;
	}
	cout<<getid()<<" ("<<getpos().x<<','<<getpos().y<<')'<<" is crying..."<<endl;
	--falls;
	//Output here, just in case collision is called.
}

void IceSchild::end_move()
{
	int i;
	if((--time)==0||falls==0)
	{
		for(i=1;&(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->get_referance(i))!=this;++i)//Find the pair in the list
		{
			
		}
		goodbye_msg();
		if(hasPair())
		{
			pair->goodbye_msg();
		}
		delete lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i);
	}
}

void IceSchild::goodbye_msg() const
{
	cout<<getid()<<" ("<<getpos().x<<','<<getpos().y<<')'<<" says:\"I want to stay longer...\""<<endl;
}

IceStype IceSchild::givetype() const
{
	return ICESCHILD;
}

IceSchild::~IceSchild()
{
	
}

  /////////////////////////////////
 //			IceSexpert			//
/////////////////////////////////
IceSexpert::IceSexpert(Lakegrid<IceSkater>& Lakegrid,unsigned long Id,unsigned int Speed,unsigned int Time, pos2D pos):IceSkater(Lakegrid,Id,Speed,Time,pos)
{
	
}

IceSexpert::IceSexpert(Lakegrid<IceSkater>& Lakegrid,unsigned long Id, unsigned int Speed, unsigned int Time, unsigned int x, unsigned int y):IceSkater(Lakegrid,Id,Speed,Time,x,y)
{
	
}

void IceSexpert::collision()
{
	bool hasnonfaller=false;
	if(has_Moved())
	{
		for (unsigned int i=1;i<=lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->object_ammount();++i)
		{
			if(!lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->get_referance(i).isfaller()&&(&(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->get_referance(i)))!=this)
			{
				hasnonfaller=true;
				break;
			}
		}
		if (hasnonfaller)//Make him move if he is sitting in the same tile with an IceSkater type other than the ones that relocate
		{
			this->IceSkater::collision();
		}
		else
		{
			cout<<getid()<<" ("<<getpos().x<<','<<getpos().y<<')'<<" says:\"I simply overcame the situation!\""<<endl;	
		}
	}
	else
	{
		cout<<getid()<<" ("<<getpos().x<<','<<getpos().y<<')'<<" says:\"I simply overcame the situation!\""<<endl;	
	}
}

void IceSexpert::move()
{
	unsigned int i;
	const short signx=sgn(end.x-start.x);
	const short signy=sgn(end.y-start.y);
	if(hasPair())
	{
		//If the pair is going to stop sooner also stop sooner
		if((signx>0&&(unsigned int)pair->getendpos().x<(unsigned int)end.x+(unsigned int)pair->getstartpos().x-(unsigned int)start.x)||(signx<0&&(unsigned int)pair->getendpos().x>(unsigned int)end.x+(unsigned int)pair->getstartpos().x-(unsigned int)start.x))
		{
			end.x=(unsigned int)pair->getendpos().x+start.x-pair->getstartpos().x;
		}
		if((signy>0&&pair->getendpos().y<end.y+pair->getstartpos().y-start.y)||(signy<0&&pair->getendpos().y>end.y+pair->getstartpos().y-start.y))
		{
			end.y=(unsigned int)pair->getendpos().y+start.y-pair->getstartpos().y;
		}
	}
	if (start.x==end.x&&start.y==end.y)
	{
		cerr<<"IceSkater "<<getid()<<" remained immobile."<<endl;
	}
	if(start.x==end.x)
	{
		for(long int i=start.y;i!=(long int)end.y+signy;i+=signy)//Start checking along the way
		{
			for(unsigned int k=1;k<=lakegrid.grid[(unsigned int)start.x][i]->object_ammount();k++)
			//All objects in all squares
			{
				if(&(lakegrid.grid[(unsigned int)start.x][i]->get_referance(k))!=this&&lakegrid.grid[(unsigned int)start.x][i]->get_referance(k).getheading()==heading&&signy*lakegrid.grid[(unsigned int)start.x][i]->get_referance(k).getendpos().y<signy*end.y)
				//If the object overtakes another object
				{
					if(lakegrid.grid[(unsigned int)start.x][i]->get_referance(k).isfaller())
					{
						lakegrid.grid[(unsigned int)start.x][i]->get_referance(k).sethasmoved(false);//We want the overtake function to work, not the same tile one.
					}
					lakegrid.grid[(unsigned int)start.x][i]->get_referance(k).collision();
					collision();
				}
			}
		}
	}
	else
	{
		for(long int i=start.x;i!=(long int)end.x+signx;i+=signx)
		{
			for(unsigned int k=1;k<=lakegrid.grid[i][(unsigned int)(start.y+abs((unsigned int)start.x-i)*signy)]->object_ammount();k++)
			{
				if(&(lakegrid.grid[i][(unsigned int)(start.y+abs((unsigned int)start.x-i)*signy)]->get_referance(k))!=this&&lakegrid.grid[i][(unsigned int)(start.y+abs((unsigned int)start.x-i)*signy)]->get_referance(k).getheading()==heading&&signy*lakegrid.grid[i][(unsigned int)(start.y+abs((unsigned int)start.x-i)*signy)]->get_referance(k).getendpos().y<signy*end.y)
				{
					if(lakegrid.grid[i][(unsigned int)(start.y+abs((unsigned int)start.x-i)*signy)]->get_referance(k).isfaller())
					{
						lakegrid.grid[i][(unsigned int)(start.y+abs((unsigned int)start.x-i)*signy)]->get_referance(k).sethasmoved(false);
					}
					lakegrid.grid[i][(unsigned int)(start.y+abs((unsigned int)start.x-i)*signy)]->get_referance(k).collision();
					collision();
				}
			}
		}
	}
	//After this we conclude that the no obstacles were encountered, so the move is the successful
	for(i=1;&(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->get_referance(i))!=this;++i)//Find the pair in the list
	{
		
	}
	lakegrid.grid[(unsigned int)(end.x)][(unsigned int)(end.y)]->insert_start(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->pop_pointer(i));
	current.x=end.x;
	current.y=end.y;
	hasmoved=true;
	if(hasPair()&&!pair->has_Moved())
	{
		pair->move();
	}
	if(lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->object_ammount()!=1&&lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->get_finish_referance().has_Moved())
	{
		lakegrid.grid[(unsigned int)current.x][(unsigned int)current.y]->get_finish_referance().collision();
	collision();
	}
}

inline IceStype IceSexpert::givetype() const
{
	return ICESEXPERT;
}

inline bool IceSexpert::isfaller() const
{
	return false;
}

IceSexpert::~IceSexpert()
{
	
}
