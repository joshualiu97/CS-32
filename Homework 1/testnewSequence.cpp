#include <iostream>
#include <cassert>
#include "newSequence.h"

int main()
{
	Sequence a(1000);   // a can hold at most 1000 items
	Sequence b(5);      // b can hold at most 5 items
	Sequence c;         // c can hold at most DEFAULT_MAX_ITEMS items
	ItemType v = 0;

	// No failures inserting 5 items into b
	for (int k = 0; k < 5; k++)
		assert(b.insert(v) != -1);
	
	// Failure if we try to insert a sixth item into b
	assert(b.insert(v) == -1);
	a.swap(b);
	assert(a.insert(v) == -1 && b.insert(v) != -1);

	Sequence t1(1);
	Sequence t0(0);
	assert(t0.insert(v) == -1);
	t1.swap(t0);
	assert(t1.insert(v) == -1);
	assert(t0.insert(v) == 0);
	assert(t0.insert(v) == -1);

	for (int i = 0; i < 250; i++)
		c.insert(v);
	assert(c.size() == 250 && c.insert(v) == -1);

	Sequence something = t0;
	Sequence ao;
	ao = something;
	ItemType insertForao;
	assert(ao.erase(0) == 1 && ao.insert(0, 50) == 0);
	assert(ao.get(0, insertForao) == 1 && insertForao == 50);

	Sequence cc(ao);
	assert(cc.size() == 1 && cc.insert(v) == -1);
	assert(cc.remove(50) == 1 && cc.insert(v) == 0 && cc.insert(v) == -1);

	std::cerr << "All tests passed" << std::endl;
}