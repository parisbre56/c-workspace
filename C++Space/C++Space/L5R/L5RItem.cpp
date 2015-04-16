#include "L5RItem.hpp"

#include <iostream>

using namespace std;

namespace L5RItemNamespace
{
	int data[7][5] = {
	/* Cost 	*/	{5,	5,	10,	15,	20},
	/* Attack 	*/	{3,	2,	4,	6,	8},
	/* Defence 	*/	{3,	2,	4,	6,	8},
	/* Min.Honor*/	{1,	1,	2,	3,	6},
	/* Eff.Bonus*/	{5,	4,	4,	6,	8},
	/* Eff.Cost */	{6,	6,	10,	10,	15},
	/* Durab.	*/	{4,	4,	6,	8,	10}};

	std::string text[5] = {"This is a sword. It's for cutting things. Who would have thought?",
							"This is a spear. It's for stabbing things. Perfect for making \"I'm thrusting my spear!\" jokes.",
							"This is a bow. You can be boring and just shoot stuff with it OR you can use it to bash people in the head like a dwarf.",
							"This is a ninja sword, the ancestor of the landmine. Simply leave these swords pointing upright in the battlefield. Since ninjas are invisible, the enemy will fall into them and die. Careful however: do not use too many of them or the law of inverse ninjas will make them powerless.",
							"This is a side sword. Do not mistake it with a side dish. Eating this is NOT good for you."};
}
using namespace L5RItemNamespace;

Item::Item(std::string name, int SubType):
	GreenCard(name, data[0][SubType],data[5][SubType],
		data[4][SubType],text[SubType],data[3][SubType],data[2][SubType],
		data[1][SubType]),
	durability(data[6][SubType]),subType((enum ItemType)SubType) 
	//Assuming that subType will always be set correctly by DeckBuilder
{
	#ifdef __L5R_DEBUG_CERR__
	cerr<<"Creating Item with Name: "<<name<<endl;
	#endif
}
void Item::print(void) const
{
	cout<<"Item"<<endl;
	cout<<"Name: "<<getName()<<endl;
	cout<<"Cost: "<<getCost()<<endl;
	cout<<"Minimum Honour: "<<getMinimumHonour()<<endl;
	cout<<"Tapped: "<<((getIsTapped())?("YES"):("NO"))<<endl;
	cout<<"Attack Bonus: "<<getAttackBonus()<<endl;
	cout<<"Defence Bonus: "<<getDefenceBonus()<<endl;
	cout<<"Effect Cost: "<<getEffectCost()<<endl;
	cout<<"Effect Bonus: "<<getEffectBonus()<<endl;
	cout<<"Effect Active: "<<((getEffectActive())?("YES"):("NO"))<<endl;
	cout<<"Durability: "<<getDurability()<<endl;
	cout<<"Card Text: "<<getCardText()<<endl;
}
int Item::reduceDurability(void)
{
	#ifdef __L5R_DEBUG_CERR__
	cerr<<"Reducing durability of Item with Name: "<<getName()<<endl;
	if(durability<=0)
	{
		cerr<<"Item with Name: "<<getName()<<" has negative durability."<<endl;
	}
	#endif
	if (--durability == 0)
	{
		cout<<"Item with Name: "<<getName()<<" has been damaged beyond repair."<<endl;
	}
	return durability;
}
int Item::getDurability(void) const
{
	return durability;
}
enum CardType Item::getType(void) const
{
	return ITEM;
}
enum ItemType Item::getSubtype(void) const
{
	return subType;
}
Item::~Item()
{
	#ifdef __L5R_DEBUG_CERR__
	cerr<<"Deleting Item with Name: "<<getName()<<endl;
	#endif
}
