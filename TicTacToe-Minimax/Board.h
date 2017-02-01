#pragma once
#include <vector>
#include <algorithm>

//GLOBAL
int CELLS = 9;

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
	//std::vector<char> y;
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
		for (int i = 0; i < 8; i++)
		{
			if (values[wins[i][0]] == values[wins[i][1]] && values[wins[i][1]] == values[wins[i][2]])
			{
				if (values[wins[i][0]] == 'x')
					return 10 + depth;
				if (values[wins[i][0]] == 'o')
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
		for (int i = 0; i < 9; i++)
		{
			if (x[i] == ' ')
				return false;
		}

		return true;
	}

	std::vector<int> possibleMoves(std::vector<char> x)
	{
		std::vector<int> pos;
		
		for (int i = 0; i < 9; i++)
		{
			if (x[i] == ' ')
				pos.push_back(i);
		}
		//std::cout << "SIZE OF POS " << pos.size() << std::endl;

		return pos;

	}

	std::vector<char> hypotheticalBoard(const std::vector<char> &x, int pos, bool minMax)
	{
		//std::cout << "BEGIN HYPO BOARD" << std::endl;
		std::vector<char> z = x;
		//std::cout << x.size() << std::endl;
		z[pos] = (minMax) ? 'x' : 'o';
		//std::cout << z[pos] << std::endl;
		return z;
	}

	int minimax(const std::vector<char> x, int depth, bool min)
	{
		//std::cout << "STARTING MINIMAX" << std::endl;
		if (checkWin(x, depth) != -1)
			return checkWin(x, depth);

		//std::cout << "CHECKED WIN!" << std::endl;

		std::vector<int> scores;
		std::vector<int> moves;

		//std::cout << "GET POSSIBLE MOVES" << std::endl;
		std::vector<int> possible = possibleMoves(x);
		//std::cout << "RETURNED POSSIBLE MOVES" << possible.size() << std::endl;

		//if (possible.size() != 0) {
		for(std::vector<int>::size_type i = 0; i != possible.size(); i++)//(auto iter = possible.begin(); iter != possible.end(); ++iter)
			{
				//int i = *iter;
				//std::cout << "GET H BOARD I IS " << i << std::endl;
				std::vector<char> h = hypotheticalBoard(x, possible[i], min);
				//std::cout << "GOT H BOARD" << std::endl;
				scores.push_back(minimax(h, depth + 1, !min));
				moves.push_back(possible[i]);
				//std::cout << "POSSIBLE MOVES" << std::endl;
			}
		//}

		//std::cout << "PAST LOOP " << possible.size() <<  std::endl;
		if (min)
		{
			//std::cout << "SCORES SIZE: " << scores.size() << std::endl;
			int maxIndex = (max_element(scores.begin(), scores.end()) - scores.begin());
			//std::cout << "maxINDEX is " <<  std::endl;
			bestMove = moves[maxIndex];
			//std::cout << "maxINDEX" << std::endl;
			return scores[maxIndex];
		}
		else
		{
			//std::cout << "SCORES SIZE: " << scores.size() << std::endl;
			int minIndex = (min_element(scores.begin(), scores.end()) - scores.begin());
			//std::cout << "minINDEX is " << minIndex << std::endl;
			bestMove = moves[minIndex];
			//std::cout << "RETURN MIN" << std::endl;
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
		for (int i = 0; i < CELLS; i++)
		{
			values.push_back(' ');
		}

		turn = false; //If false, x goes first, else o goes first
		playerOneSym = 'x';
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

	void introPrompt()
	{
		std::cout << "Welcome to Tic-Tac-Toe!" << std::endl;
		std::cout << "Select '1' to play against a buddy or type '2' to play against the computer!"<< std::endl;

		std::cin >> mode;

		if (mode == 1)
		{
			std::cout << "Player 1 choose your symbol, type 'x' or '0'" << std::endl;
			std::cin >> playerOneSym;

			if (playerOneSym == 'x')
			{
				one->setProperties(1, 'x');
				two->setProperties(2, 'o');
			}
				
			else
			{
				one->setProperties(1, 'o');
				two->setProperties(2, 'x');
				turn = !turn;
			}
		}

		else if (mode == 2)
		{
			std::cout << "Player 1 choose your symbol, type 'x' or '0'" << std::endl;
			std::cin >> playerOneSym;

			// Make a player a computer
			if (playerOneSym == 'x')
			{
				one->setProperties(1, 'x');
				computer->setProperties(2, 'o');
			}
			else
			{
				one->setProperties(2, 'o');
				computer->setProperties(1, 'x');
				turn = !turn;
			}
		}

		else
		{
			std::cout << "That's not a valid response" << std::endl;
			//Make it so that it prompts again and returns to top of if statements
			introPrompt();
		}
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

		// Write value 
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
		if (checkWin(0) != -1)
			return true;
		else
			return false;
	}

	int checkWin(int depth)
	{
		for (int i = 0; i < 8; i++)
		{
			if (values[wins[i][0]] == values[wins[i][1]] && values[wins[i][1]] == values[wins[i][2]])
			{
				if (values[wins[i][0]] == 'x')
					return 10 + depth;
				if (values[wins[i][0]] == 'o')
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
			if (values[i] == ' ')
				return false;
		}

		return true;
	}

};