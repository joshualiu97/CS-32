#include <iostream>
#include <stack>
using namespace std;

bool pathExists(char maze[][10], int sr, int sc, int er, int ec);

class Coord
{
public:
	Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
	int r() const { return m_r; }
	int c() const { return m_c; }
private:
	int m_r;
	int m_c;
};

bool pathExists(char maze[][10], int sr, int sc, int er, int ec)
{
	stack<Coord> coordStack;
	coordStack.push(Coord(sr, sc)); // push starting coords and update maze
	maze[sr][sc] = '0';

	while (!coordStack.empty())
	{
		Coord popped = coordStack.top(); // pop current coords, if matches end coords, return true
		coordStack.pop();
		//cerr << popped.r() << " " << popped.c() << endl;

		if (popped.r() == er && popped.c() == ec)
			return true;
		// if we can move SOUTH add the coord and update it
		if (popped.r() < 9 && maze[popped.r() + 1][popped.c()] == '.')
		{
			coordStack.push(Coord(popped.r() + 1, popped.c()));
			maze[popped.r() + 1][popped.c()] = '0';
		}
		// if we can move WEST add the coord and update it
		if (popped.c() > 0 && maze[popped.r()][popped.c() - 1] == '.')
		{
			coordStack.push(Coord(popped.r(), popped.c() - 1));
			maze[popped.r()][popped.c() - 1] = '0';
		}
		// if we can move NORTH add the coord and update it
		if (popped.r() > 0 && maze[popped.r() - 1][popped.c()] == '.')
		{
			coordStack.push(Coord(popped.r() - 1, popped.c()));
			maze[popped.r() - 1][popped.c()] = '0';
		}
		// if we can move EAST add the coord and update it
		if (popped.c() < 9 && maze[popped.r()][popped.c() + 1] == '.')
		{
			coordStack.push(Coord(popped.r(), popped.c() + 1));
			maze[popped.r()][popped.c() + 1] = '0';
		}
	}
	//	no solution so return false
	return false;
}

int main()
{
	char maze[10][10] = {
		{ 'X','X','X','X','X','X','X','X','X','X' },
		{ 'X','.','.','.','.','X','.','.','.','X' },
		{ 'X','.','X','X','.','X','X','X','.','X' },
		{ 'X','X','X','.','.','.','.','X','.','X' },
		{ 'X','.','X','X','X','.','X','X','X','X' },
		{ 'X','.','X','.','.','.','X','.','.','X' },
		{ 'X','.','.','.','X','.','X','.','.','X' },
		{ 'X','X','X','X','X','.','X','.','X','X' },
		{ 'X','.','.','.','.','.','.','.','.','X' },
		{ 'X','X','X','X','X','X','X','X','X','X' }
	};

	if (pathExists(maze, 3, 5, 8, 8))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;
}