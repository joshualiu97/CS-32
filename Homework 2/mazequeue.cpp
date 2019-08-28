#include <iostream>
#include <queue>
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
	queue<Coord> coordQueue;
	coordQueue.push(Coord(sr, sc)); // push starting coords and update maze
	maze[sr][sc] = '0';

	while (!coordQueue.empty())
	{
		Coord popped = coordQueue.front(); // pop current coords, if matches end coords, return true
		coordQueue.pop();
		//cerr << popped.r() << " " << popped.c() << endl;

		if (popped.r() == er && popped.c() == ec)
			return true;
		// if we can move SOUTH add the coord and update it
		if (popped.r() < 9 && maze[popped.r() + 1][popped.c()] == '.')
		{
			coordQueue.push(Coord(popped.r() + 1, popped.c()));
			maze[popped.r() + 1][popped.c()] = '0';
		}
		// if we can move WEST add the coord and update it
		if (popped.c() > 0 && maze[popped.r()][popped.c() - 1] == '.')
		{
			coordQueue.push(Coord(popped.r(), popped.c() - 1));
			maze[popped.r()][popped.c() - 1] = '0';
		}
		// if we can move NORTH add the coord and update it
		if (popped.r() > 0 && maze[popped.r() - 1][popped.c()] == '.')
		{
			coordQueue.push(Coord(popped.r() - 1, popped.c()));
			maze[popped.r() - 1][popped.c()] = '0';
		}
		// if we can move EAST add the coord and update it
		if (popped.c() < 9 && maze[popped.r()][popped.c() + 1] == '.')
		{
			coordQueue.push(Coord(popped.r(), popped.c() + 1));
			maze[popped.r()][popped.c() + 1] = '0';
		}
	}
	//	no solution so return false
	return false;
}

int main()
{
	char maze[10][10] = {
		// 0   1   2   3   4   5   6   7   8   9
		{ 'X','X','X','X','X','X','X','X','X','X' }, //0
		{ 'X','.','.','.','.','X','.','.','.','X' }, //1
		{ 'X','.','X','X','.','X','X','X','.','X' }, //2
		{ 'X','X','X','.','.','.','.','X','.','X' }, //3
		{ 'X','.','X','X','X','.','X','X','X','X' }, //4
		{ 'X','.','X','.','.','.','X','.','.','X' }, //5
		{ 'X','.','.','.','X','.','X','.','.','X' }, //6
		{ 'X','X','X','X','X','.','X','.','X','X' }, //7
		{ 'X','.','.','.','.','.','.','.','.','X' }, //8
		{ 'X','X','X','X','X','X','X','X','X','X' }  //9
	};

	if (pathExists(maze, 3, 5, 8, 8))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;
}