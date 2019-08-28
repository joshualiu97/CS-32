#ifndef BOARD_H
#define BOARD_H

#include "Side.h"

class Board
{
public:
	Board(int nHoles, int nInitialBeansPerHole);
	Board(const Board& other);
	Board &operator=(const Board& other);
	int holes() const { return m_nHoles; }
	int beans(Side s, int hole) const;
	int beansInPlay(Side s) const;
	int totalBeans() const;
	bool sow(Side s, int hole, Side& endSide, int& endHole);
	bool moveToPot(Side s, int hole, Side potOwner);
	bool setBeans(Side s, int hole, int beans);
	~Board();
private:
	int* south;
	int* north;
	int m_nHoles;
};

#endif