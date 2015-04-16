#ifndef __L5RPERSONALITY__
#define __L5RPERSONALITY__

#include "L5RGreenCard.hpp"
#include "L5RBlackCard.hpp"

#include <list>

enum PersonalityType {ATTACKER=1,DEFENDER,SHOGUN,CHANCELLOR,CHAMPION};

class Personality:public BlackCard
{
//Data
public:
	
private:
	const int attack;
	const int defence;
	int honour;
	bool isDead;
	std::list<GreenCard*> attachments;
	//Items and followers
	const int maxAttachments;
	const enum PersonalityType subType;
	
//Functions
public:
	Personality(std::string name, int subType);
	virtual void print(void) const;
	int getAttack(void) const;
	int getDefence(void) const;
	int getHonour(void) const;
	int reduceHonour(void);
	//Returns the honour after it has been reduced.
	void performSeppuku(void);
	bool getIsDead(void) const;
	std::list<GreenCard*>& getAttachments(void);
	enum PersonalityType getSubType(void) const;
	enum CardType getType(void) const;
	int getMaxAttachments(void) const;
	int getAttachmentNum(void) const;
	void attach(GreenCard* source);
	int getActualAttack(void) const;
	int getActualDefence(void) const;
	void battleFought(void);
	//Does everything that needs to be done after a battle
	//It reduces the durability of every object and deletes
	//those that are at 0.
	~Personality(void);
	//Also deletes all GreenCards inside it.
private:
		
};

#endif
