#include "L5RGameBoard.hpp"


//Public

GameBoard::GameBoard(void)
{
	//Constructor. Initialise data as necessary.
	cout << "What a pretty gameboard that is!" << endl;
}

bool likeasort(Player* a, Player* b){return a->sorting(a, b);}

void GameBoard::initializeGameBoard(int n)
{
	for (int i=0; i< n; i++)
		vec.push_back( new Player(i + 1));
	
	sort(vec.begin(), vec.end(), likeasort);
	// !! Function depends on deckbuilder. Maybe best to wait until it becomes availiable
	//All players are created and given their cards. 
	//Needs to be given info about how many card categories each player can have.
}

void GameBoard::printGameStatistics(void)
{	cout << "Statistics :" << endl;
	for (vector<Player*>::iterator it= vec.begin(); it != vec.end(); it++ )
		(*it)->print();
	cout << endl;
	//Print statistics about the game 
	// ?? (What statistics should they be?)
}

void GameBoard::gameplay()
{	int i, remain=vec.size(), j=0;
	while(true){
		j++;
		cout<<"\t--- Round "<< j << " ---\n" <<endl;
		for (i=0; i< remain; i++){
			vec[i]->shortprint();
			vec[i]->initialPhase();	//InitialPhase
		}
		
		for (i=0; i< remain; i++){
			vec[i]->shortprint();
			vec[i]->equipPhase();	//EquipPhase
		}


		for (i=0; i< remain; i++){
			vec[i]->shortprint();
			vec[i]->battlePhase(vec);	//BattlePhase
		}


		for (i=0; i< remain; i++){
			vec[i]->shortprint();
			vec[i]->economyPhase();	//EconomyPhase
		}

		for (i=0; i< remain; i++){
			vec[i]->shortprint();
			vec[i]->discardSurplusFateCards();
		}

		printGameStatistics();
		
		for (i=0; i< remain; i++) //maybe after BattlePhase?
			if (vec[i]->get_number_of_provinces() == 0){
				cout << "Has lost :";
				vec[i]->shortprint();
				vec.push_back(vec[i]);
				vec.erase(vec.begin() + i);
				i--;
				remain--;
			}



		if (checkWinningCondition(remain)){
			cout<<"And the winner is..."<<endl;
			vec[0]->print();
			break;}
	}
	
}

GameBoard::~GameBoard(void) //Destructor. Delete data as necessary.
{
	if(vec.size()==0)
	{
		return;
	}
	vector<Player*>::iterator it;
	it = vec.begin();
	while(it!=vec.end())
	{
		delete *it;
		++it;
	}
}

bool GameBoard::checkWinningCondition(int n){return n == 1;}


// <-- Any other function goes here.
// ?? Need other functions?
