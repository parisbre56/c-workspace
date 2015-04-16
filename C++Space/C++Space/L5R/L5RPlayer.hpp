#ifndef __L5RPlayer__
#define __L5RPlayer__

#include "L5RDeckBuilder.hpp"
#include "L5RProvince.hpp"
#include "L5RStronghold.hpp"

#define __L5RMAXHAND__ 6 //?

class Player
{
//Data
// ?? Need either Name, Id, or both.
// ?? Should the above be private or public?
public:
	
private:
	const short id;
	list<GreenCard* > FateDeck;
	//Player draws a green card from here every turn.
	list<BlackCard*> dynastyDeck;
	//Place cards from here inside provinces
	list<GreenCard*> hand; 
	//Remember, maximum is __L5RMAXHAND__
	list<Holding*> holdings;
	//Every property bought goes here.
	//Contains the player's stronghold on position 1.
	list<Personality*> army;
	//Every personality bought goes here.
	vector<Province*> provinces; 
	//Maybe make this a vector? vector<Province> provinces;
	//Remember, Province is a container for BlackCards
	short numberOfProvinces;
	//This is this player's health.
	//Default value is 4.

//Functions
public:

	Player(int n);	//Constructor. Initialise data as necessary.
	// ** InitialPhaseStarts
	void untapEverything(void);	//Make all cards untapped.

	void print(void);
	void shortprint(){cout << "Player " << id<< endl;}
	bool sorting( Player* dyn1,  Player* dyn2) {
	        return (static_cast<Stronghold*>(*dyn1->holdings.begin()))->getHonour() > 
				(static_cast<Stronghold*>(*dyn2->holdings.begin()))->getHonour();}   


	bool drawFateCard(void);	//Get a Card from FateDeck and put it in dynasty deck.
	void revealProvinces(void);	//Reveal the black card inside each Province.

	void printHand(void);
	void printProvinces(void);

	void equipPhase(void);

	void economyPhase(void);
	//Buy a personality or property from the ones availiable in each province.
	//Put a new BlackCard card in empty provinces from dynastyDeck if any are availiable.
	void discardSurplusFateCards(void);
	// ** This is the finalPhase.
	//If more cards in hand than __L5RMAXHAND__ ask the player to discard (hand.size() - __L5RMAXHAND__) cards.
	void printArena(void);
	// ?? Print info about the army?
	void printHoldings(void);
	//Print info about holdings
	~Player(void);
	//Destructor. Delete data as necessary.
	void initialPhase(void);
	vector<Province*>& getProvinces(void);
	short getId(void) const;
	int get_number_of_provinces(void) const;
	void battlePhase(vector<Player*>&);
};

#endif
