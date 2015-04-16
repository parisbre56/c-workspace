#ifndef __L5RFOLLOWER__
#define __L5RFOLLOWER__

#include "L5RGreenCard.hpp"

#include <string>

enum FollowerType {FOOTSOLDIER, ARCHER, SIEGER, CAVALRY, NAVAL, BUSHIDO};

class Follower:public GreenCard
{
//Data
public:
	
private:
	enum FollowerType const subType;
//Functions
public:
	Follower(std::string name, int subType);
	enum CardType getType(void) const;
	enum FollowerType getSubtype(void) const;
	virtual void print(void) const;
	~Follower(void);
private:
	
};

#endif
