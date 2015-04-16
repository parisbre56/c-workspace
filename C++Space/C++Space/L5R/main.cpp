// !! Remember to change defense to defence with a C! 


#include "L5RGameBoard.hpp"
#include <iostream>
#include <cstdlib>

int main(int argc, char **argv)
{
	//Data
	if (argc != 2){
		cout << "Number of players needed" << endl;
		return 0;}

	int n = atoi(argv[1]);
	if (n < 2){
		cout << "Need at least two players" << endl;
		return 0;}
	GameBoard gameboard;
	//Functionality
	
	// ?? Need to get data about the players here, inside initializeGameBoard or both?
	
	gameboard.initializeGameBoard(n);
	gameboard.printGameStatistics();
	gameboard.gameplay();
	
	// ?? Anything else before exiting?
	return EXIT_SUCCESS;
}
