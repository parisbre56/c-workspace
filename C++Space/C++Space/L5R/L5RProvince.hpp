#ifndef __L5RProvince__
#define __L5RProvince__

#include "L5RBlackCard.hpp"

class Province
{
//Data
public:
	
private:
	int defence;
	//The defense of the province is equal 
	//to the initialDefense of the Player's stronghold.
	bool isdead;
	BlackCard* card;
//Functions
public:
	Province(int Defence);
	bool isDead(void) const;
	void die(void);
	//Sets isdead to true and 
	//deletes the card inside the province.
	void reveal(void) const;
	//Reveals the BlackCard.
	void print(void) const;
	bool hasCard(void) const;
	BlackCard * getCard(void);
	//Used to access the BlackCard for info.
	BlackCard* removeCard(void);
	//Used to take a card from the province.
	//The card pointer is set to NULL.
	void setCard(BlackCard* source);
	//Used to put a card in the province.
	int getDefence(void) const;
	~Province();
private:
	
};

#endif
