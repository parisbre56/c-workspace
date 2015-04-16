#include "L5RCard.hpp"

#include <iostream>

using namespace std;

Card::Card(std::string& Name, int Cost):name(Name),cost(Cost),isTapped(false)
{
	#ifdef __L5R_DEBUG_CERR__
	cerr<<"Creating card with Name:"<<Name<<endl;
	#endif
}

int Card::getCost(void) const
{
	return cost;
}

bool Card::getIsTapped(void) const
{
	return isTapped;
}

std::string const & Card::getName(void) const
{
	return name;
}

void Card::setIsTapped(bool to)
{
	isTapped=to;
}

Card::~Card(void)
{
	#ifdef __L5R_DEBUG_CERR__
	cerr<<"Deleting card with Name:"<<name<<endl;
	#endif
}
