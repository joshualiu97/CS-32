#ifndef GAME_H
#define GAME_H

#include "Side.h"
#include "Board.h"
#include "Player.h"
#include <iostream>
#include <string>

class Game
{
public:
	Game(const Board& b, Player* south, Player* north) : m_board(b), m_pNorth(north), m_pSouth(south), m_turn(0) {}
	void display() const;
	void status(bool& over, bool& hasWinner, Side& winner) const;
	bool move();
	void play();
	int beans(Side s, int hole) const;
private:
	Board m_board;
	Player* m_pNorth;
	Player* m_pSouth;
	int m_turn;
	char m_again;
};

#endif