#ifndef __L5RITEM__
#define __L5RITEM__

#include "L5RGreenCard.hpp"

enum ItemType {KATANA, SPEAR, BOW, NINJATO, WAKIZASHI};

class Item:public GreenCard
{
//Data
public:
	
private:
	int durability;
	const enum ItemType subType;
//Functions
public:
	Item(std::string name, int subType);
	virtual void print(void) const;
	int reduceDurability(void);
	//Reduces durability by 1 point and then 
	//returns the current value of durability.
	int getDurability(void) const;
	enum CardType getType(void) const;
	enum ItemType getSubtype(void) const;
	~Item();
private:
	
};

#endif
