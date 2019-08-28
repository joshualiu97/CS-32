#if defined(_MSC_VER)  &&  !defined(_DEBUG)
#include <iostream>
#include <windows.h>
#include <conio.h>

struct KeepWindowOpenUntilDismissed
{
	~KeepWindowOpenUntilDismissed()
	{
		DWORD pids[1];
		if (GetConsoleProcessList(pids, 1) == 1)
		{
			std::cout << "Press any key to continue . . . ";
			_getch();
		}
	}
} keepWindowOpenUntilDismissed;
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <vector>
#include <cassert>
//#include <time.h> // to see execution time
using namespace std;

const int BUCKETSIZE = 11117;
const int CONSEC = 8;

size_t getHash(const string& seq)
// returns hash value
{
	return hash<string>()(seq) % BUCKETSIZE;
}

class Sequence
{
public:
	Sequence(const string& seq, const int& numOfChar, const int& offset)
		: m_seq(seq), m_numOfChar(numOfChar), m_offset(offset) {}
	Sequence() : m_seq(""), m_numOfChar(0), m_offset(-1) {};
	string seq() const { return m_seq; }
	int numOfChar() const { return m_numOfChar; }
	int offset() const { return m_offset; }
	void setOffset() { m_offset = -1; }
	void display();
private:
	string m_seq;
	int m_numOfChar;
	int m_offset;
};

void Sequence::display()
// display items in Sequence object
{
	cerr << getHash(m_seq) << " " << m_seq << " " << m_numOfChar << " " << m_offset << endl;
}

class HashTable
{
public:
	HashTable() { bucket.resize(BUCKETSIZE); }
	void insert(Sequence& obj);
	bool find(const string& input, Sequence& seq);
	void view();
	vector<Sequence> getBucket(size_t num) { return bucket[num]; }
private:
	vector<vector<Sequence>> bucket; // 0 to 11116, 11117 buckets
};

void HashTable::insert(Sequence& obj)
// inserts into hash table
{
	Sequence temp;
	size_t hashVal = getHash(obj.seq());
	for (size_t e = 0; e < bucket[hashVal].size(); e++) // if the same object is hashed or the bucket has too many items, do not hash
		if (bucket[hashVal][e].offset() == obj.offset() || bucket[hashVal].size() > 1000)
			return;
	vector<Sequence> items;
	if (!bucket[hashVal].empty()) // if not empty, collision, and add to end of vector
		items = bucket[hashVal];
	items.push_back(obj);
	bucket[hashVal] = items;
	return;
}

bool HashTable::find(const string& input, Sequence& seq)
// finds Sequence in hash table. returns true if found, false if not found
{
	size_t hashVal = getHash(input);
	for (size_t i = 0; i < bucket[hashVal].size(); i++)
		if (input == bucket[hashVal][i].seq())
		{
			seq = bucket[hashVal][i];
			return true;
		}
	return false;
}

void HashTable::view()
// print out hash table
{
	for (size_t i = 0; i < bucket.size(); i++)
	{
		if (!bucket[i].empty())
		{
			for (size_t j = 0; j < bucket[i].size(); j++)
				bucket[i][j].display();
		}
	}
}

void createDiff(istream& fold, istream& fnew, ostream& fdiff)
{
	//clock_t start, end, start1, end1; // clocks to keep time
	//start = clock(); // begin clock for reading into hash table

	// goes to the start of the fold and fnew file
	fold.clear();
	fold.seekg(0);
	fnew.clear();
	fnew.seekg(0);

	HashTable myHash; // create hash table
	string old_, new_, temp, copy1, copy2;

	// read in the old and new files (including new lines)
	while (getline(fold, temp))
	{
		old_ += temp;
		old_ += "\n";
	}
	if (temp != "") // if temp is not empty, then there is no new line character at the end
	{
		size_t lookForNewLine = old_.length();
		old_ = old_.substr(0, lookForNewLine - 1);
	}
	temp = "";
	while (getline(fnew, temp))
	{
		new_ += temp;
		new_ += "\n";
	}
	if (temp != "") // if temp is not empty, then there is no new line character at the end
	{
		size_t lookForNewLine = new_.length();
		new_ = new_.substr(0, lookForNewLine - 1);
	}
	temp = "";

	//cerr << old_ << endl;
	//cerr << new_ << endl;
	
	//cerr << old_.length() << endl;
	//cerr << new_.length() << endl << endl;

	size_t n = 0;
	size_t old_len = old_.length();
	size_t endcase = CONSEC + 1;
	size_t compare;

	if (old_len < endcase) // make sure that substring gets valid inputs
		compare = 0;
	else
		compare = old_len - endcase;

	while (n < compare) // go through old_ and put into hash table
	{
		string sub = old_.substr(n, CONSEC); // get sequence of len CONSEC
		size_t len = sub.length();
		Sequence push(sub, len, n); // insert into hash table
		myHash.insert(push);
		n++;
	}
	
	//end = clock();
	//cerr << "First half: " << double(end - start) / CLOCKS_PER_SEC << endl; // prints time taken to put items in hash table
	
	//myHash.view(); // view hash table 
	
	//start1 = clock(); // begin clock for searching for a consecutive string
	
	Sequence check;
	Sequence best;
	temp = "";
	string diffstr;
	size_t i = 0;
	int consecutive = 0;

	// Go through new string a see if there is are more matches. Then print the correct add and copy instruction.
	while (i < new_.length())
	{
		size_t new_len = new_.length();
		size_t endcase2 = CONSEC + 1;
		size_t compare2;

		if (new_len < endcase2) // make sure substring parameters are valid
			compare2 = 0;
		else
			compare2 = new_len - endcase2;

		if (i > compare2) // back of string check, since no more substrings have length CONSEC
		{
			temp += new_[i];
			i++;
			continue;
		}

		string sub = new_.substr(i, CONSEC); // find sequence of len CONSEC
		if (myHash.find(sub, check)) // if found in hash table
		{
			vector<Sequence> bucket = myHash.getBucket(getHash(sub)); // get bucket
			int greatest = 0;

			for (size_t p = 0; p < bucket.size(); p++)
			{
				if (bucket[p].seq() != sub)
					continue;

				size_t newcounter = i + CONSEC; // position after CONSEC sequence
				size_t k = bucket[p].offset() + CONSEC;
				check = bucket[p];
				consecutive = 0;

				if (best.offset() == -1) // set first value of best to a valid matching sequence
					best = check;

				while (k < old_.length() && newcounter < new_.length()) // look for more consecutive matches and keep count
				{
					if (old_[k] == new_[newcounter])
					{
						newcounter++;
						k++;
						consecutive++;
					}
					else
						break;
				}

				if (consecutive > greatest) // if the number of consecutive matches is greater than before, update the best sequence
				{
					greatest = consecutive;
					best = check;
				}
			}

			// do add instruction if nothing matches from old string
			if (temp != "")
				diffstr += "A" + to_string(temp.length()) + ':' + temp;
			temp = "";

			// then do copy instruction based on consecutive matches
			diffstr += "C" + to_string(CONSEC + greatest) + ',' +
				to_string(best.offset());

			i += CONSEC + greatest; // update i to next position in new string
			best.setOffset(); // reset the offset value
		}
		else // no match so need to add character
		{
			temp += new_[i];
			i++;
		}
	}

	// write add instruction for left over characters
	if (temp != "")
		diffstr += "A" + to_string(temp.length()) + ':' + temp;

	//cerr << diffstr << endl;
	fdiff << diffstr; // write to ostream

	//end1 = clock();
	//cerr << "Second half: " << double(end1 - start1) / CLOCKS_PER_SEC << endl; // end clock and print time taken to create diff file
	
	return;
}

bool getInt(istream& inf, int& n)
// manages ints for getCommand. returns false if invalid and true if valid
{
	char ch;
	if (!inf.get(ch) || !isascii(ch) || !isdigit(ch))
		return false;
	inf.unget();
	inf >> n;
	return true;
}

bool getCommand(istream& inf, char& cmd, int& length, int& offset)
// gets command and returns corresponding items. returns false if invalid instruction and true if valid instruction
{
	if (!inf.get(cmd))
	{
		cmd = 'x';  // signals end of file
		return true;
	}
	char ch;
	switch (cmd)
	{
	case 'A':
		return getInt(inf, length) && inf.get(ch) && ch == ':';
	case 'C':
		return getInt(inf, length) && inf.get(ch) && ch == ',' && getInt(inf, offset);
	case '\r':
	case '\n':
		return true;
	}
	return false;
}

bool applyDiff(istream& fold, istream& fdiff, ostream& fnew)
{
	// read from beginning of istream
	fold.clear();
	fold.seekg(0);
	fdiff.clear();
	fdiff.seekg(0);

	// if error for files (end of file too), return false
	if (!fold || !fdiff || !fnew)
		return false;
	
	string old_, temp, new_;

	// get string for fold
	while (getline(fold, temp))
	{
		old_ += temp;
		old_ += "\n";
	}
	if (temp != "")
	{
		size_t lookForNewLine = old_.length();
		old_ = old_.substr(0, lookForNewLine - 1);
	}
	temp = "";
	
	char cmd = 'b'; // char that is not 'x'
	int length = 0;
	int offset = 0;

	while (cmd != 'x')
	{
		if (!getCommand(fdiff, cmd, length, offset)) // get command, return false if command invalid
			return false;

		if (cmd == 'A') // do add instruction
		{
			for (int i = 0; i < length; i++)
			{
				char get;
				fdiff.get(get);
				new_ += get;
			}
		}

		if (cmd == 'C') // check for valid copy parameters then do copy instruction
		{
			size_t comp1 = offset;
			size_t comp2 = length;
			if (offset < 0 || comp1 > old_.length() - 1 || length < 0 || comp2 > old_.length())
				return false;
			string copied = old_.substr(offset, length);
			new_ += copied;
		}
	}
	
	//cerr << new_ << endl;
	fnew << new_; // write to ostream
	
	return true;
}

/*
bool runtest(string oldName, string newName, string diffName, string newName2)
{
	if (diffName == oldName || diffName == newName ||
		newName2 == oldName || newName2 == diffName ||
		newName2 == newName)
	{
		cerr << "Files used for output must have names distinct from other files" << endl;
		return false;
	}
	ifstream oldFile(oldName, ios::binary);
	if (!oldFile)
	{
		cerr << "Cannot open " << oldName << endl;
		return false;
	}
	ifstream newFile(newName, ios::binary);
	if (!newFile)
	{
		cerr << "Cannot open " << newName << endl;
		return false;
	}
	ofstream diffFile(diffName, ios::binary);
	if (!diffFile)
	{
		cerr << "Cannot create " << diffName << endl;
		return false;
	}
	createDiff(oldFile, newFile, diffFile);
	diffFile.close();

	oldFile.clear();   // clear the end of file condition
	oldFile.seekg(0);  // reset back to beginning of the file
	ifstream diffFile2(diffName, ios::binary);
	if (!diffFile2)
	{
		cerr << "Cannot read the " << diffName << " that was just created!" << endl;
		return false;
	}
	ofstream newFile2(newName2, ios::binary);
	if (!newFile2)
	{
		cerr << "Cannot create " << newName2 << endl;
		return false;
	}
	assert(applyDiff(oldFile, diffFile2, newFile2));
	newFile2.close();

	newFile.clear();
	newFile.seekg(0);
	ifstream newFile3(newName2, ios::binary);
	if (!newFile)
	{
		cerr << "Cannot open " << newName2 << endl;
		return false;
	}
	if (!equal(istreambuf_iterator<char>(newFile), istreambuf_iterator<char>(),
		istreambuf_iterator<char>(newFile3), istreambuf_iterator<char>()))
	{

		cerr << newName2 << " is not identical to " << newName
			<< "; test FAILED" << endl;
		return false;
	}
	return true;
}
*/

/*
int main()
{
	cerr << "smallmart Tests: ";
	assert(runtest("C:/CS32/Project 4(2)/txt_files/smallmart1.txt", "C:/CS32/Project 4(2)/txt_files/smallmart2.txt", "C:/CS32/Project 4(2)/txt_files/data.txt.txt", "C:/CS32/Project 4(2)/txt_files/out.txt"));
	cerr << "smallmart tests PASSED" << endl;
	
	cerr << "greeneggs Tests: ";
	assert(runtest("C:/CS32/Project 4(2)/txt_files/greeneggs1.txt", "C:/CS32/Project 4(2)/txt_files/greeneggs2.txt", "C:/CS32/Project 4(2)/txt_files/data.txt.txt", "C:/CS32/Project 4(2)/txt_files/out.txt"));
	cerr << "greeneggs tests PASSED" << endl;

	cerr << "warandpeace Tests: ";
	assert(runtest("C:/CS32/Project 4(2)/txt_files/warandpeace1.txt", "C:/CS32/Project 4(2)/txt_files/warandpeace2.txt", "C:/CS32/Project 4(2)/txt_files/data.txt.txt", "C:/CS32/Project 4(2)/txt_files/out.txt"));
	cerr << "warandpeace tests PASSED" << endl;

	cerr << "strange Tests: ";
	assert(runtest("C:/CS32/Project 4(2)/txt_files/strange1.txt", "C:/CS32/Project 4(2)/txt_files/strange2.txt", "C:/CS32/Project 4(2)/txt_files/data.txt.txt", "C:/CS32/Project 4(2)/txt_files/out.txt"));
	cerr << "smallmart tests PASSED" << endl;

	cerr << "strange Tests: ";
	assert(runtest("C:/CS32/Project 4(2)/txt_files/faq1.txt", "C:/CS32/Project 4(2)/txt_files/faq2.txt", "C:/CS32/Project 4(2)/txt_files/data.txt.txt", "C:/CS32/Project 4(2)/txt_files/out.txt"));
	cerr << "smallmart tests PASSED" << endl;

	cerr << "abc Tests: ";
	assert(runtest("C:/CS32/Project 4(2)/txt_files/abc1.txt", "C:/CS32/Project 4(2)/txt_files/abc2.txt", "C:/CS32/Project 4(2)/txt_files/data.txt.txt", "C:/CS32/Project 4(2)/txt_files/out.txt"));
	cerr << "abc tests PASSED" << endl;

	cerr << "enter Tests: ";
	assert(runtest("C:/CS32/Project 4(2)/txt_files/enter1.txt", "C:/CS32/Project 4(2)/txt_files/enter2.txt", "C:/CS32/Project 4(2)/txt_files/data.txt.txt", "C:/CS32/Project 4(2)/txt_files/out.txt"));
	cerr << "enter tests PASSED" << endl;

	cerr << endl << "PASSED ALL TESTS" << endl;
}
*/

int main()
{
	cerr << "Enter file name: ";
	string input;
	cin >> input;
	
	ifstream i1("C:/CS32/Project 4(2)/txt_files/" + input + "1.txt");   // outfile is a name of our choosing.
	ifstream i2("C:/CS32/Project 4(2)/txt_files/" + input + "2.txt");   // outfile is a name of our choosing.
	ofstream o3("C:/CS32/Project 4(2)/txt_files/data.txt");   // outfile is a name of our choosing.

	createDiff(i1, i2, o3);
	o3.close();

	ifstream i3("C:/CS32/Project 4(2)/txt_files/data.txt");   // outfile is a name of our choosing.
	ofstream o4("C:/CS32/Project 4(2)/txt_files/out.txt");   // outfile is a name of our choosing.

	cout << "T or F: " << applyDiff(i1, i3, o4) << endl;
}