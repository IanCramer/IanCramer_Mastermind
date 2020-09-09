// Ian Cramer
// Mastermind additional development
// 9/18/2020
//



// Header

#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <map>
#include <algorithm>
#include <ctime>

using namespace std;



// Utility Functions

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

template <typename p>
void print(vector<p> v)
{
	if (v.empty())
		return;
	cout << v[0];
	for (int i = 1; i < v.size(); i++)
		cout << ", " << v[i];
	cout << endl; 
}








// Acting like class data but global in this scope

vector<string> letters = {"a", "b", "c", "d", "e", "f", "g", "h"};
int codeLength = 3;
bool duplicates = false;


bool validCode(string code);
bool checkDuplicates(string code);
int calcNumPosCodes();
bool gameOver(string hint);

string checkGuess(string guess, string code);

string randCode();

































/*
vector<string> guessBuilder;
vector<string> possibleGuesses;

void interpretHint(string g, string h);
void interpretStar(string g);
void interpretHyp(string g);
void interpretMisses(string g);
void validateGuesses();
*/





































string baseGuess;
string correctLetters = "";

vector<string> guesses;
vector<string> eliminated;
vector<string> hints;


string makeGuess()
{
	clock_t start;
	double duration;
	double timeLimit = codeLength * guesses.size() - codeLength/2.;
	if (!duplicates)
		timeLimit = timeLimit/2.;
	cout << timeLimit << endl;

	bool success = false;
	string newGuess;

	int oldScore = 0;
	int newScore = 0;
	string bestGuess;

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

		// Watch the time
		duration = (clock() - start) / (double) CLOCKS_PER_SEC;
		if (duration > timeLimit)
			return bestGuess;

		// Check the new guess is a potential code from previous guesses
		for (int i = 0; i < guesses.size(); i++)
		{
			// What would the hint have been for a previous guess if the new guess was the code?
			string mockHint = checkGuess(guesses[i], newGuess);

			// Score the guess
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
		if (newScore > oldScore)
		{
			oldScore = newScore;
			bestGuess = newGuess;
		}
	}
	eliminated.push_back(newGuess);
	return newGuess;
}

/*
int main()
{
	srand(time(NULL));

	for (int i = 0; i < codeLength; i++)
		baseGuess += '_';
	// guessBuilder.push_back(baseGuess);


	string code = randCode();

	string guess = baseGuess;
	if (duplicates)
		for (int i = 0; i < codeLength; i++)
		{
			if (i < codeLength/2.)
				guess[i] = letters[0][0];
			else
				guess[i] = letters[1][0];
		}
	else
		for (int i = 0; i < codeLength; i++)
			guess[i] = letters[i][0];

	guesses.push_back(guess);
	cout << "Guess 1: " << guess << endl;

	// For human feedback
	// string hint;
	// cout << "Hint: ";
	// cin >> hint;

	string hint = checkGuess(guess, code);
	hints.push_back(hint);
	cout << "Hint 1: " << hint << endl;


	for (int i = 2; i < 11; i++)
	{
		guess = makeGuess();
		guesses.push_back(guess);
		cout << "Guess " << i << ": " << guess << endl;

		hint = checkGuess(guess, code);
		hints.push_back(hint);
		cout << "Hint " << i << ": " << hint << endl;

		if (gameOver(hint))
			break;
	}

	sort(eliminated.begin(), eliminated.end());
	print(eliminated);
}
*/





































/*
void interpretHint(string g, string h)
{
	int stars = count(h.begin(), h.end(), '*');
	int hyps = count(h.begin(), h.end(), '-');
	int misses = codeLength - stars - hyps;

	for (int i = 0; i < stars; i++)
		interpretStar(g);
	guessBuilder = possibleGuesses;

	for (int i = 0; i < hyps; i++)
		interpretHyp(g);
	guessBuilder = possibleGuesses;

	for (int i = 0; i < misses; i++)
		interpretMisses(g);

	// print(possibleGuesses);

	validateGuesses();
}

void interpretStar(string g)
{
	vector<string> x;
	int n = guessBuilder.size();
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < g.length(); j++)
		{
			string newGuess = guessBuilder[i];
			newGuess[j] = g[j];
			if (!is_in(guessBuilder, newGuess))
			{
				guessBuilder.push_back(newGuess);
				x.push_back(newGuess);
			}
		}
	}
	possibleGuesses = x;
}

void interpretHyp(string g)
{
	vector<string> x;
	int n = guessBuilder.size();
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < codeLength; j++)
		{
			string newGuess = guessBuilder[i];
			if (newGuess[j] != '_')
				continue;
			for (int k = 0; k < codeLength; k++)
			{
				if (j == k)
					continue;
				if (is_in(newGuess, g[k]))
					continue;
				newGuess[j] = g[k];
				guessBuilder.push_back(newGuess);
				x.push_back(newGuess);
			}
		}
	}
	possibleGuesses = x;
}

void interpretMisses(string g)
{
	vector<string> x;
	int n = guessBuilder.size();
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < codeLength; j++)
		{
			string newGuess = guessBuilder[i];
			if (newGuess[j] != '_')
				continue;
			for (int k = 0; k < letters.size(); k++)
			{
				if (!is_in(g, letters[k][0]))
				{
					newGuess[j] = letters[k][0];
					if (!checkDuplicates(newGuess))
					{
						guessBuilder.push_back(newGuess);
						if (validCode(newGuess))
							x.push_back(newGuess);
					}
				}
			}
		}
	}
	possibleGuesses = x;
}

void validateGuesses()
{
	cout << possibleGuesses.size() << endl;
}
*/














































string randCode()
{
	string s = "";
	while (!validCode(s))
	{
		string l = letters[rand() % letters.size()];
		if (!checkDuplicates(s+l))
			s += l;
	}
	return s;
}




string checkGuess(string guess, string code)
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








bool validCode(string code)
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



bool checkDuplicates(string code)
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



int calcNumPosCodes()
{
	int x = 1;

	if (duplicates)
		return pow(8, codeLength);

	// Note that if codeLength > letters.size(), duplicates will be allowed and this code will not be run.
	for (int i = codeLength; i > letters.size() - codeLength; i--)
		x = x * i;

	return x;
}

bool gameOver(string hint)
{
	if (hint.length() != codeLength)
		return false;

	for (int i = 0; i < hint.length(); i++)
		if (hint[i] != '*')
			return false;

	return true;
}












