#ifndef __L5RHOLDING__
#define __L5RHOLDING__

#include "L5RBlackCard.hpp"

enum HoldingType {PLAIN=1, MINE, GOLD_MINE, CRYSTAL_MINE, FARMS, SOLO, STRONGHOLD};

class Holding:public BlackCard
{
//Data
public:
	
private:
	int harvestValue;
	Holding* upperHolding;
	Holding* subHolding;
	enum HoldingType subType;
	
//Functions
public:
	Holding(std::string name, int subType);
	virtual void print(void) const;
	int getHarvestValue(void) const;
	int getActualHarvestValue(void) const;
	//Returns the harvest value after 
	//it's been modified by sub and upper holdings.
	bool hasUpperHolding(void) const;
	bool hasSubHolding(void) const;
	Holding* getUpperHolding(void);
	Holding* getSubHolding(void);
	void setUpperHolding(Holding* source);
	//Also sets the other province's subHolding;
	void setSubHolding(Holding* source);
	//Also sets the other province's upperHolding.
	enum CardType getType(void) const;
	enum HoldingType getSubType(void) const;
	~Holding(void);
private:
		
};

#endif
