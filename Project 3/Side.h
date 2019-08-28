#ifndef SIDE_H
#define SIDE_H

enum Side { NORTH, SOUTH };

const int NSIDES = 2;
const int POT = 0;

inline
Side opponent(Side s)
{
	return Side(NSIDES - 1 - s);
}

// It may contain additional types, constants, and
// non-member function declarations you find useful to add.

#endif