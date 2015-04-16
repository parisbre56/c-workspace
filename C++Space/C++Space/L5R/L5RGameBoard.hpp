#ifndef __L5RGameBoard__
#define __L5RGameBoard__

#include "L5RPlayer.hpp"
class GameBoard
{
//Data
public:

private:
	// <-- A structure to put players in (array, list, etc.). Maybe organize them based on their Honour to make gameplay function easier.
	// ?? Need any more data?
	vector<Player*> vec;
//Functions
public:
	GameBoard(void);
	//Constructor. Initialise data as necessary.
	void initializeGameBoard(int n);
	// !! Function depends on deckbuilder. Maybe best to wait until it becomes availiable
	//All players are created and given their cards. 
	//Needs to be given info abouthow many players are there.
	//Needs to be given info about how many card categories each player can have.
	void printGameStatistics(void);
	//Print statistics about the game 
	// ?? (What statistics should they be?)
	void gameplay();
	//This function runs the entire game.
	/* Start looping until winning condition == true
	 * {
	 * 		For all undefeated Players in order of Honour 
	 * 		(most honourable plays first, least honourable plays last)
	 * 		{
	 * 			InitialPhase
	 * 		}
	 * 		For all undefeated Players in order of Honour
	 * 		{
	 * 			EquipmentPhase
	 * 		}
	 * 		For all undefeated Players in order of Honour
	 * 		{
	 * 			BattlePhase
	 * 		}
	 * 		For all undefeated Players in order of Honour
	 * 		{
	 * 			EconomyPhase
	 * 		}
	 * 		For all undefeated Players in order of Honour
	 * 		{
	 * 			FinalPhase
	 * 			If this Player has won
	 * 			{
	 * 				winning condition = true;
	 * 				Say that this player has won.
	 * 				break;
	 * 			}
	 * 		}
	 * 		printGameStatistics();
	 * 		For all players
	 * 		{
	 * 			PrintInfo
	 * 		}
	 * }
	 * // ?? Need anything else before exiting?
	 */
	~GameBoard(void);
	//Destructor. Delete data as necessary.
private:
	bool checkWinningCondition(int);
	//Called by gameplay after a player finshes their turn and
	//checks whether or not that player has won.
	//Returns true if everybody except that player has numberOfProvines==0
	//Needs to somehow know which player it's checking for.

	// <-- Any other function goes here.
	// ?? Need other functions?
};

#endif
