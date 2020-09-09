// Mastermind Codebreaker Class File
// Ian Cramer
// 9/6/18
// making a change



#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <algorithm>
#include <ctime>



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
	string checkGuess(string guess, string code);
	string randCode();
	bool gameOver();



	// Brute Force Algorithm
	bool bruteForce();
	void genAllCodes();
	void pruneCodes();
	string bruteGuess();



	// Heuristic Algorithm
	bool heuristic();
	bool firstGuess();
	string makeGuess();
	
	



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
	vector<string> guesses;
	vector<string> eliminated;
	vector<string> hints;


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

	cout << "Here" << endl;
	
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
        cout << endl << "Please enter code length (1 digit integer): ";
        cin >> in;
        
        if (isdigit(in[0]))
            return int(in[0]) - '0';
        
        cout << "Invalid Code Length" << endl;
    }
    
    return 4;
}

bool Codebreaker::allowDuplicates()
{
	if (codeLength > mLetters.size())
		return true;
	if (codeLength < 2)
		return false;

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

string Codebreaker::randCode()
{
	string code = "";
    vector<char> letters = mLetters;

    for (int i = 0; i < codeLength; i++)
    {
        // Pick a new letter
        char newLetter = letters[rand() % letters.size()];
        
        // Duplicates Not Allowed?
        if (!duplicates)
            // Find and remove each new letter from the code building blocks vector
            letters.erase( find(letters.begin(), letters.end(), newLetter) );
        
        // Add new letter to code
        code += newLetter;
    }

    return code;
}

bool Codebreaker::gameOver()
{
	return guess == mCode;
}



// -------------------- Game Play --------------------
string Codebreaker::checkGuess(string guess, string code)
{
	string hint = "";

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

	for (int i = 1; i < 11; i++)
	{
		guess = bruteGuess();

		cout << "On turn " << i << " I guessed: " << guess << endl;

		hint = checkGuess(guess, mCode);

		cout << "You gave me the feedback: " << hint << endl;

		if (gameOver())
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

string Codebreaker::bruteGuess()
{
	if (possibleCodes.empty())
		return randCode();
	return possibleCodes[rand() % possibleCodes.size()];
}





// Heuristic Algorithm and Hellper Functions
bool Codebreaker::heuristic()
{
	if (firstGuess())
		return true;

	for (int i = 2; i < 11; i++)
	{
		cout << endl;

		guess = makeGuess(); cout << endl;
		cout << "On turn " << i << " I guessed: \t\t" << guess << endl;

		hint = checkGuess(guess, mCode);
		hints.push_back(hint);
		cout << "You gave me the feedback: \t" << hint << endl;

		if (gameOver())
			return true;
	}

	return false;
}

bool Codebreaker::firstGuess()
{
	guess = "";

	if (duplicates)
		for (int i = 0; i < codeLength; i++)
			guess += mLetters[i/2];
	else
		for (int i = 0; i < codeLength; i++)
			guess += mLetters[i];


	guesses.push_back(guess);
	eliminated.push_back(guess);
	cout << "On turn 1 I guessed: \t\t" << guess << endl;

	hint = checkGuess(guess, mCode);
	hints.push_back(hint);
	cout << "You gave me the feedback: \t" << hint << endl;

	return gameOver();
}

string Codebreaker::makeGuess()
{
	// Make sure the computer doesn't take too long coming up with a guess
	clock_t start;
	double duration;
	double timeLimit = codeLength/4. + guesses.size();
	if (!duplicates)
		timeLimit = timeLimit/2.;

	// Is the newGuess a possible match for the code?
	bool success = false;

	// Guesses can be scored based on close they might be to the code
	// This is used for determining the best guess if time runs out.
	int bestScore = 0;
	int newScore = 0;

	// Space to save new and best guesses
	string newGuess;
	string bestGuess = randCode();

	// Let's make a guess, start timing
	start = clock();
	while (!success)
	{
		// Come up with a new guess
		newGuess = randCode();
		// Assume it will work
		success = true;

		// Check the new guess hasn't already been eliminated
		if (is_in(eliminated, newGuess))
		{
			// If it has, the new guess is not a success.
			success = false;
			// skip to next new guess
			continue;
		}

		// Watch the time. If time limit reached, return the best guess
		duration = (clock() - start) / (double) CLOCKS_PER_SEC;
		if (duration > timeLimit)
		{
			guesses.push_back(bestGuess);
			return bestGuess;
		}

		// THIS IS THE IMPORTANT PART
		// Check the new guess is a potential code from previous guesses
		for (int i = 0; i < guesses.size(); i++)
		{
			// What would the hint have been for a previous guess if the new guess was the code?
			string mockHint = checkGuess(guesses[i], newGuess);

			// Score the new guess
			newScore = i;

			// If it's not the same hint a previous guess received...
			if (mockHint != hints[i])
			{
				// That guess is not a success
				success = false;
				// And should be eliminated as an option
				eliminated.push_back(newGuess);
				// No  need to check if the new guess would have been valid for other previous guesses.
				break;
			}		
		}

		// If the new score is better than the best so far, save that guess
		if (newScore > bestScore)
		{
			bestScore = newScore;
			bestGuess = newGuess;
		}
	}

	guesses.push_back(newGuess);
	eliminated.push_back(newGuess);
	return newGuess;
}








