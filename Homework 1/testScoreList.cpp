#include <iostream>
#include <cassert>
#include "ScoreList.h"

int main()
{
	ScoreList s;
	s.add(0);
	assert(s.size() == 1);
	s.add(97);
	assert(s.size() == 2);
	s.add(98);
	assert(s.size() == 3);
	s.add(99);
	assert(s.size() == 4);
	s.add(100);
	assert(s.size() == 5);
	s.remove(0);
	assert(s.size() == 4);
	assert(s.maximum() == 100);
	assert(s.minimum() == 97);
	std::cerr << "All tests passed" << std::endl;
}