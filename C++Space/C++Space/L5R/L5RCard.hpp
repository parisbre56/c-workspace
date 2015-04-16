#ifndef __L5RCARD__
#define __L5RCARD__

#include <string>

// !! Need card info first
// ?? Shold all of these be pure virtual?
// ?? Do attachments change each personality's attack power or must it be recomputed every time it is required? Should it be cached in a hidden variable? Should there be a function that returns it?

/*
// ?? How to tell cards apart? Make an enum for card types and a function for each? 
// enum L5Rcardtype {L5R_C_UNKNOWNCARD=-1,L5R_C_CARD=1,L5R_C_GREENCARD,L5R_C_ITEM,L5R_C_KATANA,L5R_C_SPEAR,L5R_C_BOW,L5R_C_NINJATO,L5R_C_WAKIZASHI,L5R_C_ITEM_END,L5R_C_FOLLOWER,L5R_C_FOOTSOLDIER,L5R_C_ARCHER,L5R_C_CAVALARY,L5R_C_BUSHIDO,L5R_C_SIEGER,L5R_C_ATAKEBUNE,L5R_C_FOLLOWER_END,L5R_C_GREENCARD_END,L5R_C_BLACKCARD,L5R_C_PERSONALITY,L5R_C_ATTACKER,L5R_C_DEFENDER,L5R_C_CHAMPION,L5R_C_CHANCELLOR,L5R_C_SHOGUN,L5R_C_PERSONALITY_END,L5R_C_PROPERTY,L5R_C_STRONGHOLD,L5R_C_PLAIN,L5R_C_MINE,L5R_C_GOLDMINE,L5R_C_CRYSTALMINE,L5R_C_FARMLAND,L5R_C_GIFTANDFAVOUR,L5R_C_PROPERTY_END,L5R_C_BLACKCARD_END,L5R_C_CARD_END};
// bool isValidCard(L5Rcardtype card){if(card>=L5R_C_CARD&&card<=L5R_C_CARD_END){return true;} return false;}
// bool isGreenCard(L5Rcardtype card){if(card>=L5R_C_GREENCARD&&card<=L5R_C_GREENCARD_END){return true;} return false;}
// bool isFollower(L5Rcardtype card){if(card>=L5R_C_FOLLOWER&&card<=L5R_C_FOLLOWER_END){return true;} return false;}
// bool isBlackCard etc...*/ //<-- Not needed.

enum CardType {PERSONALITY=1,HOLDING,FOLLOWER,ITEM};

class Card
{
//Data
public:
	
private:
	const std::string name;
	const int cost;
	bool isTapped;
	// ?? Maybe make these protected?

//Functions
public:
	Card(std::string& Name, int cost);
	virtual enum CardType getType(void) const = 0;
	virtual void print(void) const =0;
	int getCost(void) const;
	bool getIsTapped(void) const;
	std::string const & getName(void) const;
	void setIsTapped(bool to);
	~Card(void);
private:
	
};

#endif
