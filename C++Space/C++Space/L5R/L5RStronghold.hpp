#ifndef __L5RSTRONGHOLD__
#define __L5RSTRONGHOLD__

#include "L5RHolding.hpp"

#define __L5R_S_MAXHONOUR__ 20 
//The maximum honour each player can have
#define __L5R_S_MINHONOUR__ 0
//The minimum honour each player can have
#define __L5R_S_MAXINITD__ 10
//The maximum initial defence a player can have
#define __L5R_S_MININITD__ 5
//The minimum initial defence a player can have

class Stronghold:public Holding
{
//Data
public:
	
private:
	const int honour;
	const int initialDefence;
//Functions
public:
	Stronghold(std::string name, int SubType);
	virtual void print(void) const;
	int getHonour(void) const;
	int getInitialDefence(void) const;
	~Stronghold(void);
private:
	
};

#endif
