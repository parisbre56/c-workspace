#include "L5RGreenCard.hpp"

#include <iostream>
#include <list>

using namespace std;

GreenCard::GreenCard(std::string& Name, int Cost, 
	int EffectCost, int EffectBonus, std::string& CardText, 
	int MinimumHonour, int DefenceBonus, int AttackBonus):
		Card(Name,Cost),effectActive(false),effectCost(EffectCost),
		effectBonus(EffectBonus),cardText(CardText),minimumHonour(MinimumHonour),
		defenceBonus(DefenceBonus),attackBonus(AttackBonus),attached(NULL)
{
	#ifdef __L5R_DEBUG_CERR__
	cerr<<"Creating GreenCard with Name:"<<getName()<<endl;
	#endif
}
bool GreenCard::getEffectActive(void) const
{
	return effectActive;
}
bool GreenCard::isAttached(void) const
{
	return attached!=NULL;
}
int GreenCard::getEffectCost(void) const
{
	return effectCost;
}
int GreenCard::getEffectBonus(void) const
{
	return effectBonus;
}
std::string const & GreenCard::getCardText(void) const
{
	return cardText;
}
int GreenCard::getMinimumHonour(void) const
{
	return minimumHonour;
}
int GreenCard::getDefenceBonus(void) const
{
	return defenceBonus;
}
int GreenCard::getAttackBonus(void) const
{
	return attackBonus;
}
void GreenCard::attach(Personality* target)
{
	attached=target;
	#ifdef __L5R_DEBUG_CERR__
	cerr<<"Attaching GreenCard with Name:"<<getName()<<endl;
	#endif
}
void GreenCard::detach(void)
{
	#ifdef __L5R_DEBUG_CERR__
	cerr<<"Detaching GreenCard with Name:"<<getName()<<endl;
	#endif
	attached=NULL;
}

Personality* GreenCard::getAttached(void)
{
	return attached;
}
GreenCard::~GreenCard(void)
{
	#ifdef __L5R_DEBUG_CERR__
	cerr<<"Deleting GreenCard with Name:"<<getName()<<endl;
	#endif
}
