#include "L5RStronghold.hpp"

#include <cstdlib>
#include <iostream>

using namespace std;

Stronghold::Stronghold(string name,int subType = STRONGHOLD):
	Holding(name,STRONGHOLD),
	honour(__L5R_S_MINHONOUR__+rand()%(__L5R_S_MAXHONOUR__-__L5R_S_MINHONOUR__+1)),
	initialDefence(__L5R_S_MININITD__+rand()%(__L5R_S_MAXINITD__-__L5R_S_MININITD__+1))
{
	this->reveal();
	#ifdef __L5R_DEBUG_CERR__
	cerr<<"Creating Stronghold with Name: "<<name<<endl;
	#endif
}

void Stronghold::print(void) const
{
	if(!getIsRevealed())
	{
		cout<<"Hidden BlackCard"<<endl;
		return;
	}
	cout<<"Stronghold"<<endl;
	cout<<"Name: "<<getName()<<endl;
	cout<<"Cost: "<<getCost()<<endl;
	cout<<"Tapped: "<<((getIsTapped())?("YES"):("NO"))<<endl;
	cout<<"Harv.Val.: "<<getHarvestValue()<<endl;
	cout<<"Harv.Val + Bonuses: "<<getActualHarvestValue()<<endl;
	cout<<"Linked Upper: "<<((hasUpperHolding())?("YES"):("NO"))<<endl;
	cout<<"Linked Sub: "<<((hasSubHolding())?("YES"):("NO"))<<endl;
	cout<<"Honour: "<<getHonour()<<endl;
	cout<<"Defence: "<<getInitialDefence()<<endl;
}

int Stronghold::getHonour(void) const
{
	return honour;
}


int Stronghold::getInitialDefence(void) const
{
	return initialDefence;
}

Stronghold::~Stronghold(void)
{
	#ifdef __L5R_DEBUG_CERR__
	cerr<<"Deleting Stronghold with Name: "<<getName()<<endl;
	#endif
}
