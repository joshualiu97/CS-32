#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Board.h"
#include "Side.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

//==========================================================================
// AlarmClock ac(numMilliseconds);  // Set an alarm clock that will time out
//                                  // after the indicated number of ms
// if (ac.timedOut())  // Will be false until the alarm clock times out; after
//                     // that, always returns true.
//==========================================================================

#include <chrono>
#include <future>
#include <atomic>

class AlarmClock
{
public:
	AlarmClock(int ms)
	{
		m_timedOut = false;
		m_isRunning = true;
		m_alarmClockFuture = std::async([=]() {
			for (int k = 0; k < ms && m_isRunning; k++)
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			if (m_isRunning)
				m_timedOut = true;
		});
	}

	~AlarmClock()
	{
		m_isRunning = false;
		m_alarmClockFuture.get();
	}

	bool timedOut() const
	{
		return m_timedOut;
	}

	AlarmClock(const AlarmClock&) = delete;
	AlarmClock& operator=(const AlarmClock&) = delete;
private:
	std::atomic<bool> m_isRunning;
	std::atomic<bool> m_timedOut;
	std::future<void> m_alarmClockFuture;
};

// ABSTRACT BASE CLASS

class Player
{
public:
	Player(std::string name) : m_name(name) {}
	std::string name() const { return m_name; }
	virtual bool isInteractive() const { return false; }
	virtual int chooseMove(const Board& b, Side s) const = 0;
	// Every concrete class derived from this class must implement this
	// function so that if the player were to be playing side s and
	// had to make a move given board b, the function returns the move
	// the player would choose. If no move is possible, return −1.
	virtual ~Player() {}
private:
	std::string m_name;
};

// DERIVED CLASSES

class HumanPlayer : public Player
{
public:
	HumanPlayer(std::string name) : Player(name) {}
	virtual bool isInteractive() const { return true; }
	virtual int chooseMove(const Board& b, Side s) const;
};

class BadPlayer : public Player
{
public:
	BadPlayer(std::string name) : Player(name) {}
	virtual int chooseMove(const Board& b, Side s) const;
};

class SmartPlayer : public Player
{
public:
	SmartPlayer(std::string name) : Player(name) {}
	virtual int chooseMove(const Board& b, Side s) const;
private:
	void smartMove(AlarmClock& ac, const Board& board, const Side player, const Side& originalPlayer,
		int& bestHole, int& value, int& counter) const;
};

#endif