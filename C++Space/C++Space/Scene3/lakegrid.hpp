#ifndef __X3IceSlakegrid__
#define __X3IceSlakegrid__

/* Remember, all classes assume that RIGHT(EAST) and DOWN(SOUTH) are positive
 * and LEFT(WEST) and UP(NORTH) are negative. That means that the upper left
 * cell is [0][0], the upper right [hside][0], the lower left [0][vside] and
 * the lower right [hside][vside].
 * 
 *  +-->
 *  |
 *  | Positive!
 *  v
 */

#include "IceSlist.hpp"
#include "pos2D.hpp"

template <class TC>
class Lakegrid
{
public:
	const unsigned int hside;//The maximum value that can be given to the array (ie grid[hside][vside] is the last cell)
	const unsigned int vside;//The maximum value that can be given to the array
	pos2D center;
	List<TC>*** grid;
	
	Lakegrid(unsigned int hSide, unsigned int vSide);
	~Lakegrid();
};

template <class TC>
Lakegrid<TC>::Lakegrid(unsigned int hSide, unsigned int vSide):hside(hSide-1),vside(vSide-1),center(((double)(hSide)-1)/2,((double)(vSide)-1)/2)
{
	grid=new List<TC>**[hSide];
	for(unsigned int i=0;i<hSide;++i)
	{
		grid[i]=new List<TC>*[vSide];
		for(unsigned int j=0;j<vSide;++j)
		{
			grid[i][j]=new List<TC>;
		}
	}
}

template <class TC>
Lakegrid<TC>::~Lakegrid()
{
	for(unsigned int i=0;i<=hside;++i)
	{
		for(unsigned int j=0;j<=vside;++j)
		{
			delete grid[i][j];
		}
		delete grid[i];
	}
	delete grid;
}

#endif