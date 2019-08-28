#include <iostream>
#include "newSequence.h"

Sequence::Sequence(int setmaxItems)
{
	m_size = 0;
	m_sequence = new ItemType[setmaxItems];
	m_maxItems = setmaxItems;
}

Sequence::~Sequence()
{
	delete [] m_sequence;
}

Sequence::Sequence(const Sequence& other)
{
	m_size = other.m_size;
	m_maxItems = other.m_maxItems;
	m_sequence = new ItemType[m_maxItems];
	for (int i = 0; i < m_size; i++)
		m_sequence[i] = other.m_sequence[i];
}

Sequence & Sequence::operator=(const Sequence & other)
{
	if (this != &other)
	{	
		Sequence temp(other);
		swap(temp);

		//delete[] m_sequence;
		//m_size = other.m_size;
		//m_maxItems = other.m_maxItems;
		//m_sequence = new ItemType[m_maxItems];
		//for (int i = 0; i < m_size; i++)
		//	m_sequence[i] = other.m_sequence[i];
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
	if (m_size < m_maxItems && pos <= m_size && pos >= 0)
	{
		int counter = m_size;
		if (m_size == 0)
		{
			m_sequence[0] = value;
			m_size++;
			return 0;
		}
		while (counter > pos)
		{
			m_sequence[counter] = m_sequence[counter - 1];
			counter--;
		}
		m_sequence[pos] = value;
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
	if (m_size < m_maxItems)
	{
		int posOfSmall = 0;
		for (posOfSmall = 0; posOfSmall < m_size; posOfSmall++)
			if (value <= m_sequence[posOfSmall])
				break;
		int counter = m_size;
		if (m_size == 0)
		{
			m_sequence[0] = value;
			m_size++;
			return 0;
		}
		while (counter > posOfSmall)
		{
			m_sequence[counter] = m_sequence[counter - 1];
			counter--;
		}
		m_sequence[posOfSmall] = value;
		m_size++;
		return posOfSmall;
	}
	return -1;
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
		int position = pos;
		while (position < m_size - 1)
		{
			m_sequence[position] = m_sequence[position + 1];
			position++;
		}
		m_size--;
		return true;
	}
	return false;
}

int Sequence::remove(const ItemType& value)
// Erase all items from the sequence that == value.  Return the
// number of items removed (which will be 0 if no item == value).
{
	int numRemoved = 0;
	int numBefore = -1;
	do {
		numBefore++;
		for (int i = 0; i < m_size; i++) // search for same
		{
			if (m_sequence[i] == value) // erase value
			{
				erase(i);
				numRemoved++;
			}
		}
	} while (numRemoved != numBefore);
	return numRemoved;
}

bool Sequence::get(int pos, ItemType& value) const
// If 0 <= pos < size(), copy into value the item at position pos
// of the sequence and return true.  Otherwise, leave value unchanged
// and return false.
{
	if (m_size == 0)
		return false;
	if (pos >= 0 && pos < m_size)
	{
		value = m_sequence[pos];
		return true;
	}
	return false;
}

bool Sequence::set(int pos, const ItemType& value)
// If 0 <= pos < size(), replace the item at position pos in the
// sequence with value and return true.  Otherwise, leave the sequence
// unchanged and return false.
{
	if (m_size == 0)
		return false;
	if (pos >= 0 && pos < m_size)
	{
		m_sequence[pos] = value;
		return true;
	}
	return false;
}

int Sequence::find(const ItemType& value) const
// Let p be the smallest integer such that value == the item at
// position p in the sequence; if no such item exists, let p be -1.
// Return p.
{
	for (int i = 0; i < m_size; i++)
	{
		if (m_sequence[i] == value)
		{
			return i;
		}
	}
	return -1;
}

void Sequence::swap(Sequence& other)
// Exchange the contents of this sequence with the other one.
{
	//// Using assignment operator
	//Sequence tempSequence = other;
	//other = *this;
	//*this = tempSequence;


	// Swapping with temp object
	ItemType* tempSequence = other.m_sequence;
	int tempSize = other.m_size;
	int tempMaxItems = other.m_maxItems;

	other.m_sequence = m_sequence;
	other.m_size = m_size;
	other.m_maxItems = m_maxItems;

	m_sequence = tempSequence;
	m_size = tempSize;
	m_maxItems = tempMaxItems;

	//std::cerr << m_maxItems << std::endl;
	//std::cerr << other.m_maxItems << std::endl;

}

void Sequence::dump() const
// Prints sequence (separated by space) and size(on a new line)
{
	int printCounter = m_size;
	int count = 0;
	while (printCounter != 0)
	{
		std::cerr << m_sequence[count] << " ";
		count++;
		printCounter--;
	}
	std::cerr << std::endl << m_size << std::endl;
}