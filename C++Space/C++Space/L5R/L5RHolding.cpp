#include "L5RHolding.hpp"

#include <iostream>

using namespace std;

namespace L5RHoldingNamespace
{
	int data[2][7] = {
	/* Cost 	*/	{2,	5,	7,	12,	3,	2,	0},
	/* Harv.Val.*/	{2,	3,	5,	6,	4,	2,	5}};
}

using namespace L5RHoldingNamespace;

Holding::Holding(std::string name, int SubType):BlackCard(name,data[0][SubType-1]),
	harvestValue(data[1][SubType-1]),upperHolding(NULL),subHolding(NULL),subType((enum HoldingType)SubType)
{
	#ifdef __L5R_DEBUG_CERR__
	cerr<<"Creating Holding with Name: "<<getName()<<endl;
	#endif
}
void Holding::print(void) const
{
	if(!getIsRevealed())
	{
		cout<<"Hidden BlackCard"<<endl;
		return;
	}
	cout<<"Property"<<endl;
	cout<<"Name: "<<getName()<<endl;
	cout<<"Cost: "<<getCost()<<endl;
	cout<<"Tapped: "<<((getIsTapped())?("YES"):("NO"))<<endl;
	cout<<"Harv.Val.: "<<getHarvestValue()<<endl;
	cout<<"Harv.Val + Bonuses: "<<getActualHarvestValue()<<endl;
	cout<<"Linked Upper: "<<((hasUpperHolding())?("YES"):("NO"))<<endl;
	cout<<"Linked Sub: "<<((hasSubHolding())?("YES"):("NO"))<<endl;
}
int Holding::getHarvestValue(void) const
{
	return harvestValue;
}
int Holding::getActualHarvestValue(void) const
{
	if(hasSubHolding()&&hasUpperHolding())
	{
		if(subHolding->getSubType()==MINE && subType==GOLD_MINE && upperHolding->getSubType()==CRYSTAL_MINE)
		{
			return harvestValue*2;
		}
	}
	else if(hasSubHolding())
	{
		if(subType==CRYSTAL_MINE)
		{
			if(subHolding->getSubType()==GOLD_MINE)
			{
				if(subHolding->hasSubHolding())
				{
					if(subHolding->getSubHolding()->getSubType()==MINE)
					{
						return harvestValue*4;
					}
				}
				else
				{
					return harvestValue*2;
				}
			}
		}
		else if(subType==GOLD_MINE)
		{
			if(subHolding->getSubType()==MINE)
			{
				return harvestValue+4;
			}
		}
	}
	else if(hasUpperHolding())
	{
		if(subType==GOLD_MINE)
		{
			if(upperHolding->getSubType()==CRYSTAL_MINE)
			{
				return harvestValue+5;
			}
		}
		else if(subType==MINE)
		{
			if(upperHolding->getSubType()==GOLD_MINE)
			{
				return harvestValue+2;
			}
		}
	}
	return harvestValue;
}
bool Holding::hasUpperHolding(void) const
{
	return upperHolding!=NULL;
}
bool Holding::hasSubHolding(void) const
{
	return subHolding!=NULL;
}
Holding* Holding::getUpperHolding(void)
{
	return upperHolding;
}
Holding* Holding::getSubHolding(void)
{
	return subHolding;
}
void Holding::setUpperHolding(Holding* source)
{
	if(hasUpperHolding())
	{
		if(upperHolding==source)
		{
			return;
		}
		else
		{
			cout<<"Unable to bond provinces. UpperProvince with Name: "<<
			getName()<<" is already linked with different card.";
			return;
		}
	}
	if(source->hasSubHolding())
	{
		if(source->getSubHolding()==this)
		{
			upperHolding=source;
			return;
		}
		else
		{
			cout<<"Unable to bond provinces. SubProvince with Name: "<<
			source->getName()<<" is already linked with different card.";
			return;
		}
	}
	if(subType==MINE && source->getSubType()==GOLD_MINE)
	{
		upperHolding=source;
		source->setSubHolding(this);
		return;
	}
	if(subType==GOLD_MINE && source->getSubType()==CRYSTAL_MINE)
	{
		upperHolding=source;
		source->setSubHolding(this);
		return;
	}
	cout<<"Incompatible card types. Cannot form bond between subProvince with Name: "<<
	getName()<<" and upperProvince with Name: "<<source->getName()<<endl;
	return;
}
void Holding::setSubHolding(Holding* source)
{
	if(hasSubHolding())
	{
		if(subHolding==source)
		{
			return;
		}
		else
		{
			cout<<"Unable to bond provinces. UpperProvince with Name: "<<
			getName()<<" is already linked with different card.";
			return;
		}
	}
	if(source->hasUpperHolding())
	{
		if(source->getUpperHolding()==this)
		{
			subHolding=source;
			return;
		}
		else
		{
			cout<<"Unable to bond provinces. SubProvince with Name: "<<
			source->getName()<<" is already linked with different card.";
			return;
		}
	}
	if(subType==CRYSTAL_MINE && source->getSubType()==GOLD_MINE)
	{
		subHolding=source;
		source->setUpperHolding(this);
		return;
	}
	if(subType==GOLD_MINE && source->getSubType()==MINE)
	{
		subHolding=source;
		source->setUpperHolding(this);
		return;
	}
	cout<<"Incompatible card types. Cannot form bond between subProvince with Name: "<<
	source->getName()<<" and upperProvince with Name: "<<getName()<<endl;
	return;
}
enum CardType Holding::getType(void) const
{
	return HOLDING;
}
enum HoldingType Holding::getSubType(void) const
{
	return subType;
}
Holding::~Holding(void)
{
	#ifdef __L5R_DEBUG_CERR__
	cerr<<"Deleting Holding with Name: "<<getName()<<endl;
	#endif
}
