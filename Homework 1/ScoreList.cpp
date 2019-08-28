#include "ScoreList.h"

#include <limits>

ScoreList::ScoreList()
// Create an empty score list.
{
	//m_numScores = 0;
}

bool ScoreList::add(unsigned long score)
// If the score is valid (a value from 0 to 100) and the score list
// has room, add it to the score list and return true.
// Otherwise, leave the score list unchanged and return false.
{
	if (score <= 100)
	{
		if (m_list.insert(score) != -1)
		{
			return true;
		}
	}
	return false;
}

bool ScoreList::remove(unsigned long score)
// Remove one instance of the specified score from the score list.
// Return true if a score was removed; otherwise false.
{
	int pos = m_list.find(score);
	if (pos != -1)
	{
		m_list.erase(pos);
		//m_numScores--;

		return true;
	}
	return false;
}

int ScoreList::size() const
// Return the number of scores in the list.
{
	return m_list.size();
}

unsigned long ScoreList::minimum() const
// Return the lowest score in the score list.  If the list is
// empty, return NO_SCORE.
{
	if (m_list.empty())
	{
		return NO_SCORE;
	}
	unsigned long min = 100;
	unsigned long temp = 0;
	for (int i = 0; i < m_list.size(); i++)
	{
		m_list.get(i, temp);
		if (temp < min)
			min = temp;
	}
	return min;
}

unsigned long ScoreList::maximum() const
// Return the highest score in the score list.  If the list is
// empty, return NO_SCORE.
{
	if (m_list.empty())
	{
		return NO_SCORE;
	}
	unsigned long max = 0;
	unsigned long temp = 0;
	for (int i = 0; i < m_list.size(); i++)
	{
		m_list.get(i, temp);
		if (temp > max)
			max = temp;
	}
	return max;
}