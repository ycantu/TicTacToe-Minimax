#pragma once
#include <vector>
#include <algorithm>

//GLOBAL
int TOTAL_CELLS = 9;
int TOTAL_WINS = 8;
char BLANK = ' ';
char X_SYM = 'X';
char O_SYM = 'O';

class Player
{
public:
	int position;
	char sym;
	bool ai;

	Player()
	{

	}

	Player(int pos, char x)
	{
		position = pos;
		sym = x;
		ai = false;
	}

	void setProperties(int pos, char x)
	{
		position = pos;
		sym = x;
	}

};

//Inherit from player
class AI: public Player
{
public:
	//Board* state;
	int bestMove;
	int wins[8][3] = { {0, 1, 2},{3, 4, 5},{6, 7, 8},
	{0, 3, 6},{1, 4, 7},{2, 5, 8},
	{0, 4, 8},{2, 4, 6} };


	AI()
	{
		bestMove = -1;
	}

	AI(std::vector<char> x)
	{
		//y = x;
		bestMove = -1;
	}


	int checkWin(std::vector<char> values, int depth)
	{
		for (int i = 0; i < TOTAL_WINS; i++)
		{
			if (values[wins[i][0]] == values[wins[i][1]] && values[wins[i][1]] == values[wins[i][2]])
			{
				if (values[wins[i][0]] == X_SYM)
					return 10 + depth;
				if (values[wins[i][0]] == O_SYM)
					return depth - 10;
			}
		}

		if (isBoardFull(values))
			return 0;
		else
			return -1;
	}

	bool isBoardFull(std::vector<char> x)
	{
		for (int i = 0; i < TOTAL_CELLS; i++)
		{
			if (x[i] == BLANK)
				return false;
		}

		return true;
	}

	std::vector<int> possibleMoves(std::vector<char> x)
	{
		std::vector<int> pos;
		
		for (int i = 0; i < TOTAL_CELLS; i++)
		{
			if (x[i] == BLANK)
				pos.push_back(i);
		}

		return pos;

	}

	std::vector<char> hypotheticalBoard(const std::vector<char> &x, int pos, bool minMax)
	{
		std::vector<char> z = x;
		z[pos] = (minMax) ? X_SYM : O_SYM;
		return z;
	}

	int minimax(const std::vector<char> x, int depth, bool min)
	{
		if (checkWin(x, depth) != -1)
			return checkWin(x, depth);

		std::vector<int> scores;
		std::vector<int> moves;

		std::vector<int> possible = possibleMoves(x);
	
		for(std::vector<int>::size_type i = 0; i != possible.size(); i++)//(auto iter = possible.begin(); iter != possible.end(); ++iter)
			{
				std::vector<char> h = hypotheticalBoard(x, possible[i], min);
				scores.push_back(minimax(h, depth + 1, !min));
				moves.push_back(possible[i]);
			}

		if (min)
		{
			int maxIndex = (max_element(scores.begin(), scores.end()) - scores.begin());
			bestMove = moves[maxIndex];
			return scores[maxIndex];
		}
		else
		{
			int minIndex = (min_element(scores.begin(), scores.end()) - scores.begin());
			bestMove = moves[minIndex];
			return scores[minIndex];
		}
	}

	int makeAIMove(std::vector<char> x, bool min)
	{
		if (min)
			minimax(x, 0, true);
		else
			minimax(x, 0, false);

		return bestMove;
	}
};

// Board class keeps state of board, displays, and prompts
class Board
{
public:
	std::vector<char> values;
	char playerOneSym;
	bool turn;
	int mode;
	Player* one;
	Player* two;
	AI* computer;
	int wins[8][3] = { { 0, 1, 2 },{ 3, 4, 5 },{ 6, 7, 8 },
	{ 0, 3, 6 },{ 1, 4, 7 },{ 2, 5, 8 },
	{ 0, 4, 8 },{ 2, 4, 6 } };

	Board()
	{
		for (int i = 0; i < TOTAL_CELLS; i++)
		{
			values.push_back(BLANK);
		}

		turn = false; //If false, x goes first, else o goes first
		playerOneSym = X_SYM;
		mode = 0;
		one = new Player();
		two = new Player();
		computer = new AI();
	}

	void displayBoard()
	{
		std::cout << values[0] << " | " << values[1] << " | " << values[2] << std::endl;
		std::cout << "_________" << std::endl;
		std::cout << values[3] << " | " << values[4] << " | " << values[5] << std::endl;
		std::cout << "_________" << std::endl;
		std::cout << values[6] << " | " << values[7] << " | " << values[8] << std::endl;
		std::cout << std::endl << std::endl;
	}

	void clearBoard()
	{
		values.clear();

		for (int i = 0; i < TOTAL_CELLS; i++)
		{
			values.push_back(' ');
		}

		if (one->position == 1)
			turn = false;
		else
			turn = true;
	}

	void introPrompt()
	{
		std::cout << "Select '1' to play against a buddy or type '2' to play against the computer!"<< std::endl;

		std::cin >> mode;

		if (mode == 1 || mode == 2)
		{
			setTokensPrompt();
		}
		else
		{
			std::cout << "That's not a valid response" << std::endl;
			introPrompt();
		}
	}

	void setTokensPrompt()
	{
		std::cout << "Player 1 choose your symbol, type '" << X_SYM << "' or '" << O_SYM << "'" << std::endl;
		std::cin >> playerOneSym;
		
	/*	if (mode == 1)
		{
			if (playerOneSym == X_SYM)
			{
				one->setProperties(1, X_SYM);
				two->setProperties(2, O_SYM);
			}

			else if (playerOneSym == O_SYM)
			{
				one->setProperties(1, O_SYM);
				two->setProperties(2, X_SYM);
				turn = !turn;
			}
			else
			{
				std::cout << "That's not a valid response" << std::endl;
				setTokensPrompt();
			}
		}

		else if (mode == 2)
		{
			// Make a player a computer
			if (playerOneSym == X_SYM)
			{
				one->setProperties(1, X_SYM);
				computer->setProperties(2, O_SYM);
			}
			else if (playerOneSym == O_SYM)
			{
				one->setProperties(2, O_SYM);
				computer->setProperties(1, X_SYM);
				turn = !turn;
			}
			else
			{
				std::cout << "That's not a valid response" << std::endl;
				setTokensPrompt();
			}
		}*/

		if (playerOneSym == X_SYM)
		{
			if (mode == 1)
			{
				one->setProperties(1, X_SYM);
				two->setProperties(2, O_SYM);
			}
			else
			{
				one->setProperties(1, X_SYM);
				computer->setProperties(2, O_SYM);
			}
		}
		else if (playerOneSym == O_SYM)
		{
			if (mode == 1)
			{
				one->setProperties(1, O_SYM);
				two->setProperties(2, X_SYM);
			}
			else
			{
				one->setProperties(2, O_SYM);
				computer->setProperties(1, X_SYM);
			}

			turn = !turn;
		}
		else
		{
			std::cout << "That's not a valid response" << std::endl;
			setTokensPrompt();
		}
			
	}

	bool playAgainPrompt() 
	{
		int play;
		std::cout << "Would you like to play again? (1 for yes, 2 for no)" << std::endl;
		std::cin >> play;
		return (play == 1) ? true : false;
	}

	void turnPrompt()
	{
		if (mode == 1)
		{
			if (turn == false)
				turnPrompt(one);			
			else
				turnPrompt(two);
		}
		else
		{
			if (turn == false)
				turnPrompt(one);
			else
				turnPromptAI();
		}		
	}

	void turnPrompt(Player* x)
	{
		int cell;

		std::cout << "Player " << x->position << " turn" << std::endl;
		std::cout << "Which cell would you like to fill?" << std::endl;
		std::cin >> cell;

		if (values[cell] != BLANK)
			turnPrompt(x);
		else
			values[cell] = x->sym;
	}

	void turnPromptAI()
	{
		std::cout << "Waiting for computer to make a move..." << std::endl;
		int computerMove;

		if(computer->position == 2)
			computerMove = computer->makeAIMove(values, false);
		else
			computerMove = computer->makeAIMove(values, true);

		values[computerMove] = computer->sym;
		std::cout << "Computer chose cell " << computerMove << std::endl;
	}

	void nextPlayer()
	{
		//Switch to next player
		turn = !turn;
	}
	
	bool checkWin()
	{
		if (turn == false)
			return checkWin(one->sym);
		else
			return checkWin(two->sym);
	}

	bool checkWinAI()
	{
		int score = checkWin(0);
		if (score != -1)
		{
			if (score == 10)
				std::cout << "'" << X_SYM << "' wins!" << std::endl;
			else if (score == -10)
				std::cout << "'" << O_SYM << "' wins!" << std::endl;
			else
				std::cout << "It's a draw!" << std::endl;

			return true;
		}
			
		else
			return false;
	}

	int checkWin(int depth)
	{
		for (int i = 0; i < 8; i++)
		{
			if (values[wins[i][0]] == values[wins[i][1]] && values[wins[i][1]] == values[wins[i][2]])
			{
				if (values[wins[i][0]] == X_SYM)
					return 10 + depth;
				if (values[wins[i][0]] == O_SYM)
					return depth - 10;
			}
		}

		if (isBoardFull())
			return 0;
		else
			return -1;
	}

	bool isBoardFull()
	{
		for (int i = 0; i < 9; i++)
		{
			if (values[i] == BLANK)
				return false;
		}

		return true;
	}

};