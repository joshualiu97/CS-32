#include "Player.h"

// Player class
// (in .h file since implementations are short)

// HumanPlayer class
int HumanPlayer::chooseMove(const Board& b, Side s) const
{
	if (b.beansInPlay(s) == 0) // if there are no beans in play, no move is possible
		return -1;

	int input;
	std::cout << "Enter a hole number, " << name() << ": ";
	std::cin >> input; // read input
	std::cin.ignore(10000, '\n');
	while (b.beans(s, input) <= 0) // while input is invalid, prompt for another input
	{
		if (b.beans(s, input) == 0 && input > 0 && input < b.holes() + 1)
			std::cout << "There are no beans in this hole. Pick another one." << std::endl;
		else
			std::cout << "The hole number must be from 1 to " << b.holes() << "." << std::endl;
		std::cout << "Enter a hole number: ";
		std::cin >> input;
		std::cin.ignore(10000, '\n');
	}
	return input;
}

// BadPlayer class
int BadPlayer::chooseMove(const Board& b, Side s) const
{
	if (b.beansInPlay(s) == 0) // if there are no beans in play, no move is possible
		return -1;
	
	// choose spot closest to pot
	int posNorth = 1;
	int posSouth = b.holes();
	if (s == Side::NORTH)
	{
		while (b.beans(Side::NORTH, posNorth) == 0)
			posNorth++;
		return posNorth;
	}
	else
	{
		while (b.beans(Side::SOUTH, posSouth) == 0)
			posSouth--;
		return posSouth;
	}

	//// generate random move
	//int random = 1;
	//do {
	//	srand(unsigned(time(0)));
	//	random = rand() % b.holes() + 1;
	//} while (b.beans(s, random) <= 0);
	//return random;

	//// beans closes to spot 1
	//for (int i = 1; i < b.holes() + 1; i++)
	//{
	//	if (b.beans(s, i) != 0)
	//	{
	//		return i;
	//	}
	//}
}

// SmartPlayer class
int SmartPlayer::chooseMove(const Board& b, Side s) const
{
	AlarmClock ac(3000);  // will time out after 3.0 seconds
	if (b.beansInPlay(s) == 0) // if there are no beans in play, no move is possible
		return -1;

	// variables for smartMove
	Side player = s;
	int bestHole = -2;
	int value = 0;
	int counter = 0;

	smartMove(ac, b, player, player, bestHole, value, counter); // find the best move possible
	
	if (b.beans(s, bestHole) <= 0) // if best move is not valid (should not happen, but just in case)
	{
		for (int i = 1; i < b.holes() + 1; i++)
		{
			if (b.beans(s, i) != 0)
				return i;
		}
	}
	return bestHole;
}

void SmartPlayer::smartMove(AlarmClock& ac, const Board& board, const Side player, const Side& originalPlayer,
	int& bestHole, int& value, int& counter) const
// recursive function for determining best move with a game tree
{
	// get values of beans on current side
	int currentSide = board.beansInPlay(player);
	int otherSide = board.beansInPlay(opponent(player));

	// if no more moves possible, move all beans to respective pots
	if (currentSide == 0 || otherSide == 0)
	{
		Board endGame = board;
		for (int i = 1; i < endGame.holes() + 1; i++)
		{
			endGame.moveToPot(Side::SOUTH, i, Side::SOUTH);
			endGame.moveToPot(Side::NORTH, i, Side::NORTH);
		}
		// if original player's pot has more beans, win, so value is high
		if (endGame.beans(originalPlayer, 0) > endGame.beans(opponent(originalPlayer), 0))
		{
			bestHole = -1;
			value = 999;
		}
		else // other player wins
		{
			bestHole = -1;
			value = -999;
		}
		return;
	}

	//int totalBeans = board.totalBeans();
	//int myPot = board.beans(player, 0);
	//int otherPot = board.beans(opponent(player), 0);

	// criterion by layer and timer
	if (counter > 5 || ac.timedOut()) // evaluation
	//if (ac.timedOut()) // evaluation
	{
		//int originalPlayerPot = board.beans(originalPlayer, 0);
		//int opponentPlayerPot = board.beans(opponent(originalPlayer), 0);
		//int originalPlayerSide = board.beansInPlay(originalPlayer);
		//int opponentPlayerSide = board.beansInPlay(opponent(originalPlayer));
		//value = (originalPlayerPot + originalPlayerSide) - (opponentPlayerPot + opponentPlayerSide);

		bestHole = -1;
		// value is based on player's pot subtracted by other player's pot
		value = board.beans(originalPlayer, 0) - board.beans(opponent(originalPlayer), 0);
				
		//// slightly adjusts value based on how many beans on their side (not stable)
		//if (1.0 * (originalPlayerSide + originalPlayerPot) / totalBeans >= 0.8)
		//	value += 3;
		//else if (1.0 * (originalPlayerSide + originalPlayerPot) / totalBeans > 0.5)
		//	value++;
		//else if (1.0 * (originalPlayerSide + originalPlayerPot) / totalBeans == 0.5)
		//	value += 0;
		//else if (1.0 * (originalPlayerSide + originalPlayerPot) / totalBeans >= 0.2)
		//	value --;
		//else if (1.0 * (originalPlayerSide + originalPlayerPot) / totalBeans >= 0.0)
		//	value -= 3;
		
		return;
	}

	// create temporary board
	Board playingBoard = board;
	
	for (int i = 1; i < board.holes() + 1; i++) // cycle through all spots in board
	{
		if (playingBoard.beans(player, i) == 0) // if nothing in spot go to next hole
			continue;

		// variables for recursive call (no specific value except h2=-2 which helps us get initial value)
		int h2 = -2;
		int v2 = 0;
		Side endSide = Side::NORTH;
		int endHole = -1;

		// sow on new board
		playingBoard.sow(player, i, endSide, endHole);
		
		if (endSide == player && endHole == 0) // if end turn at own pot, go again
		{
			int repeatCounter = counter; // keeps track of layer (same since repeat move)
			smartMove(ac, playingBoard, player, originalPlayer, h2, v2, repeatCounter);
		}
		else
		{
			if (endSide == player && playingBoard.beans(player, endHole) == 1
				&& playingBoard.beans(opponent(player), endHole) > 0) // do capture in the right situation 
			{
				playingBoard.moveToPot(player, endHole, player);
				playingBoard.moveToPot(opponent(player), endHole, player);
			}
			int temp_counter = counter + 1; // add 1 for next layer, criterion
			smartMove(ac, playingBoard, opponent(player), originalPlayer, h2, v2, temp_counter);
		}
		
		playingBoard = board; // unmake move

		if (bestHole == -2) // set first value
		{
			bestHole = i;
			value = v2;
		}

		if (originalPlayer == player) // if original player, compare values of greatest
		{
			if (v2 >= value)
			{
				bestHole = i;
				value = v2;
			}
		}
		else
		{
			if (v2 <= value) // if other player, compare values of least
			{
				bestHole = i;
				value = v2;
			}
		}
	}
	return;
}