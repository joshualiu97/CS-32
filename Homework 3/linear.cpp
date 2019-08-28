//#include <iostream>
//#include <string>
//using namespace std;
//
//// testing predicate
//bool somePredicate(string s);

// Return false if the somePredicate function returns false for at
	// least one of the array elements; return true otherwise.
bool allTrue(const string a[], int n)
{
	if (n > 0)
	{
		int num = n;
		if (!somePredicate(a[n - 1]))
			return false;
		else
			return allTrue(a, num - 1);
	}
	return true;
}

// Return the number of elements in the array for which the
// somePredicate function returns false.
int countFalse(const string a[], int n)
{
	if (n > 0)
	{
		int num = n;
		if (!somePredicate(a[num - 1]))
			return 1 + countFalse(a, num - 1);
		else
			return countFalse(a, num - 1);
	}
	return 0;
}

// Return the subscript of the first element in the array for which
// the somePredicate function returns false.  If there is no such
// element, return -1.
int firstFalse(const string a[], int n)
{
	if (n <= 0)
		return -1;
	if (n > 0)
	{
		if (n < 2 && somePredicate(a[n - 1]) == false)
			return 0;
		else if (n < 2)
			return -1;
		int temp = -1;
		temp = firstFalse(a, n - 1);
		if (temp == -1)
		{
			if (somePredicate(a[n - 1]) == false)
				return n - 1;
		}
		else
			return temp;
	}
	return -1;

	//if (n <= 0)
	//	return -1;
	//if (n == 1 && somePredicate(a[0]) == false)
	//	return 0;
	//else if (n == 1)
	//	return -1;
	//if (firstFalse(a, n - 1) != -1)
	//	return firstFalse(a, n - 1);
	//else
	//{
	//	if (somePredicate(a[n - 1]) == false)
	//		return n - 1;
	//	else return firstFalse(a, n - 1);
	//}
}

// Return the subscript of the least string in the array (i.e.,
// return the smallest subscript m such that a[m] <= a[k] for all
// k from 0 to n-1).  If the function is told to examine no
// elements, return -1.
int indexOfLeast(const string a[], int n)
{
	if (n <= 0)
		return -1;
	if (n > 0)
	{
		if (n == 1)
			return 0;
		int temp = indexOfLeast(a, n - 1);
		if (a[n - 1] < a[temp])
			return n - 1;
		else
			return temp;
	}
	return -1;
	
	//// Method involving simple recursive call(but too many times)
	//if (n <= 0)
	//	return -1;
	//if (n == 1)
	//	return 0;
	//if (a[n - 1] < a[indexOfLeast(a, n - 1)])
	//	return n - 1;
	//else
	//	return indexOfLeast(a, n - 1);
}

// If all n2 elements of a2 appear in the n1 element array a1, in
// the same order (though not necessarily consecutively), then
// return true; otherwise (i.e., if the array a1 does not include
// a2 as a not-necessarily-contiguous subsequence), return false.
// (Of course, if a2 is empty (i.e., n2 is 0), return true.)
// For example, if a1 is the 7 element array
//    "stan" "kyle" "cartman" "kenny" "kyle" "cartman" "butters"
// then the function should return true if a2 is
//    "kyle" "kenny" "butters"
// or
//    "kyle" "cartman" "cartman"
// and it should return false if a2 is
//    "kyle" "butters" "kenny"
// or
//    "stan" "kenny" "kenny"
bool includes(const string a1[], int n1, const string a2[], int n2)
{
	if (n2 <= 0)
		return true;
	if (n2 > 0)
	{
		if (n1 <= 0)
			return false;
		if (a2[n2 - 1] == a1[n1 - 1])
		{
			return includes(a1, n1 - 1, a2, n2 - 1);
		}
		else
		{
			return includes(a1, n1 - 1, a2, n2);
		}
	}
	return false;
}

//// testing predicate
//bool somePredicate(string s)
//{
//	int nDigits = 0;
//	for (int k = 0; k != s.size(); k++)
//	{
//		if (isdigit(s[k]))
//			nDigits++;
//	}
//	return nDigits == 3;
//}
//
//int main()
//{
//	string a = "";
//	//cout << somePredicate(a) << endl;
//	string ar[10] = { "0", "0", "0", "0", "0", "3", "7"};   //T F T
//	//cout << somePredicate(ar[0]) << somePredicate(ar[1]) << somePredicate(ar[2]) << endl << endl;
//	//cout << allTrue(ar, 7) << endl; // return false
//	//cout << countFalse(ar, 7) << endl; // return 1
//	//cout << firstFalse(ar, 7) << endl; // return 1
//	//cout << indexOfLeast(ar, 7) << endl;
//	
//	string one[10] = { "a", "a", "d", "d" };
//	string two[10] = { "a", "t", "c" };
//	cout << includes(one, 4, two, 2); // return 1
//}