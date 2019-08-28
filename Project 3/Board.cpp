#include "Board.h"

Board::Board(int nHoles, int nInitialBeansPerHole)
// Construct a Board with the indicated number of
// holes per side(not counting the pot) and initial
// number of beans per hole.If nHoles is not positive,
// act as if it were 1; if nInitialBeansPerHole
// is negative, act as if it were 0.
{
	if (nHoles <= 0) // if nHoles is less than zero, set it to 1
		nHoles = 1;
	south = new int[nHoles + 1]; // create arrays
	north = new int[nHoles + 1];
	if (nInitialBeansPerHole < 0) // if initial number is less that 1, set it to zero
		nInitialBeansPerHole = 0;
	for (int i = 1; i < nHoles + 1; i++) // set beans for each spot
	{
		south[i] = nInitialBeansPerHole;
		north[i] = nInitialBeansPerHole;
	}
	south[0] = 0; // set pots to 0
	north[0] = 0;
	m_nHoles = nHoles;
}

int Board::beans(Side s, int hole) const
// Return the number of beans in the indicated hole or pot,
// or −1 if the hole number is invalid.
{
	if (hole < 0 || hole > m_nHoles) // if number of holes invalid, return -1
		return -1;
	if (s == Side::NORTH) // return hole of correct side
		return north[hole];
	if (s == Side::SOUTH)
		return south[hole];
	return -1; // invalid case
}

int Board::beansInPlay(Side s) const
// Return the total number of beans in all the holes on
// the indicated side, not counting the beans in the pot.
{
	if (s == Side::NORTH) // return sum of beans on side
	{
		int sum = 0;
		for (int i = 1; i < m_nHoles + 1; i++)
			sum += north[i];
		return sum;
	}
	if (s == Side::SOUTH)
	{
		int sum = 0;
		for (int i = 1; i < m_nHoles + 1; i++)
			sum += south[i];
		return sum;
	}
	return -1; // invalid case
}

int Board::totalBeans() const
// Return the total number of beans in the
// game, including any in the pots.
{
	int sum = 0;
	for (int i = 0; i < m_nHoles + 1; i++) // find sum for north
		sum += north[i];
	for (int i = 0; i < m_nHoles + 1; i++) // find sum for south
		sum += south[i];
	return sum;
}

bool Board::sow(Side s, int hole, Side& endSide, int& endHole)
// If the hole indicated by (s,hole) is empty or invalid or a pot,
// this function returns false without changing anything. Otherwise,
// it will return true after sowing the beans: the beans are removed
// from hole (s,hole) and sown counterclockwise, including s's pot if
// encountered, but skipping s's opponent's pot. The parameters endSide
// and endHole are set to the side and hole where the last bean was placed.
// (This function does not make captures or multiple turns; different Kalah
// variants have different rules about these issues, so dealing with them
// should not be the responsibility of the Board class.)
{
	if ((s != Side::NORTH && s != Side::SOUTH) || hole < 1 || hole > m_nHoles) // check if invalid
		return false;
	if (s == Side::NORTH) // check if empty
		if (north[hole] == 0)
			return false;
	if (s == Side::SOUTH)
		if (south[hole] == 0)
			return false;

	// sow
	int amount; // empty the hole and get the number of beans
	if (s == Side::NORTH)
	{
		amount = north[hole];
		north[hole] = 0;
	}
	if (s == Side::SOUTH)
	{
		amount = south[hole];
		south[hole] = 0;
	}

	Side loop = s;
	while (amount != 0)
	{
		switch (loop)
		{
		case Side::NORTH:
			hole--;
			if (hole == 0)
			{
				// if 'loop' matches the given side, drop a bean
				if (loop == s)
				{
					// add bean to norths's pot
					north[0]++;
					amount--;
					if (amount == 0) // if last bean, set end variables
					{
						endSide = loop;
						endHole = hole;
					}
				}
				// swtich loop to the other side
				if (loop == Side::NORTH)
					loop = Side::SOUTH;
				else
					loop = Side::NORTH;
			}
			else // drop a bean and continue
			{
				north[hole]++;
				amount--;
				if (amount == 0) // if last bean, set end variables
				{
					endSide = loop;
					endHole = hole;
				}
			}
			break;
		case Side::SOUTH:
			hole++;
			if (hole == m_nHoles + 1)
			{
				// if 'loop' matches the given side, drop a bean
				if (loop == s)
				{
					// add bean to south's pot
					south[0]++;
					amount--;
					if (amount == 0) // if last bean, set end variables
					{
						endSide = loop;
						endHole = 0;
					}
				}
				// swtich loop to the other side
				if (loop == Side::NORTH)
					loop = Side::SOUTH;
				else
					loop = Side::NORTH;
			}
			else // drop a bean and continue
			{
				south[hole]++;
				amount--;
				if (amount == 0) // if last bean, set end variables
				{
					endSide = loop;
					endHole = hole;
				}
			}
			break;
		default:
			break;
		}
	}
	return true;
}

bool Board::moveToPot(Side s, int hole, Side potOwner)
// If the indicated hole is invalid or a pot, return false without
// changing anything. Otherwise, move all the beans in hole (s,hole)
// into the pot belonging to potOwner and return true.
{
	if ((s != Side::NORTH && s != Side::SOUTH) || hole < 1 || hole > m_nHoles) // check if invalid
		return false;

	if (s == Side::NORTH) // get number of beans from spot
	{
		int nBeans = north[hole];
		north[hole] = 0;
		if (potOwner == Side::NORTH) // put beans in poOwner's pot
			north[0] += nBeans;
		else
			south[0] += nBeans;
		return true;
	}
	if (s == Side::SOUTH) // get number of beans from spot
	{
		int nBeans = south[hole];
		south[hole] = 0;
		if (potOwner == Side::NORTH) // put beans in poOwner's pot
			north[0] += nBeans;
		else
			south[0] += nBeans;
		return true;
	}
	return false; // safety check
}

bool Board::setBeans(Side s, int hole, int beans)
// If the indicated hole is invalid or beans is negative, this
// function returns false without changing anything. Otherwise, it
// will return true after setting the number of beans in the indicated
// hole or pot to the value of the third parameter. (This may change
// what beansInPlay and totalBeans return if they are called later.)
// This function exists solely so that we and you can more easily test
// your program: None of your code that implements the member functions
// of any class is allowed to call this function directly or
// indirectly. (We'll show an example of its use below.)
{
	if ((s != Side::NORTH && s != Side::SOUTH) || hole < 0 || hole > m_nHoles || beans < 0) // check if invalid
		return false;
	
	if (s == Side::NORTH) // find correct side and set the beans
	{
		north[hole] = beans;
		return true;
	}
	if (s == Side::SOUTH)
	{
		south[hole] = beans;
		return true;
	}
	return false;
}

Board::~Board()
// destructor
{
	delete[] north;
	delete[] south;
}

Board::Board(const Board& other)
// copy constructor
{
	m_nHoles = other.m_nHoles;
	south = new int[m_nHoles + 1];
	north = new int[m_nHoles + 1];
	for (int i = 0; i < m_nHoles + 1; i++)
	{
		south[i] = other.south[i];
		north[i] = other.north[i];
	}
}

Board & Board::operator=(const Board& other)
// assignment operator
{
	if (this != &other)
	{
		delete[] north;
		delete[] south;
		m_nHoles = other.m_nHoles;
		south = new int[m_nHoles + 1];
		north = new int[m_nHoles + 1];
		for (int i = 0; i < m_nHoles + 1; i++)
		{
			south[i] = other.south[i];
			north[i] = other.north[i];
		}
	}
	return *this;
}