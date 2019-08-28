#include "History.h"
#include <iostream>
using namespace std;

History::History(int nRows, int nCols)
{
	m_rows = nRows;
	m_cols = nCols;
	for (int r = 0; r < m_rows; r++)
		for (int c = 0; c < m_cols; c++)
			m_grid[r][c] = '.';
}

bool History::record(int r, int c)
{
	int recRow = r - 1;
	int recCol = c - 1;
	if (recRow < 0 || recRow >= m_rows)
	{
		return false;
	}
	if (recCol < 0 || recCol >= m_cols)
	{
		return false;
	}
	if (m_grid[recRow][recCol] == '.')
	{
		m_grid[recRow][recCol] = 64;
	}
	if (m_grid[recRow][recCol] == 90)
	{
		return true;
	}
	m_grid[recRow][recCol]++;
	return true;
}

void History::display() const
{
	clearScreen();
	for (int r = 0; r < m_rows; r++)
	{
		for (int c = 0; c < m_cols; c++)
			cout << m_grid[r][c];
		cout << endl;
	}
	cout << endl;
}