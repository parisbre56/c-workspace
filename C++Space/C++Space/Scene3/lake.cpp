#include "lake.hpp"
#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;

inline unsigned long long roundupdiv(unsigned long long x, unsigned long long y) //Does x/y and rounds up. ONLY FOR INTEGER TYPES!!!
{
	return 1+((x-1)/y);
}

inline int roundint(double x)
{
    return static_cast<int>(floor(x + 0.5f));
}

template <class T>
inline bool posOK(Lakegrid<T>& grid, float Dratio, pos2D pos)
{
	Dratio=Dratio/10;
	if((pos.x>grid.hside*Dratio&&pos.x<grid.hside-grid.hside*Dratio)||(pos.y>grid.vside*Dratio&&pos.y<grid.vside-grid.vside*Dratio))//Distance grater than maxDratio allows
	{
		return false;
	}
	if(pos.x>grid.hside||pos.x<0||pos.y>grid.vside||pos.y<0)//Is within bounds
	{
		return false;
	}
	return true;
}

//Private

void Lake::filllake(unsigned int Skaternum)
//Fills the lake with skaternum IceSkaters. A switch(rand) selects the type of IceSkater. Two rands (position also depends on IceStype) with a check for empty position.
{
	pos2D temppos;
	double tempdratio;
	unsigned short temprand;
	unsigned long long tenprecentnum=(grid.hside+1)*(grid.vside+1)-roundupdiv(grid.hside*80,100)*roundupdiv(grid.vside*80,100); 
	//The number of squares left that can be occupied by objects with a 10 precent maxdistance from the border.
	unsigned long long thirtyprecentnum=(grid.hside+1)*(grid.vside+1)-roundupdiv(grid.hside*40,100)*roundupdiv(grid.vside*40,100);
	//The number of squares left that can be occupied by objects with a 30 precent maxdistance from the border.
	while(Skaternum>0)
	{
		if(tenprecentnum>0)
		//There's room for all kinds of skaters
		//To better understand this see enum IceStype in IceSkater.hpp
		{
			temprand=rand()%4+2;
		}
		else if(thirtyprecentnum>0)
		//There's only room for normals and experts
		{
			temprand=ICESNORMAL+(rand()%2)*(ICESEXPERT-ICESNORMAL);
		}
		else
		//There's only room for experts
		{
			temprand=ICESEXPERT;
		}
		//temprand=ICESEXPERT;
		switch(temprand)//What type of IceSkater is it?
		{
			case ICESNORMAL:
				tempdratio=(double)((rand()%30)+1)/100;//The maximum distance from the sides
				switch(rand()%4)//Which side does it spawn closer to?
				{
					case 0://Left
						temppos=(pos2D(grid.hside*tempdratio,grid.vside*tempdratio+rand()%((unsigned int)(grid.vside-grid.vside*tempdratio*2+1))));
					break;
					case 1://Up
						temppos=(pos2D(grid.hside*tempdratio+rand()%((unsigned int)(grid.hside-grid.hside*tempdratio*2+1)),grid.vside*tempdratio));
					break;
					case 2://Right
						temppos=(pos2D(grid.hside-grid.hside*tempdratio,grid.vside*tempdratio+rand()%((unsigned int)(grid.vside-grid.vside*tempdratio*2+1))));
					break;
					case 3://Down
						temppos=(pos2D(grid.hside*tempdratio+rand()%((unsigned int)(grid.hside-grid.hside*tempdratio*2+1)),grid.vside-grid.vside*tempdratio));
					break;
				}
				if(grid.grid[(unsigned int)temppos.x][(unsigned int)temppos.y]->object_ammount()==0)
				{
					grid.grid[(unsigned int)temppos.x][(unsigned int)temppos.y]->insert_start(new IceSnormal(grid,Skaternum--,rand()%(((grid.hside<grid.vside)?(grid.hside):(grid.vside))/2)+1,rand()%__ICESMAXTIME__+1,temppos,roundint(tempdratio*100)));
					if(posOK(grid,10,temppos))
					{
						--tenprecentnum;
						--thirtyprecentnum;
					}
					else
					{
						--thirtyprecentnum;
					}
					
				}
			break;
			case ICESBEGINNER:
				tempdratio=10.0/100;
				switch(rand()%4)//Which side does it spawn closer to?
				{
					case 0://Left
						temppos=pos2D(rand()%((unsigned int)(grid.hside*tempdratio)+1),rand()%(grid.vside+1));
					break;
					case 1://Up
						temppos=pos2D(rand()%(grid.hside+1),rand()%((unsigned int)(grid.vside*tempdratio)+1));
					break;
					case 2://Right
						temppos=pos2D(grid.hside-rand()%((unsigned int)(grid.hside*tempdratio)+1),rand()%(grid.vside+1));
					break;
					case 3://Down
						temppos=pos2D(rand()%(grid.hside+1),grid.vside-rand()%((unsigned int)(grid.vside*tempdratio)+1));
					break;
				}
				if(grid.grid[(unsigned int)temppos.x][(unsigned int)temppos.y]->object_ammount()==0)
				{
					grid.grid[(unsigned int)temppos.x][(unsigned int)temppos.y]->insert_start(new IceSbeginner(grid,Skaternum--,rand()%__ICESMAXTIME__+1,temppos,10));
					--tenprecentnum;
					--thirtyprecentnum;
				}
			break;
			case ICESCHILD:
				tempdratio=10.0/100;
				switch(rand()%4)//Which side does it spawn closer to?
				{
					case 0://Left
						temppos=pos2D(rand()%((unsigned int)(grid.hside*tempdratio)+1),rand()%(grid.vside+1));
					break;
					case 1://Up
						temppos=pos2D(rand()%(grid.hside+1),rand()%((unsigned int)(grid.vside*tempdratio)+1));
					break;
					case 2://Right
						temppos=pos2D(grid.hside-rand()%((unsigned int)(grid.hside*tempdratio)+1),rand()%(grid.vside+1));
					break;
					case 3://Down
						temppos=pos2D(rand()%(grid.hside+1),grid.vside-rand()%((unsigned int)(grid.vside*tempdratio)+1));
					break;
				}
				if(grid.grid[(unsigned int)temppos.x][(unsigned int)temppos.y]->object_ammount()==0)
				{
					grid.grid[(unsigned int)temppos.x][(unsigned int)temppos.y]->insert_start(new IceSchild(grid,Skaternum--,rand()%__ICESMAXTIME__+1,temppos,10,rand()%__ICESMAXFALLS__+1));
					--tenprecentnum;
					--thirtyprecentnum;
				}
			break;
			case ICESEXPERT:
				temppos=pos2D(rand()%(grid.hside+1),rand()%(grid.vside+1));
				if(grid.grid[(unsigned int)temppos.x][(unsigned int)temppos.y]->object_ammount()==0)
				{
					grid.grid[(unsigned int)temppos.x][(unsigned int)temppos.y]->insert_start(new IceSexpert(grid,Skaternum--,rand()%(((grid.hside<grid.vside)?(grid.hside):(grid.vside))/2)+1,rand()%__ICESMAXTIME__+1,temppos));
					if(posOK(grid,10,temppos))
					{
						--tenprecentnum;
						--thirtyprecentnum;
					}
					else if(posOK(grid,30,temppos))
					{
						--thirtyprecentnum;
					}
				}
			break;
			default:
				cerr<<"ERROR: Impossible error at Lake::filllake"<<endl;
				cin.get();
			break;
		}
	}
}

void Lake::checkpairs()
{
	for(unsigned int i=1;i<=grid.hside-1;++i)
	{
		for(unsigned int j=1;j<=grid.vside-1;++j)//The outer edge is covered, no need to check there
		{
			if(grid.grid[i][j]->object_ammount()!=0&&!(grid.grid[i][j]->get_start_referance().hasPair()))
			{
				switch(grid.grid[i][j]->get_start_referance().propose_direction())
				{
					case NORTH:
					case NORTHEAST:
					case SOUTH:
					case SOUTHWEST:
					//Search for a pair either left or right
						if(grid.grid[i-1][j]->object_ammount()!=0&&!(grid.grid[i-1][j]->get_start_referance().hasPair())&&(grid.grid[i-1][j]->get_start_referance().propose_direction()==grid.grid[i][j]->get_start_referance().propose_direction()||(grid.grid[i-1][j]->get_start_referance().propose_direction()+1)%7+1==grid.grid[i][j]->get_start_referance().propose_direction()||(grid.grid[i-1][j]->get_start_referance().propose_direction()-1)%7+1==grid.grid[i][j]->get_start_referance().propose_direction()))
						//If left has an unpaired IceSkater, going in a similar direction
						{
							grid.grid[i-1][j]->get_start_referance().setpair(&(grid.grid[i][j]->get_start_referance()));
						}
						else if(grid.grid[i+1][j]->object_ammount()!=0&&!(grid.grid[i+1][j]->get_start_referance().hasPair())&&(grid.grid[i+1][j]->get_start_referance().propose_direction()==grid.grid[i][j]->get_start_referance().propose_direction()||(grid.grid[i+1][j]->get_start_referance().propose_direction()+1)%7+1==grid.grid[i][j]->get_start_referance().propose_direction()||(grid.grid[i+1][j]->get_start_referance().propose_direction()-1)%7+1==grid.grid[i][j]->get_start_referance().propose_direction()))
						//If right has an unpaired IceSkater, going in a similar direction
						{
							grid.grid[i+1][j]->get_start_referance().setpair(&(grid.grid[i][j]->get_start_referance()));
						}
					break;
					case EAST:
					case SOUTHEAST:
					case WEST:
					case NORTHWEST:
					//Search for a pair either up or down
						if(grid.grid[i][j-1]->object_ammount()!=0&&!(grid.grid[i][j-1]->get_start_referance().hasPair())&&(grid.grid[i][j-1]->get_start_referance().propose_direction()==grid.grid[i][j]->get_start_referance().propose_direction()||(grid.grid[i][j-1]->get_start_referance().propose_direction()+1)%7+1==grid.grid[i][j]->get_start_referance().propose_direction()||(grid.grid[i][j-1]->get_start_referance().propose_direction()-1)%7+1==grid.grid[i][j]->get_start_referance().propose_direction()))
						//If up has an unpaired IceSkater, going in a similar direction
						{
							grid.grid[i][j-1]->get_start_referance().setpair(&(grid.grid[i][j]->get_start_referance()));
						}
						else if(grid.grid[i][j+1]->object_ammount()!=0&&!(grid.grid[i][j+1]->get_start_referance().hasPair())&&(grid.grid[i][j+1]->get_start_referance().propose_direction()==grid.grid[i][j]->get_start_referance().propose_direction()||(grid.grid[i][j+1]->get_start_referance().propose_direction()+1)%7+1==grid.grid[i][j]->get_start_referance().propose_direction()||(grid.grid[i][j+1]->get_start_referance().propose_direction()-1)%7+1==grid.grid[i][j]->get_start_referance().propose_direction()))
						//If down has an unpaired IceSkater, going in a similar direction
						{
							grid.grid[i][j+1]->get_start_referance().setpair(&(grid.grid[i][j]->get_start_referance()));
						}
					break;
					case UNKNOWN:
						//If it's dead center, do nothing (rarely happens)
					break;
					default:
						cerr<<"ERROR: Impossible error at Lake::checkpairs"<<endl;
						cin.get();
					break;
				}
			}
		}
	}
}

void Lake::preparemove()
{
	for(unsigned int i=0;i<=grid.hside;++i)
	{
		for(unsigned int j=0;j<=grid.vside;++j)
		{
			if(grid.grid[i][j]->object_ammount()!=0)
			{
				for(unsigned int k=1;k<=grid.grid[i][j]->object_ammount();++k)
				{
					grid.grid[i][j]->get_referance(k).prepare_move();
				}
			}
		}
	}
}

void Lake::startmove()
{
	for(unsigned int i=0;i<=grid.hside;++i)
	{
		for(unsigned int j=0;j<=grid.vside;++j)
		{
			if(grid.grid[i][j]->object_ammount()!=0)
			{
				for(unsigned int k=1;k<=grid.grid[i][j]->object_ammount();++k)
				{
					if(!(grid.grid[i][j]->get_referance(k).has_Moved()))
					{
						grid.grid[i][j]->get_referance(k).move();
					}
				}
			}
		}
	}
}

void Lake::endmove()
{
	bool escape=false;
	unsigned int k;
	cerr<<"\t\tChecking for collisions..."<<endl;
	while(!escape)
	{
		escape=true;
		for(unsigned int i=0;i<=grid.hside;++i)
		{
			for(unsigned int j=0;j<=grid.vside;++j)
			{
				if(grid.grid[i][j]->object_ammount()>1)
				{
					escape=false;
					for(k=grid.grid[i][j]->object_ammount();k>0;--k)
					{
						grid.grid[i][j]->get_referance(k).collision();
					}
				}
			}
		}
	}
	cerr<<"\t\tFinalising positions..."<<endl;
	for(unsigned int i=0;i<=grid.hside;++i)
	{
		for(unsigned int j=0;j<=grid.vside;++j)
		{
			if(grid.grid[i][j]->object_ammount()!=0)
			{
				if(grid.grid[i][j]->object_ammount()!=1)
				{
					cerr<<"ERROR: Impossible error at Lake::endmove"<<endl;
					cin.get();
				}
				if(!(grid.grid[i][j]->get_start_referance().has_Moved()))
				{
					cerr<<"ERROR: Impossible error at Lake::endmove"<<endl;
					cin.get();
				}
				grid.grid[i][j]->get_start_referance().end_move();
			}
		}
	}
	
}

//Public

Lake::Lake(unsigned int hSide, unsigned int vSide, unsigned int Skaternum):grid(hSide+1,vSide+1)
{
	if(((unsigned long long)(hSide+1)*(unsigned long long)(vSide+1))<Skaternum)
	{
		cerr<<"ERROR:Too many skaters.";
		cin.get();
	}
	cerr<<"Initialising simulation..."<<endl;;
	cerr<<"\tFilling lake with Ice Skaters..."<<endl;
	filllake(Skaternum);
	cerr<<"\tChecking for Ice Skater pairs..."<<endl;
	checkpairs();
	cerr<<"Done!"<<endl;
}

void Lake::output()
{
	cerr<<"Generating output..."<<endl;
	cout<<"\t|";
	for(unsigned int i=0;i<=grid.hside;++i)
	{
		cout<<i<<"\t|";
	}
	cout<<endl<<"-----\t+";
	for(unsigned int i=0;i<=grid.hside;++i)
	{
		cout<<"-----\t+";
	}
	cout<<endl;
	for(unsigned int j=0;j<=grid.vside;++j)
	{
		cout<<j<<'\t';
		for(unsigned int i=0;i<=grid.hside;++i)
		{
			cout<<'|';
			if(grid.grid[i][j]->object_ammount()==0)
			{
				cout<<' ';
			}
			else
			{
				if(grid.grid[i][j]->object_ammount()!=1)
				{
					cerr<<endl<<"ERROR: Impossible error at Lake::output"<<endl;
					cin.get();
				}
				if(grid.grid[i][j]->get_start_referance().getheading()==UNKNOWN)
				{
					cout<<"\033[31;5m";
				}
				switch(grid.grid[i][j]->get_start_referance().givetype())
				{
					case ICESKATER:
						cout<<'I';
					break;
					case ICESNORMAL:
						cout<<'N';
					break;
					case ICESBEGINNER:
						cout<<'B';
					break;
					case ICESCHILD:
						cout<<'C';
					break;
					case ICESEXPERT:
						cout<<'E';
					break;
					default:
						cout<<'U'<<endl;
					break;
				}
				cout<<':'<<grid.grid[i][j]->get_start_referance().getid();
				if(grid.grid[i][j]->get_start_referance().getheading()==UNKNOWN)
				{
					cout<<"\033[0m";
				}
			}
			cout<<'\t';
		}
		cout<<'|'<<endl<<"-----\t+";
		for(unsigned int i=0;i<=grid.hside;++i)
		{
			cout<<"-----\t+";
		}
		cout<<endl;
	}
	cerr<<"Done!"<<endl;
}

void Lake::advance()
{
	cerr<<"Advancing time..."<<endl;
	cerr<<"\tFinding next position..."<<endl;
	preparemove();
	cerr<<"\tMoving to next position..."<<endl;
	startmove();
	cerr<<"\tFinal checks..."<<endl;
	endmove();
	cerr<<"Done!"<<endl;
}

Lake::~Lake()
{
	for(unsigned int i=0;i<=grid.hside;++i)
	{
		for(unsigned int j=0;j<=grid.vside;++j)
		{
			for(unsigned int k=1;k<=grid.grid[i][j]->object_ammount();++k)
			{
				grid.grid[i][j]->get_referance(k).goodbye_msg();
				delete grid.grid[i][j]->pop_pointer(k);
				//The grid destructor should automatically delete all objects in it, 
				//but since we're going through them, why not do it right now?
			}
		}
	}
	cerr<<"Deleting lake..."<<endl;
}
