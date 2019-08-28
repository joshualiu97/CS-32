#if defined(_MSC_VER)  &&  !defined(_DEBUG)
#include <iostream>
#include <windows.h>
#include <conio.h>

struct KeepWindowOpenUntilDismissed
{
	~KeepWindowOpenUntilDismissed()
	{
		DWORD pids[1];
		if (GetConsoleProcessList(pids, 1) == 1)
		{
			std::cout << "Press any key to continue . . . ";
			_getch();
		}
	}
} keepWindowOpenUntilDismissed;
#endif

// This file contains your main routine and any implementations
// of any additional non-member functions you find useful to add.

#include <iostream>
#include <cassert>
#include "Board.h"
#include "Player.h"
#include "Game.h"
#include "Side.h"

void doGameTests()
{
	BadPlayer bp1("Bart");
	BadPlayer bp2("Homer");
	Board b(3, 0);
	b.setBeans(SOUTH, 1, 2);
	b.setBeans(NORTH, 2, 1);
	b.setBeans(NORTH, 3, 2);
	Game g(b, &bp1, &bp2);
	bool over;
	bool hasWinner;
	Side winner;
	//    Homer
	//   0  1  2
	// 0         0
	//   2  0  0
	//    Bart
	g.status(over, hasWinner, winner);
	assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 0 &&
		g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 1 && g.beans(NORTH, 3) == 2 &&
		g.beans(SOUTH, 1) == 2 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 0);

	g.move();
	//   0  1  0
	// 0         3
	//   0  1  0
	g.status(over, hasWinner, winner);
	assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
		g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 1 && g.beans(NORTH, 3) == 0 &&
		g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 1 && g.beans(SOUTH, 3) == 0);

	g.move();
	//   1  0  0
	// 0         3
	//   0  1  0
	g.status(over, hasWinner, winner);
	assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
		g.beans(NORTH, 1) == 1 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
		g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 1 && g.beans(SOUTH, 3) == 0);

	g.move();
	//   1  0  0
	// 0         3
	//   0  0  1
	g.status(over, hasWinner, winner);
	assert(!over && g.beans(NORTH, POT) == 0 && g.beans(SOUTH, POT) == 3 &&
		g.beans(NORTH, 1) == 1 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
		g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 1);

	g.move();
	//   0  0  0
	// 1         4
	//   0  0  0
	g.status(over, hasWinner, winner);
	assert(over && g.beans(NORTH, POT) == 1 && g.beans(SOUTH, POT) == 4 &&
		g.beans(NORTH, 1) == 0 && g.beans(NORTH, 2) == 0 && g.beans(NORTH, 3) == 0 &&
		g.beans(SOUTH, 1) == 0 && g.beans(SOUTH, 2) == 0 && g.beans(SOUTH, 3) == 0);
	assert(hasWinner && winner == SOUTH);
}

void doPlayerTests()
{
	HumanPlayer hp("Marge");
	assert(hp.name() == "Marge"  &&  hp.isInteractive());
	BadPlayer bp("Homer");
	assert(bp.name() == "Homer" && !bp.isInteractive());
	SmartPlayer sp("Lisa");
	assert(sp.name() == "Lisa" && !sp.isInteractive());
	Board b(3, 2);
	b.setBeans(SOUTH, 2, 0);
	std::cout << "=========" << std::endl;
	int n = hp.chooseMove(b, SOUTH);
	std::cout << "=========" << std::endl;
	assert(n == 1 || n == 3);
	n = bp.chooseMove(b, SOUTH);
	assert(n == 1 || n == 3);
	n = sp.chooseMove(b, SOUTH);
	assert(n == 1 || n == 3);
}

void doBoardTests()
{
	Board b(3, 2);
	assert(b.holes() == 3 && b.totalBeans() == 12 &&
		b.beans(SOUTH, POT) == 0 && b.beansInPlay(SOUTH) == 6);
	b.setBeans(SOUTH, 1, 1);
	b.moveToPot(SOUTH, 2, SOUTH);
	assert(b.totalBeans() == 11 && b.beans(SOUTH, 1) == 1 &&
		b.beans(SOUTH, 2) == 0 && b.beans(SOUTH, POT) == 2 &&
		b.beansInPlay(SOUTH) == 3);
	Side es;
	int eh;
	b.sow(SOUTH, 3, es, eh);
	assert(es == NORTH && eh == 3 && b.beans(SOUTH, 3) == 0 &&
		b.beans(NORTH, 3) == 3 && b.beans(SOUTH, POT) == 3 &&
		b.beansInPlay(SOUTH) == 1 && b.beansInPlay(NORTH) == 7);
}

int main()
{
	
//=========TEST CASES FOR CLASS BOARD===========================================================================================================
	
	Board testingA(3, 0);
	Board testingB(100, 30);
	Board testingC(-1, -1);
	Board testingD(3, 3);
	Board testingE(3, 50);
	Board testingF(3, 3);
	Board testingG(6, 20);
	Board testingH(4, 4);
	Board testingI(2, 2);

	assert(testingA.holes() == 3); // test number of holes
	assert(testingC.holes() == 1); // test number of holes when negative
	assert(testingC.beans(NORTH, 1) == 0); // test number of beans when negative
	assert(testingA.beans(NORTH, 2) == 0); // test number of beans when creating board
	assert(testingA.beans(NORTH, 0) == 0); // test number of beans at pot
	assert(testingB.beans(SOUTH, 100) == 30); // test beans regular inputs
	assert(testingB.beans(SOUTH, 101) == -1); // invalid spot, greater than number of holes
	assert(testingB.beans(SOUTH, -1) == -1); // invalid spot, negative holes
	assert(testingB.beansInPlay(SOUTH) == 3000); // test number of beans with south
	assert(testingB.beansInPlay(NORTH) == 3000); // test number of beans with north
	assert(testingA.beansInPlay(SOUTH) == 0); // test number of beans in north
	assert(testingA.beansInPlay(NORTH) == 0); // test number of beans in south
	Side whichSideItsOn;
	int endingPositionOfSow;
	assert(testingD.sow(NORTH, 1, whichSideItsOn, endingPositionOfSow) == true
		&& whichSideItsOn == Side::SOUTH && endingPositionOfSow == 2
		&& testingD.beans(NORTH, 0) == 1);// sow regularly
	assert(testingE.sow(NORTH, 1, whichSideItsOn, endingPositionOfSow) == true
		&& whichSideItsOn == Side::NORTH && endingPositionOfSow == 0
		&& testingE.beans(NORTH, 0) == 8 && testingE.beans(SOUTH, 0) == 0); // sow regularly
	assert(testingF.sow(NORTH, -1, whichSideItsOn, endingPositionOfSow) == false
		&& whichSideItsOn == Side::NORTH && endingPositionOfSow == 0
		&& testingF.beans(NORTH, 0) == 0 && testingF.beans(SOUTH, 0) == 0); // cannot sow with negative position, do nothing and return false
	assert(testingG.sow(SOUTH, 5, whichSideItsOn, endingPositionOfSow) == true
		&& whichSideItsOn == Side::NORTH && endingPositionOfSow == 2
		&& testingG.beans(NORTH, 0) == 0 && testingG.beans(SOUTH, 0) == 2
		&& testingG.beans(SOUTH, 5) == 1); // sow regularly from south side
	assert(testingH.moveToPot(NORTH, 3, NORTH) == true
		&& testingH.moveToPot(SOUTH, 3, NORTH) == true
		&& testingH.beans(NORTH, 3) == 0 && testingH.beans(SOUTH, 3) == 0
		&& testingH.beans(NORTH, 0) == 8 && testingH.beans(SOUTH, 0) == 0); // moveToPot tested twice, and values add up correctly
	assert(testingH.moveToPot(NORTH, -1, NORTH) == false
		&& testingH.moveToPot(SOUTH, 5, NORTH) == false
		&& testingH.beans(NORTH, 3) == 0 && testingH.beans(SOUTH, 3) == 0
		&& testingH.beans(NORTH, 0) == 8 && testingH.beans(SOUTH, 0) == 0); // moveToPot tested with negative numbers, do nothing and return false
	assert(testingI.setBeans(NORTH, 2, 100) == true
		&& testingI.beans(NORTH, 2) == 100
		&& testingI.setBeans(SOUTH, 1, -4) == false
		&& testingI.beans(SOUTH, 2) == 2); // test setBeans, normally as well as setting negative number (where it should do nothing and return false)
	
	std::cerr << "Passed all of Joshua's Test Cases for class Board." << std::endl;

//=================================================================================================================================================

//=============TEST CASES FOR PLAYER===============================================================================================================

	std::cerr << std::endl;
	
	HumanPlayer Human_Player_A("humanA");
	Board playerBoard_A(4, 4);
	assert(Human_Player_A.isInteractive() == true);
	
	//std::cout << Human_Player_A.chooseMove(playerBoard_A, NORTH) << std::endl;
	
	BadPlayer Bad_Player_A("badA");
	assert(Bad_Player_A.isInteractive() == false);

	//std::cout << Bad_Player_A.chooseMove(playerBoard_A, NORTH) << std::endl;
	//std::cout << Bad_Player_A.chooseMove(playerBoard_A, SOUTH) << std::endl;

	std::cerr << "Passed all of Joshua's test for class Player." << std::endl;

//=================================================================================================================================================

//============TEST CASES FOR GAME==================================================================================================================

	// test status function with unfinished game
	Board gameBoardA(6, 2);
	BadPlayer gameBP_A("GBP_A");
	BadPlayer gameBP_B("GBP_B");
	Game gameInstanceA(gameBoardA, &gameBP_A, &gameBP_B);
	gameInstanceA.display();
	bool gameInstanceA_over = false;
	bool gameInstanceA_hasWinner = false;
	Side gameInstanceA_winner = NORTH;
	gameInstanceA.status(gameInstanceA_over,
		gameInstanceA_hasWinner, gameInstanceA_winner);
	assert(gameInstanceA_over == false && gameInstanceA_hasWinner == false
		&& gameInstanceA_winner == NORTH);

	// test status function with finished game, no winner
	Board gameBoardB(6, 0);
	BadPlayer gameBP_C("GBP_C");
	BadPlayer gameBP_D("GBP_D");
	Game gameInstanceB(gameBoardB, &gameBP_C, &gameBP_D);
	gameInstanceB.display();
	bool gameInstanceB_over = false;
	bool gameInstanceB_hasWinner = false;
	Side gameInstanceB_winner = NORTH;
	gameInstanceB.status(gameInstanceB_over,
		gameInstanceB_hasWinner, gameInstanceB_winner);
	assert(gameInstanceB_over == true && gameInstanceB_hasWinner == false
		&& gameInstanceB_winner == NORTH);

	// test status with winner
	Board gameBoardC(6, 0);
	gameBoardC.setBeans(SOUTH, 0, 15);
	BadPlayer gameBP_E("GBP_E");
	BadPlayer gameBP_F("GBP_F");
	Game gameInstanceC(gameBoardC, &gameBP_E, &gameBP_F);
	gameInstanceC.display();
	bool gameInstanceC_over = false;
	bool gameInstanceC_hasWinner = false;
	Side gameInstanceC_winner = NORTH;
	gameInstanceC.status(gameInstanceC_over,
		gameInstanceC_hasWinner, gameInstanceC_winner);
	assert(gameInstanceC_over == true && gameInstanceC_hasWinner == true
		&& gameInstanceC_winner == SOUTH);

//=============================================================================================================================================================

//==========TESTING MOVE=======================================================================================================================================

	doGameTests();
	doPlayerTests();
	doBoardTests();
	std::cout << "Passed all tests" << std::endl;

	// Testing SmartPlayer
	SmartPlayer SmartPlayer_1("Josh");
	BadPlayer BadPlayer_1("Melody");
	Board PlayerBoard_1(6, 4);
	Game g(PlayerBoard_1, &BadPlayer_1, &SmartPlayer_1);
	g.play();
	bool over, hasWinner;
	Side winner;

	g.status(over, hasWinner, winner);
	assert(over == true && hasWinner == true && winner == NORTH); // test to see if SmartPlayer can beat BadPlayer on 6 hole board with 4 beans in each hole
	
	Board PlayerBoard_2(4, 4);
	Game g2(PlayerBoard_2, &BadPlayer_1, &SmartPlayer_1);
	g2.play();
	g2.status(over, hasWinner, winner);
	assert(over == true && hasWinner == true && winner == NORTH); // test to see if SmartPlayer can beat BadPlayer on 2 hole board with 2 beans in each hole
	
	std::cerr << "Passed all asserts!" << std::endl;
}





//#if defined(_MSC_VER)  &&  !defined(_DEBUG)
//#include <iostream>
//#include <windows.h>
//#include <conio.h>
//
//struct KeepWindowOpenUntilDismissed
//{
//	~KeepWindowOpenUntilDismissed()
//	{
//		DWORD pids[1];
//		if (GetConsoleProcessList(pids, 1) == 1)
//		{
//			std::cout << "Press any key to continue . . . ";
//			_getch();
//		}
//	}
//} keepWindowOpenUntilDismissed;
//#endif
//
//// This file contains your main routine and any implementations
//// of any additional non-member functions you find useful to add.
//
//#include <iostream>
//#include <cassert>
//#include "Board.h"
//#include "Player.h"
//#include "Game.h"
//#include "Side.h"
//
//int main()
//{
//	SmartPlayer p1("Josh");
//	BadPlayer p2("Melody");
//	Board b(4, 2);
//	Game g(b, &p1, &p2);
//	g.play();
//}