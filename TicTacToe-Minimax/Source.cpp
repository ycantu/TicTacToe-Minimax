#include <iostream>
#include "Board.h"

/* 
TODO
- After win, ask to play again
- Make AI for tic-tac-toe (Minimax)
- Testing
*/

int main()
{
	Board n;
	bool win = false;
	//bool playAgain;

	n.introPrompt();

	/*while (playAgain == true)
	{

	}*/

	while (win == false)
	{
		n.displayBoard();
		n.turnPrompt();
		win = n.checkWinAI();

			
			//n.displayBoard();
		std::cout << win << std::endl;

		n.nextPlayer();
	}

	

/*	std::vector<char> m = { ' ', ' ', 'x', 'x', 'x', 'o', 'o', 'o' , 'x'};

	AI* x = new AI(m);
	std::vector<char> l = x->hypotheticalBoard(m, 1, true);

	for (int i = 0; i < 9; i++)
	{
		std::cout << m[i] << std::endl;
	}

	std::cout << "COPY " << std::endl;

	for (int i = 0; i < 9; i++)
	{
		std::cout << l[i] << std::endl;
	}*/

	/*std::vector<char> m = { '3', '3', 'x', 'o', '3', 'o', 'o', 'o' , 'x' };
	AI x;
	std::cout << x.checkWin(m, 0) << std::endl;*/

	system("pause");
	return 0;

}