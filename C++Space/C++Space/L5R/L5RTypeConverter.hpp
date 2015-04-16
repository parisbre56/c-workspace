#ifndef _TYPECONVERTER_HPP_
#define _TYPECONVERTER_HPP_

#include "L5RPersonality.hpp"
#include "L5RHolding.hpp"
#include "L5RFollower.hpp"
#include "L5RItem.hpp"

class TypeConverter
{
public:
	TypeConverter(); 
	//Not Implemented.
	void getCorrectType(BlackCard* card, Personality **person, Holding **hold);
	void getCorrectType(GreenCard* card, Follower **follow, Item **item);
	Personality * getPersonality(BlackCard *d);
	Holding * getHolding(BlackCard *d);
	Follower *getFollower(GreenCard *d);
	Item *getItem(GreenCard *d);
};

#endif
