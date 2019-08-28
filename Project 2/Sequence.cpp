#include <iostream>
#include "Sequence.h"

Sequence::Sequence()
	: m_size(0)
// Constructor
{
	head = dummy;
	dummy->m_next = dummy;
	dummy->m_prev = dummy; // make dummy a circular list
}

Sequence::~Sequence()
// Destructor
{
	Node* p;
	p = dummy->m_prev; // delete remaining Nodes
	for (int i = 0; i < m_size; i++)
	{
		p = p->m_prev;
		delete p->m_next;
	}
	delete p; // delete last Node
}

Sequence::Sequence(const Sequence& other)
	: m_size(0)
// Copy constructor
{
	head = dummy;
	dummy->m_next = dummy;
	dummy->m_prev = dummy;
	for (int i = 0; i < other.m_size; i++) // copy values into current Sequence from other
	{
		ItemType readValue;
		other.get(i, readValue);
		insert(i, readValue);
	}
}

Sequence & Sequence::operator=(const Sequence & other)
// Assignment operator
{
	if (this != &other) // assignment operator with copy constructor and swap
	{
		Sequence temp(other);
		swap(temp);
	}
	return *this;
}

bool Sequence::empty() const
// Return true if the sequence is empty, otherwise false.
{
	if (m_size == 0)
		return true;
	return false;
}

int Sequence::size() const
// Return the number of items in the sequence.
{
	return m_size;
}

int Sequence::insert(int pos, const ItemType& value)
// Insert value into the sequence so that it becomes the item at
// position pos.  The original item at position pos and those that
// follow it end up at positions one higher than they were at before.
// Return pos if 0 <= pos <= size() and the value could be
// inserted.  (It might not be, if the sequence has a fixed capacity,
// e.g., because it's implemented using a fixed-size array.)  Otherwise,
// leave the sequence unchanged and return -1.  Notice that
// if pos is equal to size(), the value is inserted at the end.
{
	if (pos <= m_size && pos >= 0)
	{		
		Node* p; // pointer that moves
		p = head; 
		for (int i = 0; i < pos; i++)
			p = p->m_next;
		
		Node* insert = new Node;
		insert->m_data = value;
		
		// set m_next pointers
		insert->m_next = p->m_next; // insert points to next element
		p->m_next = insert; // previous element points to insert
		
		// set m_prev pointers
		insert->m_next->m_prev = insert;
		insert->m_prev = p;

		m_size++;
		return pos;
	}
	return -1;
}

int Sequence::insert(const ItemType& value)
// Let p be the smallest integer such that value <= the item at
// position p in the sequence; if no such item exists (i.e.,
// value > all items in the sequence), let p be size().  Insert
// value into the sequence so that it becomes the item at position
// p.  The original item at position p and those that follow it end
// up at positions one higher than before.  Return p if the value
// was actually inserted.  Return -1 if the value was not inserted
// (perhaps because the sequence has a fixed capacity and is full).
{
	Node* p; // pointer that moves
	p = head;
	int pos = 0;

	p = p->m_next; // move p to position 0
	
	for (pos = 0; pos < m_size; pos++)
	{
		if (value <= p->m_data) // compare values and insert
		{
			insert(pos, value);
			return pos;
		}
		p = p->m_next;
	}
	insert(pos, value);
	return pos;
}

bool Sequence::erase(int pos)
// If 0 <= pos < size(), remove the item at position pos from
// the sequence (so that all items that followed that item end up at
// positions one lower than they were at before), and return true.
// Otherwise, leave the sequence unchanged and return false.
{
	if (m_size == 0)
		return false;
	if (pos >= 0 && pos < m_size)
	{
		Node* p; // pointer than moves
		p = head;

		for (int i = 0; i <= pos; i++)
			p = p->m_next;

		// set pointers for erasing an Node
		p->m_prev->m_next = p->m_next;
		p->m_next->m_prev = p->m_prev;

		delete p;
		m_size--;
		return true;
	}
	return false;
}

int Sequence::remove(const ItemType& value)
// Erase all items from the sequence that == value.  Return the
// number of items removed (which will be 0 if no item == value).
{
	Node* p;
	p = head;
	p = p->m_next; // point to first element
	int counter = 0;

	for (int i = 0; i < m_size; i++)
	{
		if (p->m_data == value)
		{
			Node* temp; // create temp pointer to p(since p goes away when deleted)
			temp = p->m_prev;
			erase(i);
			counter++;
			i = i - 2; // go back to previous spot(-2 beacuse i increments by 1 after loop)
			p = temp; // set p back to the previous Node
		}
		else
			p = p->m_next;
	}
	return counter;
}

bool Sequence::get(int pos, ItemType& value) const
// If 0 <= pos < size(), copy into value the item at position pos
// of the sequence and return true.  Otherwise, leave value unchanged
// and return false.
{
	if (pos >= 0 && pos < m_size)
	{
		Node* p;
		p = head->m_next; // point to position 0

		for (int i = 0; i < pos; i++) // go to position given
			p = p->m_next;
		value = p->m_data; // set value
		return true;
	}
	return false;
}

bool Sequence::set(int pos, const ItemType& value)
// If 0 <= pos < size(), replace the item at position pos in the
// sequence with value and return true.  Otherwise, leave the sequence
// unchanged and return false.
{
	if (pos >= 0 && pos < m_size)
	{
		Node* p;
		p = head->m_next; // p points at position 0
		
		for (int i = 0; i < pos; i++) // go to position given
			p = p->m_next;
		p->m_data = value;
		return true;
	}
	return false;
}

int Sequence::find(const ItemType& value) const
// Let p be the smallest integer such that value == the item at
// position p in the sequence; if no such item exists, let p be -1.
// Return p.
{
	Node* point;
	point = head->m_next; // let point point to position 0
	for (int i = 0; i < m_size; i++)
	{
		if (point->m_data == value) // if values are same return position
			return i;
		point = point->m_next;
	}
	return -1;
}

void Sequence::swap(Sequence& other)
// Exchange the contents of this sequence with the other one.
{
	// Swapping with temp object
	Node* tempdummy = other.dummy;
	Node* temphead = other.head;
	int tempSize = other.m_size;
	
	other.dummy = dummy;
	other.head = head;
	other.m_size = m_size;
	
	dummy = tempdummy;
	head = temphead;
	m_size = tempSize;
}

void Sequence::dump() const
// Prints sequence (separated by space) and size(on a new line)
{
	int count = 0;
	for (Node* p = head->m_next; count != m_size; p = p->m_next, count++)
	{
		std::cerr << p->m_data << " ";
	}
	std::cerr << std::endl << m_size << std::endl;
}

int subsequence(const Sequence& seq1, const Sequence& seq2)
{
	if (seq1.size() < seq2.size())
		return -1;
	int seq1c = 0;
	int seq1size = seq1.size();
	for (int i = 0; i < seq1size; i++) // loops through seq1
	{	
		ItemType checkS1;
		ItemType checkS2;
		seq1.get(seq1c, checkS1);
		seq2.get(0, checkS2);
		if (checkS1 == checkS2) // first element of seq2==seq1
		{
			int j;
			for (j = 1; j < seq2.size(); j++) // loop through seq2
			{
				seq1.get(j + seq1c, checkS1);
				seq2.get(j, checkS2);
				if (checkS1 != checkS2)
					break;
			}
			if (j == seq2.size())
				return seq1c;
		}
		seq1c++;
	}
	return -1;
}

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result)
{
	// empty result
	Sequence storeResults;
	// check when sequences are empty
	if (seq1.size() == 0 && seq2.size() == 0)
	{
		result = storeResults;
		return;
	}
	if (seq1.size() == 0)
	{
		storeResults = seq2;
		result = storeResults;
		return;
	}

	if (seq2.size() == 0)
	{
		storeResults = seq1;
		result = storeResults;
		return;
	}

	// find larger sequence
	int seq1len = seq1.size();
	int seq2len = seq2.size();
	int maxlen;
	if (seq1len > seq2len)
		maxlen = seq1len;
	else
		maxlen = seq2len;

	int insertCounter = 0;
	int seq1c = 0;
	int seq2c = 0;
	for (int i = 0; i < maxlen; i++) // loop with maxlen and insert accordingly
	{
		ItemType value;
		if (seq1c < seq1len)
		{
			seq1.get(seq1c, value);
			storeResults.insert(insertCounter, value);
			seq1c++;
			insertCounter++;
		}
		if (seq2c < seq2len)
		{
			seq2.get(seq2c, value);
			storeResults.insert(insertCounter, value);
			seq2c++;
			insertCounter++;
		}
	}
	result = storeResults;
	return;
}