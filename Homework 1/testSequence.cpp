#include <iostream>
#include <cassert>
#include "Sequence.h"

int main()
{
	Sequence s;
	s.insert(0, 1);
	s.insert(1, 2);
	s.insert(2, 3);
	s.insert(3, 2);
	s.insert(4, 5);
	assert(s.remove(2) == 2);
	assert(s.size() == 3);
	ItemType x;
	assert(s.get(0, x) && x == 1);
	assert(s.get(1, x) && x == 3);
	assert(s.get(2, x) && x == 5);

	Sequence s1;
	s1.insert(0, 10);
	s1.insert(0, 11);
	Sequence s2;
	s2.insert(0, 12);
	s1.swap(s2);
	assert(s1.size() == 1 && s1.find(12) == 0 && s2.size() == 2 &&
		s2.find(11) == 0 && s2.find(10) == 1);

	Sequence t;
	t.insert(0, 20);
	t.insert(1, 21);
	t.insert(2, 22);
	t.insert(3, 23);

	Sequence something = t;
	t = s2;
	t = something;

	assert(t.find(22) == 2);
	t.remove(20);
	assert(t.size() == 3 && t.find(21) == 0 && t.find(22) == 1 &&
		t.find(23) == 2);

	// Test empty sequence, size(), and empty()
	Sequence a;
	assert(a.size() == 0 && a.empty() == 1);

	/*
	// Test assignment operator and copy constructor
	Sequence c1 = t;
	c1 = Sequence(t);
	assert(c1.size() == 3 && c1.find("pita") == 0 && c1.find("") == 1 &&
		c1.find("matzo") == 2);
	*/

	// Test functions
	Sequence testset;
	assert(testset.set(73, 0) == 0);
	Sequence h;
	h.insert(90);
	h.insert(91);
	h.insert(93);
	h.insert(90);
	h.insert(91);
	h.insert(0, 80);
	h.insert(6, 81);
	ItemType test_h1;
	ItemType test_h2;
	assert(h.size() == 7 && h.empty() != 1 && h.get(0, test_h1) == 1 && h.get(7, test_h1) == 0);
	assert(test_h1 == 80 && h.get(6, test_h2) == 1 && test_h2 == 81);
	assert(h.set(0, 82) == 1 && h.find(82) == 0);
	assert(h.erase(0) == 1 && h.size() == 6);
	assert(h.remove(91) == 2 && h.size() == 4);
	
	Sequence b;
	b.insert(0, 50);
	b.insert(0, 51);
	assert(b.size() == 2);
	ItemType testing = 50;
	assert(b.get(0, testing) && testing == 51);
	assert(b.get(1, testing) && testing == 50);

	std::cerr << "All tests passed" << std::endl;
}