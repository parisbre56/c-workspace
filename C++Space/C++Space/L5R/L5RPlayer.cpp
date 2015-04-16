#include "L5RPlayer.hpp"
#include "L5RStronghold.hpp"
#include "L5RTypeConverter.hpp"


Player::Player(int n): id(n){
	cout << "Creating player -dynast number " << n<< endl;
	numberOfProvinces=4;
	DeckBuilder Deck;
		list<GreenCard*>* temp1 = Deck.createFateDeck();
    	FateDeck = *temp1;
		delete temp1;
		temp1 = NULL;
		list<BlackCard*>* temp2 = Deck.createDynastyDeck();
    	dynastyDeck = *temp2;
		delete temp2;
		temp2=NULL;
    	Deck.deckShuffler(&FateDeck);
    	Deck.deckShuffler(&dynastyDeck);  
	while ( hand.size() < 6)
		if (!drawFateCard())
			break;
			
	holdings.push_back(new Stronghold("strong", STRONGHOLD));
	
	int initialDefence = (static_cast<Stronghold*>(holdings.front()))->getInitialDefence();

	while ( provinces.size() < numberOfProvinces)
		provinces.push_back(new Province(initialDefence));

	for (int i=0; i < numberOfProvinces; i++)
		if(!dynastyDeck.empty()){
 			provinces[i]->setCard(dynastyDeck.front());
			dynastyDeck.pop_front();}
		else provinces[i]->setCard(NULL);

}

Player::~Player(){
	cout << "player number " << id << " is about to leave" << endl;
	for(list<Holding*>::iterator it = holdings.begin(); it != holdings.end() ; ++it)
	{
		delete (*it);
	} 
	holdings.clear();

	for(list<Personality*>::const_iterator it = army.begin(); it!= army.end() ; it++)
	{
		delete *it;
	} 
	army.clear();
	
	for(list<GreenCard*>::const_iterator it = hand.begin(); it!=hand.end() ; it++)
	{
		delete *it;
	}

	hand.clear();

	for(list<GreenCard*>::const_iterator it = FateDeck.begin(); it != FateDeck.end() ; it++)
	{
		delete *it;
	}
	FateDeck.clear();

	for(list<BlackCard*>::const_iterator it = dynastyDeck.begin(); it!=dynastyDeck.end() ; it++)
	{
		delete *it;
	}
	dynastyDeck.clear();

	for(vector<Province*>::iterator it = provinces.begin(); it!= provinces.end();it++)
	{
		delete *it;
	}
	provinces.clear();
}



bool Player::drawFateCard(void){
	//Get a Card from FateDeck and put it in hand.
	 if(!FateDeck.empty())
	 {
	 		hand.push_front(FateDeck.front());
	 		FateDeck.pop_front();
			return 1;
	 }
	 else return 0;
}

void Player::untapEverything(void){
	for (list<Holding*>::iterator i = holdings.begin(); i != holdings.end(); i++)
		(*i)->setIsTapped(0); // carefull it untaps Stronghold

	for (list<Personality*>::iterator j = army.begin(); j != army.end(); j++)
		(*j)->setIsTapped(0);
}

void Player::revealProvinces(void){
	for (vector<Province*>::iterator it = provinces.begin(); it != provinces.end(); it++)
		if (!(*it)->isDead())
			(*it)->reveal();
}

void Player::printHand(void){
	cout << "Cards in hand for Player " << id << " are: " << endl;
	for (list<GreenCard*>::iterator it = hand.begin(); it != hand.end(); it++)
		cout << (*it)->getName() << endl;
	cout << endl;
}

void Player::printProvinces(void){
	cout << "Remaining Provinces for Player " << id << " are: " << endl;
	int i=1;
	for (vector<Province*>::iterator it = provinces.begin(); it != provinces.end(); it++){
		if ( !(*it)->isDead()){
			cout << "Province " << i << endl;}
		i++;}
	cout << endl;
}

void Player::print(void){
	cout << "player " << id << " : "  <<numberOfProvinces  << " provinces remaining" << endl;
	cout << "Honour is " << static_cast<Stronghold*>(holdings.front())->getHonour() << endl;
	cout << "dynastyDeck : " << dynastyDeck.size() << " cards remaining" << endl;
	cout << "FateDeck : " << FateDeck.size() << " cards remaining" << endl;
	printHand();
	printArena();
	printHoldings();
	printProvinces();
	cout << endl;
}

void Player::printArena(void){
	cout << "Current army for Player " << id << " is: " << endl;
	for (list<Personality*>::iterator it = army.begin(); it != army.end(); it++) // cout endl;
		cout << (*it)->getName() << endl;
	cout << endl;
}

void Player::printHoldings(void){
	cout << "Current Holdings for Player " << id << " are: " << endl;
	//if they are connected must they be printed in a row?
	for (list<Holding*>::iterator it = holdings.begin(); it != holdings.end(); it++) // cout endl;
		cout << (*it)->getName() << endl; // carefull it prints Stronghold too
	cout << endl;
}

void Player::initialPhase(void){
	untapEverything();
	if (hand.size() < 6)
		drawFateCard();
	revealProvinces();
	printHand();
	printProvinces();
}

void Player::discardSurplusFateCards(void){
	list<GreenCard*>::iterator it; 
	while ( hand.size() > 6){
		it=  hand.begin();
		advance( it, rand() % hand.size());
		delete (*it);
		it=hand.erase(it);
	}
}


void Player::economyPhase(void){
	revealProvinces();
	int money=0, i=1, x;
	list<Holding*>::iterator it;
	vector<Province*>::iterator it1;
	bool f=false;
	for (it1 = provinces.begin(); it1 != provinces.end(); it1++){
		if ( !(*it1)->isDead()){
			cout << "Province " << i << " has card : "<< endl;
			if ( (*it1)->getCard() == NULL)
				cout << "No card" << endl;
			else 	{(*it1)->getCard()->print(); f= true;}
			cout << endl;
		}
		i++;
	}

	if (!f)	// no other cards left
		return;

	cout << "Current Money : "<< money << endl;
	cout << "Current Holdings are : " << endl;
	cout << "Number		Name 		Condition		Harvest_Value" << endl;
	i=1;

	for (it = holdings.begin(); it != holdings.end(); it++){
		cout << i << "\t\t"<< (*it)->getName() << "\t\t" << ((*it)->getIsTapped()? "Tapped" : "Untapped");
		cout << "\t\t" << (*it)->getActualHarvestValue()  << endl ;
		i++; }
	i--;

	cout << "Current Money : "<< money << endl;

	it = holdings.begin();

	while ( (*it)->getIsTapped() )	// check for possible holdings
		if (++it == holdings.end())
			break;

	if (it == holdings.end())
		f=false;


	while (f){

		cout <<endl <<"select holding from 1 to " << i <<" to untap /0 for exit" <<endl;
		cin >> x; 
		if (x==0)
			break;
		else if ( x > 0 && x <= i){
			it = holdings.begin();
			advance(it , x-1);
			if ( (*it)->getIsTapped() ){
				cout << "Naughty boy: card already tapped"<< endl;
				continue;}
			(*it)->setIsTapped(1);
			cout << x << "\t\t"<< (*it)->getName() << "\t\t" << ((*it)->getIsTapped()? "Tapped" : "Untapped");
			cout << "\t\t" << (*it)->getActualHarvestValue()  << endl ; 
			money += (*it)->getActualHarvestValue();
			cout << "Current Money : "<< money << endl;

			it = holdings.begin();
			while ( (*it)->getIsTapped() )	// check for holdings left to buy
				if (++it == holdings.end())
					break;

			if (it == holdings.end())
				break;
		}
			
	}

	f=false;
	for (i=0; i< 4; i++)
		if ( !provinces[i]->isDead() && provinces[i]->getCard() != NULL )
			if (provinces[i]->getCard()->getCost() <= money){
				f=true; 
				break;}

	while (f){
		cout << "Current Money : "<< money << endl;
		cout << endl <<"Select province from above to buy card if possible /0 for exit" << endl;
		cin >> x;
		if (x==0)
			break;
		else if ( x > 0 && x <= 4){
			if (provinces[x-1]->isDead() || provinces[x-1]->getCard() == NULL){
				cout << "No cards here"<< endl;
				continue;}
				
			if (provinces[x-1]->getCard()->getCost() > money || !provinces[x-1]->getCard()->getIsRevealed() ){
				cout << "Naughty boy: cannot buy that card!"<< endl;
				continue;}

			money -= provinces[x-1]->getCard()->getCost();

			BlackCard* temp=  provinces[x-1]->removeCard();
			if(!dynastyDeck.empty()){
 				provinces[x-1]->setCard(dynastyDeck.front());
				dynastyDeck.pop_front();}
			else provinces[x-1]->setCard(NULL);

			f=false;
			for (i=0; i< 4; i++)
				if ( !provinces[i]->isDead() && provinces[i]->getCard() != NULL )
				if ( provinces[i]->getCard()->getIsRevealed() && provinces[i]->getCard()->getCost() <= money){
						f=true; 
						break;}
		
			if ( temp->getType() == PERSONALITY){
				army.push_back(static_cast<Personality*> (temp));	continue;}

			Holding* p = static_cast<Holding*> (temp);
			it= holdings.begin();
			switch ( p->getSubType()){
				case 2:
					while ( it != holdings.end() ){
						if ( (*it)->getSubType() == 3 && (*it)->getSubHolding() == NULL){
							p->setUpperHolding(*it);
							(*it)->setSubHolding(p);
							break;}
						it++;}
					break;
				case 4:
					while ( it != holdings.end() ){
						if ( (*it)->getSubType() == 3 && (*it)->getUpperHolding() == NULL){
							p->setSubHolding(*it);
							(*it)->setUpperHolding(p);
							break;}
						it++;}
					break;
						
				case 3:
					while ( it != holdings.end() ){
						if ( (*it)->getSubType() == 4 && (*it)->getSubHolding() == NULL){
							p->setUpperHolding(*it);
							(*it)->setSubHolding(p);
							break;}
						it++;}

					it=holdings.begin();
					while ( it != holdings.end() ){
						if ( (*it)->getSubType() == 2 && (*it)->getUpperHolding() == NULL){
							p->setSubHolding(*it);
							(*it)->setUpperHolding(p);
							break;}
						it++;}
			}

			holdings.push_back(p);
		}
	}
	cout << "End of economy Phase" << endl << endl;
}

                                             
void Player::equipPhase(void)
{
	list<Holding*> temphold;
	list<Personality*>::const_iterator arit;
	list<GreenCard*>::iterator grit;
	list<Holding*>::iterator hoit;
	Personality* arp=NULL;
	GreenCard* grp=NULL;
	Holding* hop=NULL;
	int arch=0;
	int grch=0;
	int hoch=0;
	bool bonusOn=false;
	bool bought=false;
	bool con_army=false;
	bool con_hold=true;
	int cost=0;
	
	cout<<"Equip phase begins for player "<<id<<endl;
	
	if(army.size()==0)
	{
		cout<<"No army. Nothing to do.";
		return;
	}
	
	con_army=false;
	for(arit=army.begin();arit!=army.end();++arit)
	{
		if((*arit)->getAttachmentNum()<(*arit)->getMaxAttachments())
		{
			con_army=true;
		}
	}
	if(con_army==false)
	{
		cout<<"No empty attachment slots in army. Nothing to do."<<endl;
		return;
	}
	
	while(con_army)
	{
		cout<<"Select a unit to equip (-2 for card info, -1 to stop selecting) [ -2 -1 ";
		arit=army.begin();
		arch=0;
		while(arit!=army.end())
		//Print all availiable units.
		{
			if((!((*arit)->getIsDead())) && (*arit)->getAttachmentNum()<(*arit)->getMaxAttachments())
			{
				cout<<arch<<' ';
			}
			++arit;
			++arch;
		}
		cout<<"]: ";
		
		while(true)
		//Force the player to make a correct choice
		{
			cin>>arch;
			if(arch==-2)
			{
				cout<<"Info for army unit: ";
				while(true)
				{
					cin>>arch;
					if(arch<0||arch>army.size()-1)
					{
						cout<<"That is not a valid army id. Please try again: ";
						continue;
					}
					arit=army.begin();
					advance(arit,arch);
					(*arit)->print();
					break;
				}
				cout<<"Select a unit to equip (-2 for card info, -1 to stop selecting) [ -2 -1 ";
				arit=army.begin();
				arch=0;
				while(arit!=army.end())
				//Print all availiable units.
				{
					if((!((*arit)->getIsDead())) && (*arit)->getAttachmentNum()<(*arit)->getMaxAttachments())
					{
						cout<<arch<<' ';
					}
					++arit;
					++arch;
				}
				cout<<"]: ";
				continue;
			}
			if(arch==-1)
			{
				cout<<"Equip phase ends for player "<<id<<endl;
				return;
			}
			if(arch<0||arch>army.size()-1)
			{
				cout<<"That is not a valid army id. Please try again: ";
				continue;
			}
			arit=army.begin();
			advance(arit,arch);
			if((*arit)->getIsDead())
			{
				cout<<"That personality is dead. You COULD make some sort of scarecraw out of it and hope the enemy falls on it but I'm not gonna let you. Try again: ";
				continue;
			}
			if((*arit)->getAttachmentNum()>=(*arit)->getMaxAttachments())
			{
				cout<<"That personallity is stuffed with items and followers. There's really nothing more you can fit in it. Try again: ";
				continue;
			}
			break;
		}
		arp=(*arit);
		//arp now holds the chosen one of the Army
		
		//Print all possible cards
		cout<<"Select a card for the unit (-2 for card info) [ -2 ";
		for(grit=hand.begin(),grch=0;grit!=hand.end();++grit,++grch)
		{
			cout<<grch<<' ';
		}
		cout<<"]: ";
		
		while (true)
		{
			cin>>grch;
			if(grch==-2)
			{
				cout<<"Info for Fate card: ";
				while(true)
				{
					cin>>grch;
					if(grch<0||grch>hand.size()-1)
					{
						cout<<"That is not a valid card id. Please try again: ";
						continue;
					}
					grit=hand.begin();
					advance(grit,grch);
					(*grit)->print();
					break;
				}
				cout<<"Select a card for the unit (-2 for card info) [ -2 ";
				for(grit=hand.begin(),grch=0;grit!=hand.end();++grit,++grch)
				{
					cout<<grch<<' ';
				}
				cout<<"]: ";
				continue;
			}
			if(grch<0||grch>hand.size()-1)
			{
				cout<<"That is not a valid card id. Please try again: ";
				continue;
			}
			grit=hand.begin();
			advance(grit,grch);
			break;
		}
		grp=(*grit);
		//grp now holds the chosen one of the greenCards.
		
		bonusOn=false;
		cout<<"This card costs "<<grp->getCost()<<endl;
		cout<<"Do you wish to activate its bonus for "<<grp->getEffectCost()<<"(Y/N)? ";
		while(true)
		{
			char c;
			c=cin.get();
			if(c=='Y'||c=='y')
			{
				bonusOn=true;
				break;
			}
			else if(c=='N'||c=='n')
			{
				bonusOn=false;
				break;
			}
			else
			{
				cout<<"Type either Y or N: ";
				continue;
			}
		}
		//bonusOn is true if the player wants the bonus.
		
		cost=grp->getCost()+grp->getEffectCost()*bonusOn;
		//cost now holds the total cost of the card.
		
		bought=false;
		con_hold=true;
		while(con_hold)
		{
			if(cost<=0)
			{
				cout<<"Card costs nothing. Purchase complete.";
				bought=true;
				break;
			}
			
			con_hold=false;
			for(hoit=holdings.begin();hoit!=holdings.end();++hoit)
			{
				if(!((*hoit)->getIsTapped()))
				{
					con_hold=true;
					break;
				}
			}
			if(con_hold==false)
			{
				cout<<"Insufficient funds."<<endl;
				bought=false;
				break;
			}
			
			cout<<"Please select a holding to fund your purchase (-2 for card info) [ -2 ";
			for(hoit=holdings.begin(),hoch=0;hoit!=holdings.end();++hoit,++hoch)
			{
				if(!(*hoit)->getIsTapped())
				{
					cout<<hoch<<' ';
				}
			}
			cout<<"]: ";
			
			while (true)
			{
				cin>>hoch;
				if(hoch==-2)
				{
					cout<<"Info for holding: ";
					while(true)
					{
						cin>>hoch;
						if(hoch<0||hoch>holdings.size()-1)
						{
							cout<<"That is not a valid holding id. Please try again: ";
							continue;
						}
						hoit=holdings.begin();
						advance(hoit,hoch);
						(*hoit)->print();
						break;
					}
					cout<<"Please select a holding to fund your purchase (-2 for card info) [ -2 ";
					for(hoit=holdings.begin(),hoch=0;hoit!=holdings.end();++hoit,++hoch)
					{
						if(!(*hoit)->getIsTapped())
						{
							cout<<hoch<<' ';
						}
					}
					cout<<"]: ";
					continue;
				}
				if(hoch<0||hoch>holdings.size()-1)
				{
					cout<<"That is not a valid holding id. Please try again: ";
					continue;
				}
				hoit=holdings.begin();
				advance(hoit,hoch);
				if((*hoit)->getIsTapped())
				{
					cout<<"That card has already been used to buy something else. Please try again: ";
					continue;
				}
				break;
			}
			hop=(*hoit);
			//hop now holds the chosen province
			
			cost-=hop->getActualHarvestValue();
			temphold.push_back(hop);
			hoit=holdings.erase(hoit);
			//holding is now in the tempHoldings list and the cost has been decremented.
			
			if(cost<=0)
			{
				cout<<"Card bought."<<endl;
				bought=true;
				break;
			}
			else
			{
				con_hold=false;
				for(hoit=holdings.begin();hoit!=holdings.end();++hoit)
				{
					if(!(*hoit)->getIsTapped())
					//If there are still untapped cards left
					{
						con_hold=true;
						break;
					}
				}
				if(con_hold==false)
				{
					cout<<"Not enough money to buy the card."<<endl;
					bought=false;
					break;
				}
			}
		}
		
		if(bought==true)
		{
			grit=hand.erase(grit);
			arp->attach(grp);
			//Put the card in the personality.
			for(hoit=temphold.begin();hoit!=temphold.end();)
			//Tap all holdings used and put them back in the holdings.
			{
				(*hoit)->setIsTapped(true);
				holdings.push_back(*hoit);
				hoit=temphold.erase(hoit);
			}
		}
		else
		{
			for(hoit=temphold.begin();hoit!=temphold.end();)
			{
				holdings.push_back(*hoit);
				hoit=temphold.erase(hoit);
			}
		}
		
		if(hand.size()==0)
		{
			cout<<"All cards have been used."<<endl;
			break;
		}
		
		con_army=false;
		for(arit=army.begin();arit!=army.end();++arit)
		{
			if((*arit)->getAttachmentNum()<(*arit)->getMaxAttachments())
			{
				con_army=true;
				break;
			}
		}
		if(con_army==false)
		{
			cout<<"No empty attachment slots left in the army."<<endl;
			break;
		}
	}
	
	cout<<"Equip phase ends for player "<<id<<endl;
}

Player* findId(vector<Player*>& players, int id)
//Helper function, used to find a player by id inside a vector.
{
	vector<Player*>::iterator it = players.begin();
	while((*it)->getId()!=id)
	{
		++it;
	}
	return (*it);
}

short Player::getId(void) const
{
	return id;
}

int Player::get_number_of_provinces(void) const
{
	return numberOfProvinces;
}

vector<Province*>& Player::getProvinces(void)
{
	return provinces;
}

void Player::battlePhase(vector<Player*>& players)
//BLOAT: Ask for info.
{
	vector<Player*>::const_iterator plit; //Player iterator
	vector<Province*>::const_iterator prit; //Province iterator
	list<Personality*>::iterator arit; //Army iterator
	list<GreenCard*>::iterator grit; //GreenCard iterator
	list<Personality*> attArm; //Holds the attacking army
	list<Personality*> defArm; //Holds the defending army
	int plch=-1; //The choice for the player to attack
	int prch=0; //The choice for the province to attack
	int arch=0; //The choice for the attacking unit
	Player* plp = NULL; //Pointer to the defending player
	Province * prp = NULL; //Pointer to the defending province
	
	bool con_player=true; //Condition for the player choice loop.
	bool con_army=true; //Condition for the attacking army choice loop. Is not currently used for anything.
	
	int attStr; //The strength of the attacker's army.
	int defStr; //The strength of the defender's army.
	int prStr;  //The strength of the defender's province.
	
	int pointDiff; //Holds the value absolute(attStr-defStr-prStr)
	
	cout<<"Battle phase begins for player "<<id<<endl;
	
	if(this->army.size()==0)
	{
		cout<<"No army. Nothing to do."<<endl;
		return;
	}
	//The army size check is the only check necessary 
	//since a player can't have two battlePhases on the same turn
	//and thus all of his army is untapped.
	
	while (con_player)
	//This keeps on looping until the player either stops or uses his entire army.
	{
		//This prints all of the availiable IDs for attack
		cout<<"Select a player from these (-2 for player info, -1 to finish battle phase) [ -2 -1 ";
		plit = players.begin();
		while(plit != players.end())
		{
			if((*plit)->get_number_of_provinces()>0&&(*plit)!=this)
			{
				cout<<(*plit)->getId()<<' ';
			}
			++plit;
		}
		cout<<"]: ";
		
		//This keeps on looping until the player either quits or chooses a correct player target.
		while(true)
		{
			cin>>plch;
			if(plch==-2)
			{
				cout<<"Info for player: ";
				while(true)
				{
					cin>>plch;
					if(plch<1||plch>players.size())
					{
						cout<<"That is not a valid player id. Please try again: ";
						continue;
					}
					findId(players,plch)->print();
					break;
				}
				cout<<"Select a player from these (-2 for player info, -1 to finish battle phase) [ -2 -1 ";
				plit = players.begin();
				while(plit != players.end())
				{
					if((*plit)->get_number_of_provinces()>0&&(*plit)!=this)
					{
						cout<<(*plit)->getId()<<' ';
					}
					++plit;
				}
				cout<<"]: ";
				continue;
			}
			if(plch==-1)
			{
				cout<<"Battle phase ends for player "<<id<<endl;
				return;
			}
			if(plch<1||plch>players.size())
			{
				cout<<"That is not a valid player id. Please try again: ";
				continue;
			}
			if(findId(players,plch)->get_number_of_provinces()<=0)
			{
				cout<<"You cannot target dead players. Please try again: ";
				continue;
			}
			if(plch==id)
			{
				cout<<"You cannot target yourself. What did you expect, your soldiers fighting each other like gladiators? Try gain: ";
				continue;
			}
			break;
		}
		plp = findId(players,plch);
		//From now on, the player chosen as target will be reffered to as "plp".
		
		//This prints the target's availiable provinces.
		cout<<"Select one of player "<<plp->getId()<<"'s provinces (-2 for province info) [ -2 ";
		prch=0;
		prit = plp->getProvinces().begin();
		while(prit != plp->getProvinces().end())
		{
			if(!((*prit)->isDead()))
			{
				cout<<prch<<' ';
			}
			++prit;
			++prch;
		}
		cout<<"]: ";
		
		//This keeps on looping until the player chooses a correct province target.
		while (true)
		{
			cin>>prch;
			if(prch==-2)
			{
				cout<<"Info for province: ";
				while(true)
				{
					cin>>prch;
					if(prch<0||prch>plp->getProvinces().size()-1)
					{
						cout<<"This is not a valid province id. Please try again: ";
						continue;
					}
					plp->getProvinces()[prch]->print();
					break;
				}
				cout<<"Select one of player "<<plp->getId()<<"'s provinces (-2 for province info) [ -2 ";
				prch=0;
				prit = plp->getProvinces().begin();
				while(prit != plp->getProvinces().end())
				{
					if(!((*prit)->isDead()))
					{
						cout<<prch<<' ';
					}
					++prit;
					++prch;
				}
				cout<<"]: ";
				continue;
			}
			if(prch<0||prch>plp->getProvinces().size()-1)
			{
				cout<<"This is not a valid province id. Please try again: ";
				continue;
			}
			if(plp->getProvinces()[prch]->isDead())
			{
				cout<<"You cannot target destroyed provinces. Please try again: ";
				continue;
			}
			break;
		}
		prp=plp->getProvinces()[prch];
		//From now on, the province chosen as target will be reffered to as "prp".
		
		//This keeps on looping until the player chooses his army
		con_army=true;
		while(con_army)
		{
			//This prints the player's availiable army.
			cout<<"Select your attacking units (-2 for card info, -1 to stop selecting) [ -2 -1 ";
			arit=army.begin();
			arch=0;
			while(arit!=army.end())
			{
				if((!((*arit)->getIsDead())) && (!(*arit)->getIsTapped()))
				{
					cout<<arch<<' ';
				}
				++arit;
				++arch;
			}
			cout<<"]: ";
			
			//This keeps on looping until the player either quits or chooses a unit correctly.
			while (true)
			{
				cin>>arch;
				if(arch==-2)
				{
					cout<<"Info for army unit: ";
					while(true)
					{
						cin>>arch;
						if(arch<0||arch>army.size()-1)
						{
							cout<<"That is not a valid army id. Please try again: ";
							continue;
						}
						arit=army.begin();
						advance(arit,arch);
						(*arit)->print();
						break;
					}
					cout<<"Select your attacking units (-2 for card info, -1 to stop selecting) [ -2 -1 ";
					arit=army.begin();
					arch=0;
					while(arit!=army.end())
					{
						if((!((*arit)->getIsDead())) && (!(*arit)->getIsTapped()))
						{
							cout<<arch<<' ';
						}
						++arit;
						++arch;
					}
					cout<<"]: ";
					continue;
				}
				if(arch==-1)
				{
					if(attArm.size()==0)
					{
						cout<<"You must select at least one unit: ";
						continue;
					}
					cout<<"Army selection finished."<<endl;
					con_army=false;
					break;
				}
				if(arch<0||arch>army.size()-1)
				{
					cout<<"That is not a valid army id. Please try again: ";
					continue;
				}
				arit=army.begin();
				advance(arit,arch);
				if((*arit)->getIsTapped())
				{
					cout<<"This unit has already been used in another attack. Please try again: ";
					continue;
				}
				if((*arit)->getIsDead())
				{
					cout<<"This unit has commited suicide in order to save its honour. Please try again: ";
					continue;
				}
				break;
			}
			//arit now points to the selected unit.
			
			if(con_army==false)
			//If the user has requested to stop choosing.
			{
				break;
			}
			
			//arit is now put into the attacking army list and removed from the army.
			attArm.push_back(*arit);
			arit=army.erase(arit);
				
			
			//If the army has been used exit immedietly.
			con_army=false;
			for(arit=army.begin();arit!=army.end();++arit)
			{
				if((!((*arit)->getIsDead())) && (!(*arit)->getIsTapped()))
				{
					con_army=true;
					break;
				}
			}
		}
		//The attacking army should now be in attArm
		
		//Put the defending army in defArm.
		for(arit=plp->army.begin();arit!=plp->army.end();)
		{
			if((!((*arit)->getIsDead())) && (!(*arit)->getIsTapped()))
			{
				defArm.push_back(*arit);
				arit=army.erase(arit);
			}
			else
			{
				++arit;
			}
		}
		
		//This computes the attacking army's strength
		attStr=0;
		for(arit=attArm.begin();arit!=attArm.end();++arit)
		{
			attStr+=(*arit)->getActualAttack();
		}
		
		//This computes the defending army's strength
		defStr=0;
		for(arit=defArm.begin();arit!=defArm.end();++arit)
		{
			defStr+=(*arit)->getActualDefence();
		}
		
		//And here we get the province's defence.
		prStr=prp->getDefence();
		
		//Now we have everything we need.
		
		//Print some info about the upcoming battle.
		cout<<"Player "<<id<<" attacks player "<<plp->getId()<<"'s province "<<prch<<endl;
		cout<<"The attacker moves in with "<<attArm.size()<<
			" personalities with a total attack strength of "<<attStr<<endl;
		cout<<"The defender rallies "<<defArm.size()<<
			" personalities for the province's defence with a total defence strength of "<<defStr<<endl;
		cout<<"The province itself provides an additional "<<prStr<<
			" strength, bringing the total defence strength to "<<defStr+prStr<<endl;
			
		if (attStr-defStr-prStr>prStr)
		{
			cout<<"Player "<<id<<"'s army is VICTORIOUS!\nThe defending army has been DESTROYED!\nThe province has been DESTROYED!"<<endl;
			prp->die();
			plp->numberOfProvinces=plp->numberOfProvinces-1;
			//Destroy the province.
			for(arit=defArm.begin();arit!=defArm.end();)
			//Destroy the defending army.
			{
				delete (*arit);
				arit=defArm.erase(arit);
			}
			for(arit=attArm.begin();arit!=attArm.end();)
			//Do all things necessary to the attacking army
			{
				(*arit)->battleFought();
				//Reduces the durability of all items.
				(*arit)->setIsTapped(true);
				//Taps all cards
				army.push_back(*arit);
				arit=attArm.erase(arit); //arit now points to the element after the erased one.
				//Transfers cards back to the player's army.
			}
		}
		else if (attStr>defStr+prStr)
		{
			cout<<"Player "<<id<<"'s army has suffered a MINOR DEFEAT!\nThe defending army has been DESTROYED!\nThe attacking army SUFFERS LOSSES!"<<endl;
			for(arit=defArm.begin();arit!=defArm.end();)
			//Destroy the defending army.
			{
				delete (*arit);
				arit=defArm.erase(arit);
			}
			pointDiff=attStr-defStr-prStr;
			//Remove at least this many points from the attacking player's army.
			for(arit=attArm.begin();arit!=attArm.end();)
			//Do all things necessary to the attacking army
			{
				(*arit)->battleFought();
				//Reduces the durability of all items.
				(*arit)->setIsTapped(true);
				//Taps all cards

				if(pointDiff>0)
				//If there's need to reduce the attacker's power.
				{
					if((*arit)->getActualAttack()<pointDiff||(*arit)->getAttachmentNum()==0)
					{
						pointDiff-=(*arit)->getActualAttack();
						delete(*arit);
					}
					else
					//Start deleting followers.
					{
						grit=(*arit)->getAttachments().begin();
						while(pointDiff>0&&grit!=(*arit)->getAttachments().end())
						{
							if((*grit)->getType()==FOLLOWER)
							{
								pointDiff=pointDiff-(TypeConverter().getFollower(*grit)->getAttackBonus()+
									TypeConverter().getFollower(*grit)->getEffectActive()*
									TypeConverter().getFollower(*grit)->getEffectBonus());
								(*grit)->detach();
								delete(*grit);
								grit=(*arit)->getAttachments().erase(grit);
							}
							else
							{
								++grit;
							}
						}
							
						if(pointDiff>0)
						//If deleting the followers wasn't enough
						{
							pointDiff-=(*arit)->getActualAttack();
							delete(*arit);
							//Delete the personality
						}
						else
						{
							(*arit)->reduceHonour();
							army.push_back(*arit);
							//Put the personality back in the army.
						}
					}
				}
				else
				{
					(*arit)->reduceHonour();
					army.push_back(*arit);
				}
				arit=attArm.erase(arit);
			}
			
		}
		else if (attStr==defStr+prStr)
		{
			cout<<"Player "<<id<<"'s army has been DEFEATED!\nThe defending army has been DESTROYED!\nThe attacking army has been DESTROYED!"<<endl;
			for(arit=attArm.begin();arit!=attArm.end();)
			//Destroy the attacking army
			{
				delete(*arit);
				arit=attArm.erase(arit);
			}
			for(arit=defArm.begin();arit!=defArm.end();)
			//Destroy the defending army
			{
				delete(*arit);
				arit=defArm.erase(arit);
			}
		}
		else if (attStr<defStr+prStr)
		{
			cout<<"Player "<<id<<"'s army has suffered a MAJOR DEFEAT!\nThe defending army SUFFERS LOSSES!\nThe attacking army has been DESTROYED!"<<endl;
			for(arit=attArm.begin();arit!=attArm.end();)
			//Destroy the attacking army
			{
				delete(*arit);
				arit=attArm.erase(arit);
			}
			
			pointDiff=defStr+prStr-attStr;
			//Remove at least this many points from the defending player's army.
			for(arit=defArm.begin();arit!=defArm.end();)
			//Do all things necessary to the defending army
			{
				//(*arit)->battleFought();

				if(pointDiff>0)
				//If there's need to reduce the defender's power.
				{
					if((*arit)->getActualDefence()<pointDiff||(*arit)->getAttachmentNum()==0)
					
					{
						pointDiff-=(*arit)->getActualDefence();
						delete(*arit);
					}
					else
					//Start deleting followers.
					{
						grit=(*arit)->getAttachments().begin();
						while(pointDiff>0&&grit!=(*arit)->getAttachments().end())
						{
							if((*grit)->getType()==FOLLOWER)
							{
								pointDiff=pointDiff-(TypeConverter().getFollower(*grit)->getDefenceBonus()+
									TypeConverter().getFollower(*grit)->getEffectActive()*
									TypeConverter().getFollower(*grit)->getEffectBonus());
								(*grit)->detach();
								delete(*grit);
								grit=(*arit)->getAttachments().erase(grit);
							}
							else
							{
								++grit;
							}
						}
							
						if(pointDiff>0)
						//If deleting the followers wasn't enough
						{
							pointDiff-=(*arit)->getActualDefence();
							delete(*arit);
							//Delete the personality
						}
						else
						{
							plp->army.push_back(*arit);
							//Put the personality back in the army.
						}
					}
				}
				else
				{
					plp->army.push_back(*arit);
				}
				arit=defArm.erase(arit);
			}
		}
		
		//After battle checks
		con_player=false;
		for(arit=army.begin();arit!=army.end();++arit)
		{
			if((!((*arit)->getIsDead())) && (!(*arit)->getIsTapped()))
			{
				con_player=true;
				break;
			}
		}
		if(con_player==false)
		{
			cout<<"No more unused units left in army."<<endl;
		}
	}
	
	cout<<"Battle phase ends for player "<<id<<endl;
}


