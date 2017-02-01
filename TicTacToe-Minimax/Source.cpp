#include <iostream>
#include "Board.h"

/* 
TODO
- Testing
- Clean up code
- See if i dont have to reuse methods in AI
*/

int main()
{
	Board n;
	bool win = false;
	bool playAgain = true;

	std::cout << "Welcome to Tic-Tac-Toe!" << std::endl;
	n.introPrompt();

	while (playAgain == true)
	{
		n.clearBoard();
		win = false;

		while (win == false)
		{
			n.displayBoard();
			n.turnPrompt();
			win = n.checkWinAI();
			n.nextPlayer();
		}

		n.displayBoard();
		playAgain = n.playAgainPrompt();
	}

	system("pause");
	return 0;

}