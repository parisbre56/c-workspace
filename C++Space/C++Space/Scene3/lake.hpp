#ifndef __X3IceSlake__
#define __X3IceSlake__

#include "IceSkater.hpp"
#include "IceSlist.hpp"
#include "lakegrid.hpp"

#define __ICESMAXTIME__ 100
#define __ICESMAXFALLS__ 10

class Lake
{
private:
	Lakegrid<IceSkater> grid;//[s]The grid. A digital frontier.[/s] The lakegrid. All IceSkaters are in here.
	
	void filllake(unsigned int Skaternum);//Fills the lake with skaternum IceSkaters. A switch(rand) selects the type of IceSkater. Two rands (position also depends on IceStype) with a check for empty position. Make sure there are enough spots left for certain types  (use a counter).
	void checkpairs();//Checks if there are any pairs. If there are it tells the two IceSkaters that they are pairs. Remember to 
	void preparemove();//Gives the prepare move signal to all IceSkaters. All IceSkaters now know their start and end position.
	void startmove();//Gives the start move signal to all IceSkaters. Ice Skaters move one at a time and reach their destination, checking if other IceSkaters along the way are moving in the same direction at a gridpos closer than their destination. If there are a collision happens. Collissions set the hasMoved flag.
	void endmove();//Gives the end move signal to all IceSkaters. Resets the hasMove flag. Check if an IceSkater must leave and does that. Final collision check. Remember to make sure that there are no collisions left after the initial check. Remember that IceSexperts don't collide with each other.
public:
	Lake(unsigned int hSide, unsigned int vSide, unsigned int Skaternum);
	void output();//Print IceSkater location.
	void advance();//Next frame
	void skaterinfo(unsigned long x, unsigned long y);//gives info about the skater at (x,y) or the appropriate message if it doesn't exist
	~Lake();
};

#endif
