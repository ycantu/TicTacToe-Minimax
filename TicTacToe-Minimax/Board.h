#pragma once
#include <vector>
#include <algorithm>

//GLOBAL
int TOTAL_CELLS = 9;
int WINS[8][3] = { { 0, 1, 2 },{ 3, 4, 5 },{ 6, 7, 8 },
{ 0, 3, 6 },{ 1, 4, 7 },{ 2, 5, 8 },
{ 0, 4, 8 },{ 2, 4, 6 } };
int TOTAL_WINS = 8;
char BLANK = ' ';
char X_SYM = 'X';
char O_SYM = 'O';

//Contains position and symbol for a player
class Player
{
public:
	int position;
	char sym;

	Player()
	{
		position = -1;
		sym = BLANK;
	}

	Player(int pos, char x)
	{
		position = pos;
		sym = x;
	}

	void setProperties(int pos, char x)
	{
		position = pos;
		sym = x;
	}
};

//Inherit from player, contains methods possible for minimax to work: 1)Check for wins, 2) Check for possible moves, 3) Return hypothetical board, 4) Minimax method
class AI: public Player
{
private:
	int bestMove;

	int checkWin(std::vector<char> values, int depth)
	{
		for (int i = 0; i < TOTAL_WINS; i++)
		{
			if (values[WINS[i][0]] == values[WINS[i][1]] && values[WINS[i][1]] == values[WINS[i][2]])
			{
				if (values[WINS[i][0]] == X_SYM)
					return 10 + depth;
				if (values[WINS[i][0]] == O_SYM)
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
public:
	AI()
	{
		bestMove = -1;
	}

	int makeAIMove(std::vector<char> x, bool min)
	{
		if (min) //If true, then we are maximizing
			minimax(x, 0, true);
		else //False, we are minimizing
			minimax(x, 0, false);

		return bestMove;
	}
};

// Board class keeps state of board, displays, and prompts
class Board
{
private:

	std::vector<char> values; //Values contains board values
	bool turn;
	int mode; //1 for two humans, 2 for computer
	Player* one;
	Player* two;
	AI* computer;

	void clearBoard()
	{
		values.clear();

		for (int i = 0; i < TOTAL_CELLS; i++)
		{
			values.push_back(BLANK);
		}
	}

	void resetTurns()
	{
		if (one->sym == X_SYM || one->position == -1)/*playerOneSym == X_SYM || one->position == -1*/
			turn = false;
		else
			turn = true;
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

		turn = !turn;
	}

	void turnPromptAI(AI* computer)
	{
		std::cout << "Waiting for computer to make a move..." << std::endl;
		int computerMove;

		if (one->sym == X_SYM)/* (computer->position == 2)   */
			computerMove = computer->makeAIMove(values, false);
		else
			computerMove = computer->makeAIMove(values, true);

		values[computerMove] = computer->sym;
		std::cout << "Computer chose cell " << computerMove << std::endl;

		turn = !turn;
	}

	int checkWin()
	{
		for (int i = 0; i < TOTAL_WINS; i++)
		{
			if (values[WINS[i][0]] == values[WINS[i][1]] && values[WINS[i][1]] == values[WINS[i][2]])
			{
				if (values[WINS[i][0]] == X_SYM)
					return 10;
				if (values[WINS[i][0]] == O_SYM)
					return -10;
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

public:
	Board()
	{
		for (int i = 0; i < TOTAL_CELLS; i++)
		{
			values.push_back(BLANK);
		}

		turn = false; //If false, x goes first, else o goes first
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
		std::cout << "Select '1' to play against a buddy or '2' to play against the computer!"<< std::endl;

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
		std::cin >> one->sym;

		if (one->sym == X_SYM)
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
		else if (one->sym == O_SYM)
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

	bool changeControls()
	{
		int change;
		std::cout << "Would you like to change controls? (1 for yes, 2 for no) " << std::endl;
		std::cin >> change;

		if (change == 1)
		{
			introPrompt();
			return true;
		}			
		else if (change == 2)
			return false;
		else
			changeControls();
	}

	void turnPrompt()
	{
		if (mode == 1)
		{
			if (turn == false)
				turnPrompt(one);			
			else if(turn == true)
				turnPrompt(two);				
		}
		else
		{
			if (turn == false)
				turnPrompt(one);
			else
				turnPromptAI(computer);
		}		
	}

	void restartGame()
	{
		clearBoard();
		resetTurns();
	}

	bool gameOver()
	{
		int score = checkWin();
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

};