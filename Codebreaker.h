// Mastermind Codebreaker Class File
// Ian Cramer
// 9/6/18
// making a change



#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <map>
#include <algorithm>



using namespace std;



// p stands for primitive
template <typename p>
bool is_in(vector<p> v, p x)
{
	for (int i = 0; i < v.size(); i++)
		if (v[i] == x)
			return true;

	return false;
}

bool is_in(string s, char c)
{
	for (int i = 0; i < s.length(); i++)
		if (s[i] == c)
			return true;
	return false;
}






class Codebreaker
{
public:

	Codebreaker();



private:
	// Member Functions

	// Gettinc Setup
	bool allowDuplicates();
	int getCodeLength();
	string getCode();

	bool checkDuplicates(string code);
	bool validCode(string code);
	int calcNumPosCodes();


	
	
	string makeGuess();
	string checkGuess(string guess, string code);
	bool gameOver(string hint);

	// Brute Force Algorithm
	bool bruteForce();
	void genAllCodes();
	void pruneCodes();

	// Heuristic Algorithm
	bool heuristic();
	
	



	// Member Variables
	int codeLength;
	bool duplicates;
	string mCode;
	string guess;
	string hint;
	bool won = false;
	vector<char> mLetters = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

	// For Brute Force Algorithm
	vector<string> possibleCodes;
	

	// For Heuristic
	vector<string> pastHints;
	vector<string> codePieces;


};



// Constructor
Codebreaker::Codebreaker()
{
	// Start Random
	srand(time(NULL));
	// Get Code Length
	codeLength = getCodeLength();
	// Allow Duplicates?
	duplicates = allowDuplicates();
	// Get Code
	mCode = getCode();

	// Reasonable problem space, solve by brute force
	if (calcNumPosCodes() < 25000)
		won = !bruteForce();
	// Problem space is too large, heuristic
	else
		won = !heuristic();

	if (won)
		cout << "You win, good game." << endl;
	else
		cout << "Haha sucker, I broke your code!" << endl;
}



// -------------------- Getting Set Up --------------------
int Codebreaker::getCodeLength()
{
	string in;
	
	while (true)
	{
		cout << endl << "Please enter code length (4, 5, 6): ";
		cin >> in;
		
		if (in[0] == '4' || in[0] == '5' || in[0] == '6')
			return int(in[0]) - '0';
		
		cout << "Invalid Code Length" << endl;
	}
	
	return 4;
}

bool Codebreaker::allowDuplicates()
{
	if (codeLength > mLetters.size())
		return true;

	string in;
	
	while (true)
	{
		cout << endl << "Allow Duplicates?" << endl << "1. Yes" << endl << "2. No" << endl;
		cin >> in;
		
		if (in[0] == '1' || tolower(in[0]) == 'y')
			return true;
		else if (in[0] == '2' || tolower(in[0]) == 'n')
			return false;

		cout << "Invalid Response" << endl;
	}

	return false;
}

string Codebreaker::getCode()
{
	string in;

	while (true)
	{
		cout << "Enter Your Code: ";
		cin >> in;

		if ( validCode(in) )
			break;
	}

	return in;
}

bool Codebreaker::validCode(string code)
{
	// Bad code length, make user reenter
	if (code.length() != codeLength)
		return false;

	// Bad code letters, make user reenter
	for (int i = 0; i < code.length(); i++)
		if (code[i] < 'a' || code[i] > 'h')
			return false;

	if (!duplicates)
		return !checkDuplicates(code);

	return true;
}

// This function checks for invalid duplicates in a mastermind code
bool Codebreaker::checkDuplicates(string code)
{
	// If duplicates are allowed, don't bother checking, just return false
	if (duplicates)
		return false;

	// If duplicates not allowed, check for duplicates
	for (int i = 0; i < code.length(); i++)
		for (int j = i + 1; j < code.length(); j++)
			if (code[i] == code[j])
				// Illegal duplicate found
				return true;

	// No duplicates found
	return false;
}

int Codebreaker::calcNumPosCodes()
{
	int x = 1;

	if (duplicates)
		return pow(8, codeLength);

	// Note that if codeLength > mLetters.size(), duplicates will be allowed and this code will not be run.
	for (int i = codeLength; i > mLetters.size() - codeLength; i--)
		x = x * i;

	return x;
}

bool Codebreaker::gameOver(string hint)
{
	if (hint.length() != codeLength)
		return false;

	for (int i = 0; i < hint.length(); i++)
		if (hint[i] != '*')
			return false;

	return true;
}



// -------------------- Game Play --------------------
string Codebreaker::makeGuess()
{
	if (!possibleCodes.empty())
		return possibleCodes[rand() % possibleCodes.size()];

	// Initialize new guess
	string newGuess = "";
	if (!codePieces.empty())
		string newGuess = codePieces[rand() % codePieces.size()];

	// Construct new guess
	while (newGuess.length() < codeLength)
	{
		char newLetter = mLetters[rand() % mLetters.size()];
		if (duplicates || !is_in(newGuess, newLetter))
			newGuess = newGuess + newLetter;
	}

	return newGuess;
}

string Codebreaker::checkGuess(string guess, string code)
{
	string hint;

	// Check correct letters in correct place
	for (int i = 0; i < codeLength; i++)
		if (guess[i] == code[i])
		{
			hint += '*';
			guess[i] = 'x';
			code[i] = 'y';
		}
	
	// Check correct letters in the wrong place
	for (int i = 0; i < codeLength; i++)
		for (int j = 0; j < codeLength; j++)
			if (guess[i] == code[j] && i != j)
			{
				hint += '-';
				guess[i] = 'x';
				code[j] = 'y';
			}
	
	return hint;
}















// Brute Force Algorithm and Helper Functions
bool Codebreaker::bruteForce()
{
	genAllCodes();

	for (int i = 0; i < 10; i++)
	{
		guess = makeGuess();

		cout << "On turn " << i + 1 << " I guessed: " << guess << endl;

		hint = checkGuess(guess, mCode);

		cout << "You gave me the feedback: " << hint << endl;

		if (gameOver(hint))
			return true;

		pruneCodes();
	}
	return false;
}

void Codebreaker::genAllCodes()
{
	vector<string> codes = {"a", "b", "c", "d", "e", "f", "g", "h"};

	for (int k = 0; k < codeLength - 1; k++)
	{
		int n = codes.size();
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < mLetters.size(); j++)
			{
				string newCode = codes[i] + mLetters[j];
				if (checkDuplicates(newCode) || count(codes.begin(), codes.end(), newCode))
					continue;
				if (newCode.length() == codeLength)
					possibleCodes.push_back(newCode);
				else
					codes.push_back(newCode);

			}
			
		}
	}
}

void Codebreaker::pruneCodes()
{
	vector<string> nextPossibleCodes;
	for (int i = 0; i < possibleCodes.size(); i++)
	{
		string feedback = checkGuess(guess, possibleCodes[i]);

		if (feedback == hint)
			nextPossibleCodes.push_back(possibleCodes[i]);
	}

	possibleCodes = nextPossibleCodes;
}





// Heuristic Algorithm and Hellper Functions
bool Codebreaker::heuristic()
{
	int numAllCodes = calcNumPosCodes();
	int numCodesLeft = numAllCodes;

	for (int i = 0; i < 10; i++)
	{
		guess = makeGuess();

		cout << "On turn " << i + 1 << " I guessed: " << guess << endl;

		hint = checkGuess(guess, mCode);

		cout << "You gave me the feedback: " << hint << endl;

		if (gameOver(hint))
			return true;
	}

	return false;
}










