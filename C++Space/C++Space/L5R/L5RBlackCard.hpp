#ifndef __L5RBLACKCARD__
#define __L5RBLACKCARD__

#include "L5RCard.hpp"

class BlackCard:public Card
{
//Data
public:
	
private:
	bool isRevealed;
	//Used when looking at province cards. All BlackCards enter provinces from the dynastyDeck unrevealed
	
//Functions
public:
	BlackCard(std::string& name, int Cost);
	virtual enum CardType getType(void) const =0;
	virtual void print(void) const =0;
	bool getIsRevealed(void) const;
	void reveal(void);
	void hide(void);
	//This should never have to be used
	~BlackCard(void);
private:
		
};

#endif
