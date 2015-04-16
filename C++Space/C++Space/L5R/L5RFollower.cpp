#include "L5RFollower.hpp"

#include <iostream>

using namespace std;

namespace L5RFollowerNamespace
{
	int data[6][6] = {
	/* Cost 	*/	{0,	0,	5,	3,	3,	8},
	/* Attack 	*/	{2,	0,	3,	4,	2,	8},
	/* Defence 	*/	{0,	2,	3,	2,	4,	8},
	/* Min.Honor*/	{1,	1,	2,	3,	3,	6},
	/* Eff.Bonus*/	{1,	1,	2,	3,	3,	3},
	/* Eff.Cost */	{2,	2,	3,	4,	4,	8}};

	std::string text[6] = {"The fools who are the first sent to die.",
							"The cowards who fight from afar.",
							"The guys who don't know how to use doors.",
							"The guys who like riding stallions.",
							"A fishing boat with a couple of cannons strapped on it.",
							"Knights in multicoloured armor. They follow the code of the warrior."};
}

using namespace L5RFollowerNamespace;

Follower::Follower(std::string name, int SubType):
	GreenCard(name, data[0][SubType],data[5][SubType],
		data[4][SubType],text[SubType],data[3][SubType],data[2][SubType],
		data[1][SubType]),
	subType((enum FollowerType)SubType)
{
	#ifdef __L5R_DEBUG_CERR__
	cerr<<"Creating Follower with Name: "<<name<<endl;
	#endif
}
void Follower::print(void) const
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
	cout<<"Card Text: "<<getCardText()<<endl;
}
enum CardType Follower::getType(void) const
{
	return FOLLOWER;
}
enum FollowerType Follower::getSubtype(void) const
{
	return subType;
}
Follower::~Follower(void)
{
	#ifdef __L5R_DEBUG_CERR__
	cerr<<"Deleting Follower with Name: "<<getName()<<endl;
	#endif
}
