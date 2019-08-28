//#include <iostream>
//using namespace std;

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
	if (sr == er && sc == ec)
		return true;
	maze[sr][sc] = '0';
	Coord popped(sr, sc);
	// if we can move SOUTH add the coord and update it
	if (popped.r() < 9 && maze[popped.r() + 1][popped.c()] == '.')
	{
		if (pathExists(maze, popped.r() + 1, popped.c(), er, ec))
			return true;
	}
	// if we can move WEST add the coord and update it
	if (popped.c() > 0 && maze[popped.r()][popped.c() - 1] == '.')
	{
		if (pathExists(maze, popped.r(), popped.c() - 1, er, ec))
			return true;
	}
	// if we can move NORTH add the coord and update it
	if (popped.r() > 0 && maze[popped.r() - 1][popped.c()] == '.')
	{
		if (pathExists(maze, popped.r() - 1, popped.c(), er, ec))
			return true;
	}
	// if we can move EAST add the coord and update it
	if (popped.c() < 9 && maze[popped.r()][popped.c() + 1] == '.')
	{
		if (pathExists(maze, popped.r(), popped.c() + 1, er, ec))
			return true;
	}
	return false;
}

//If the start location is equal to the ending location, then we've
//solved the maze, so return true.
//Mark the start location as visted.
//For each of the four directions,
//If the location one step in that direction(from the start
//	location) is unvisited,
//	then call pathExists starting from that location(and
//		ending at the same ending location as in the
//		current call).
//	If that returned true,
//	then return true.
//	Return false.

//int main()
//{
//	char maze[10][10] = {
//		// 0   1   2   3   4   5   6   7   8   9
//		{ 'X','X','X','X','X','X','X','X','X','X' },// 0
//		{ 'X','.','.','.','.','X','.','.','.','X' },// 1
//		{ 'X','.','X','X','.','X','X','X','.','X' },// 2
//		{ 'X','X','X','.','.','.','.','X','.','X' },// 3
//		{ 'X','.','X','X','X','.','X','X','X','X' },// 4
//		{ 'X','.','X','.','.','.','X','.','.','X' },// 5
//		{ 'X','.','.','.','X','.','X','.','.','X' },// 6
//		{ 'X','X','X','X','X','.','X','.','X','X' },// 7
//		{ 'X','.','.','.','.','.','.','.','.','X' },// 8
//		{ 'X','X','X','X','X','X','X','X','X','X' } // 9
//	};
//
//	if (pathExists(maze, 3, 5, 8, 8))
//		cout << "Solvable!" << endl;
//	else
//		cout << "Out of luck!" << endl;
//}