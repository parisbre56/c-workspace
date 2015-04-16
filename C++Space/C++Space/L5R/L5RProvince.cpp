#include "L5RProvince.hpp"

#include <iostream>

using namespace std;

Province::Province(int Defence):defence(Defence),isdead(false),card(NULL)
{
	#ifdef __L5R_DEBUG_CERR__
	cerr<<"Creating Province."<<endl;
	#endif
}
bool Province::isDead(void) const
{
	return isdead;
}
void Province::die(void)
{
	this->isdead=true;
	delete card;
	card=NULL;
}
void Province::print(void) const
{
	cout<<"Defence: "<<defence<<endl;
	cout<<"Destroyed: "<<((isDead())?("YES"):("NO"))<<endl;
	cout<<"Contains a card: "<<((card!=NULL)?("YES"):("NO"))<<endl;
	if(card!=NULL)
	{
		cout<<"\t--- Card info follows ---"<<endl;
		card->print();
		cout<<"\t--- Card info ends ---"<<endl;
	}
}
void Province::reveal(void) const
//Reveals the BlackCard.
{
	if(!hasCard())
	{
		cerr<<"No card. Nothing revealed."<<endl;
		return;
	}
	card->reveal();
}
bool Province::hasCard(void) const
{
	return card!=NULL;
}
BlackCard * Province::getCard(void)
//Used to access the BlackCard for info.
{
	return card;
}
BlackCard* Province::removeCard(void)
//Used to take a card from the province.
//The card pointer is set to NULL.
{
	BlackCard* temp = card;
	card=NULL;
	return temp;
}
void Province::setCard(BlackCard* source)
//Used to put a card in the province.
{
	if(hasCard())
	{
		cerr<<"Province already has a card. Nothing added."<<endl;
		return;
	}
	if(isDead())
	{
		cerr<<"Province dead. Nothing added."<<endl;
		return;
	}
	if(source->getIsRevealed())
	{
		source->hide();
	}
	card=source;
}
int Province::getDefence(void) const
{
	return defence;
}
Province::~Province()
{
	if (card!=NULL)
	{
		delete card;
	}
	#ifdef __L5R_DEBUG_CERR__
	cerr<<"Deleting Province."<<endl;
	#endif
}
