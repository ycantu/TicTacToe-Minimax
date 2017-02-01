#include <iostream>
#include "Board.h"

int main()
{
	Board n;
	bool win = false;
	bool playAgain = true;
	bool changeControl = true;

	std::cout << "Welcome to Tic-Tac-Toe!" << std::endl;
	n.introPrompt();

	// Main flow of game
	while (changeControl == true)
	{
		while (playAgain == true)
		{
			n.restartGame();
			win = false;

			while (win == false)
			{
				n.displayBoard();
				n.turnPrompt();
				win = n.gameOver();
			}

			n.displayBoard();
			playAgain = n.playAgainPrompt();
			
		}
		changeControl = n.changeControls();
		playAgain = (changeControl) ? true : false;
	}

	system("pause");
	return 0;

}