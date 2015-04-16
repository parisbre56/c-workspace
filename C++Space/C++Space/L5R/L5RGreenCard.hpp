#ifndef __L5RGREENCARD__
#define __L5RGREENCARD__

#include "L5RCard.hpp"

#include <string>

class Personality;

class GreenCard : public Card
{
//Data
public:
	
private:
	bool effectActive;
	//True if player payed the effect Cost when buying the card.
	const int effectCost;
	const int effectBonus;
	const std::string cardText;
	const int minimumHonour;
	const int defenceBonus;
	const int attackBonus;
	Personality* attached;
	
//Functions
public:
	GreenCard(std::string& name, int Cost, 
		int EffectCost, int EffectBonus, std::string& CardText, 
		int MinimumHonour, int DefenceBonus, int AttackBonus);
	virtual enum CardType getType(void) const = 0;
	virtual void print(void) const =0;
	bool getEffectActive(void) const;
	bool isAttached(void) const;
	int getEffectCost(void) const;
	int getEffectBonus(void) const;
	std::string const & getCardText(void) const;
	int getMinimumHonour(void) const;
	int getDefenceBonus(void) const;
	int getAttackBonus(void) const;
	void attach(Personality* target);
	//Sets attached to <target>
	void detach(void);
	//Sets attached to NULL
	Personality* getAttached(void);
	~GreenCard(void);
private:
	
};


#endif
