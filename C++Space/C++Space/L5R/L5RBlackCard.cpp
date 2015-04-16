#include "L5RBlackCard.hpp"

#include <iostream>

using namespace std;

BlackCard::BlackCard(std::string& name, int Cost):
	Card::Card(name,Cost),isRevealed(false)
{
	#ifdef __L5R_DEBUG_CERR__
	cerr<<"Creating BlackCard with Name:"<<name<<endl;
	#endif
}
bool BlackCard::getIsRevealed(void) const
{
	return isRevealed;
}
void BlackCard::reveal(void)
{
	isRevealed=true;
}
void BlackCard::hide(void)
{
	#ifdef __L5R_DEBUG_CERR__
	cerr<<"Warning: Hiding BlackCard with Name:"<<getName()<<endl;
	#endif
	isRevealed=false;
}
BlackCard::~BlackCard(void)
{
	#ifdef __L5R_DEBUG_CERR__
	cerr<<"Deleting BlackCard with Name:"<<getName()<<endl;
	#endif
}
