#include "Game.h"

//Game::Game(const Board& b, Player* south, Player* north)
// Construct a Game to be played with the indicated players on a copy
// of the board b.The player on the south side always moves first.

void Game::display() const
// Display the game's board in a manner of your choosing, provided
// you show the names of the players and a reasonable representation
// of the state of the board.
{
	std::cout << std::endl;

	std::cout << "              " << m_pNorth->name() << std::endl;

	std::cout << "          ";

	for (int i = 1; i < m_board.holes() + 1; i++)
		std::cout << m_board.beans(Side::NORTH, i) << " ";
	
	std::cout << std::endl;

	std::cout << m_board.beans(Side::NORTH, 0)
		<< "                              " <<
		m_board.beans(Side::SOUTH, 0);

	std::cout << std::endl;

	std::cout << "          ";
	for (int i = 1; i < m_board.holes() + 1; i++)
		std::cout << m_board.beans(Side::SOUTH, i) << " ";

	std::cout << std::endl;

	std::cout << "              " << m_pSouth->name() << std::endl;

	std::cout << std::endl;

	std::cout << std::endl;
}

void Game::status(bool& over, bool& hasWinner, Side& winner) const
// If the game isn't over (i.e., more moves are possible), set over to
// false and do not change anything else. Otherwise, set over to true and
// hasWinner to true if the game has a winner, or false if it resulted in
// a tie. If hasWinner is set to false, leave winner unchanged; otherwise,
// set it to the winning side.
{
	if (m_board.beansInPlay(Side::NORTH) != 0 && m_board.beansInPlay(Side::SOUTH) != 0) // game not over, set over to false
	{
		over = false;
		return;
	}
	else
	{
		over = true; // game over, over == true
		if (m_board.beans(Side::NORTH, 0) == m_board.beans(Side::SOUTH, 0)) // if no winner, then hasWinner == false
		{
			hasWinner = false;
			return;
		}
		else // if there is winner, set winner
		{
			hasWinner = true;
			if (m_board.beans(Side::NORTH, 0) > m_board.beans(Side::SOUTH, 0))
			{
				winner = Side::NORTH;
			}
			else
				winner = Side::SOUTH;
		}
	}
}

bool Game::move()
// If the game is over, return false.Otherwise, make a complete
// move for the player whose turn it is(so that it becomes the other
// player's turn) and return true. "Complete" means that the player
// sows the seeds from a hole and takes any additional turns required
// or completes a capture. If the player gets an additional turn, you
// should display the board so someone looking at the screen can
// follow what's happening.
{
	bool over = false; // make sure game is not over
	bool hasWinner = false;
	Side winner = Side::NORTH;
	status(over, hasWinner, winner);
	if (over) // if game is over return false
	{
		// move all the beans to the respective pots
		for (int i = 1; i < m_board.holes() + 1; i++)
			m_board.moveToPot(Side::NORTH, i, Side::NORTH);
		for (int i = 1; i < m_board.holes() + 1; i++)
			m_board.moveToPot(Side::SOUTH, i, Side::SOUTH);
		return false;
	}
	
	int move = 0;
	Side endSide = Side::NORTH;
	int endHole = 0;
	bool addTurn = false; // see if bean lands in own pot
	
	do {
		status(over, hasWinner, winner);
		if (over)
		{
			// move all the beans to the respective pots
			for (int i = 1; i < m_board.holes() + 1; i++)
				m_board.moveToPot(Side::NORTH, i, Side::NORTH);
			for (int i = 1; i < m_board.holes() + 1; i++)
				m_board.moveToPot(Side::SOUTH, i, Side::SOUTH);
			return false;
		}
		int s = m_turn % 2; // get turn
		if (addTurn) // if player gets another turn, let them know
		{
			switch (s)
			{
			case(0):
				std::cout << m_pSouth->name() << " gets another turn!" << std::endl;
				break;
			case(1):
				std::cout << m_pNorth->name() << " gets another turn!" << std::endl;
				break;
			default:
				break;
			}
		}

		addTurn = false;
		switch (s) // determine who goes (even is Side::SOUTH, odd is Side::NORTH)
		{
		case(0): // EVEN so south goes
			move = m_pSouth->chooseMove(m_board, Side::SOUTH);
			m_board.sow(Side::SOUTH, move, endSide, endHole);
			if (endSide == Side::SOUTH && endHole == 0) // if end turn at own pot, go again
			{
				addTurn = true;
				display();
				m_turn--;
			}
			else if (endSide == Side::SOUTH && m_board.beans(Side::SOUTH, endHole) == 1 && m_board.beans(Side::NORTH, endHole) > 0) // do capture in the right situation 
			{
				std::cout << m_pSouth->name() << " performs a capture!" << std::endl;
				m_board.moveToPot(Side::SOUTH, endHole, Side::SOUTH);
				m_board.moveToPot(Side::NORTH, endHole, Side::SOUTH);
			}
			m_turn++;
			break;
		case(1): // ODD so north goes
			move = m_pNorth->chooseMove(m_board, Side::NORTH);
			m_board.sow(Side::NORTH, move, endSide, endHole);
			if (endSide == Side::NORTH && endHole == 0) // if end turn at own pot, go again
			{
				addTurn = true;
				display();
				m_turn--;
			}
			else if (endSide == Side::NORTH && m_board.beans(Side::NORTH, endHole) == 1 && m_board.beans(Side::SOUTH, endHole) > 0) // do capture in the right situation 
			{
				std::cout << m_pNorth->name() << " performs a capture!" << std::endl;
				m_board.moveToPot(Side::NORTH, endHole, Side::NORTH);
				m_board.moveToPot(Side::SOUTH, endHole, Side::NORTH);
			}
			m_turn++;
			break;
		default:
			break;
		}
	} while (addTurn == true); // go again if ended move in own pot
	display(); // update user on game
	return true;
}

void Game::play()
// Play the game. Display the progress of the game in a manner of your
// choosing, provided that someone looking at the screen can follow
// what's happening. If neither player is interactive, then to keep
// the display from quickly scrolling through the whole game, it would
// be reasonable periodically to prompt the viewer to press ENTER
// to continue and not proceed until ENTER is pressed. (The ignore
// function for input streams is useful here.) Announce the winner
// at the end of the game. You can apportion to your liking the
// responsibility for displaying the board between this function and
// the move function. (Note: If when this function is called, South
// has no beans in play, so can't make the first move, sweep any beans
// on the North side into North's pot and act as if the game is thus over.)
{
	std::cout << "Welcome, " << m_pSouth->name() << "!" << std::endl;
	std::cout << "Welcome, " << m_pNorth->name() << "!" << std::endl;
	display();

	bool over = false; // variables to see if game is over
	bool hasWinner = false;
	Side winner = Side::NORTH;
	
	if (m_board.beansInPlay(Side::SOUTH) == 0) // if South has no beans in play, return winner
	{
		for (int i = 1; i < m_board.holes() + 1; i++)
			m_board.moveToPot(Side::NORTH, i, Side::NORTH);
		status(over, hasWinner, winner);
		if (hasWinner == true)
			std::cout << m_pNorth->name() << " has won!" << std::endl;
		else
			std::cout << "Tie!" << std::endl;
		return;
	}
	std::cout << "It is " << m_pSouth->name() << "'s turn!" << std::endl;

	std::cout << "Press ENTER to continue!" << std::endl; // prompt user to press ENTER
	do {
		m_again = std::cin.get();
	} while (m_again != '\n');	// move while prompting user to press ENTER to make display readable

	while (move()) // make the player move
	{
		if (m_turn % 2 == 0)
			std::cout << "It is " << m_pSouth->name() << "'s turn!" << std::endl;
		else
			std::cout << "It is " << m_pNorth->name() << "'s turn!" << std::endl;

		std::cout << "Press ENTER to continue!" << std::endl; // prompt user to press ENTER
		do {
			m_again = std::cin.get();
		} while (m_again != '\n');
	}

	display(); // show end result

	// game is over
	status(over, hasWinner, winner);
	if (hasWinner == true) // say the winner
		if (winner == Side::NORTH)
			std::cout << m_pNorth->name() << " has won!" << std::endl;
		else
			std::cout << m_pSouth->name() << " has won!" << std::endl;
	else
		std::cout << "Tie!" << std::endl;
	return;
}

int Game::beans(Side s, int hole) const
// Return the number of beans in the indicated hole or pot of
// the game's board, or −1 if the hole number is invalid. This
// function exists so that we and you can more easily test your program.
{
	return m_board.beans(s, hole);
}