// Mastermind Codebreaker Class File
// Ian Cramer
// 9/6/18
//



#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <algorithm>



using namespace std;






class Codebreaker
{
public:

	Codebreaker();



private:
	// Member Functions
	bool allowDuplicates();
    int getCodeLength();
    string getCode();


	void generateAllCodes();
	void pruneCodes();
	bool checkDuplicates(string code);
	string firstGuess();
	string makeGuess();
	string checkGuess(string guess, string code);
	bool gameOver(string hint);
	bool validCode(string code);
	int calcNumPosCodes();



	// Member Variables
	int codeLength;
	bool duplicates;
	vector<char> mLetters = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
	vector<string> possibleCodes;
	string guess;
	string hint;
	string mCode;
	bool won = false;



};


Codebreaker::Codebreaker()
{
	// Get Code Length
    codeLength = getCodeLength();
    // Allow Duplicates?
    duplicates = allowDuplicates();
    // Calculate Number of Possible Codes
    cout << "Number of Possible Codes: " << calcNumPosCodes() << endl;
    // Get Code
    mCode = getCode();
    // Generate All Codes
	generateAllCodes();

	// Start Guessing
	for (int i = 0; i < 10; i++)
	{
		guess = makeGuess();

		cout << "On turn " << i + 1 << " I guessed: " << guess << endl;

		hint = checkGuess(guess, mCode);

		cout << "You gave me the feedback: " << hint << endl;

		pruneCodes();

		if (gameOver(hint))
		{
			won = true;
			break;
		}
	}

	if (won)
		cout << "Haha sucker, I broke your code!" << endl;
	else
		cout << "You win, good game." << endl;
}

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
	if (codeLength > 4)
	{
		cout << "Duplicates will not be allowed as the problem space will become too large." << endl;
		return false;
	}

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




void Codebreaker::generateAllCodes()
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


bool Codebreaker::checkDuplicates(string code)
{
	if (duplicates)
		return false;

    // If duplicates not allowed, check for duplicates
    for (int i = 0; i < code.length(); i++)
    	for (int j = i + 1; j < code.length(); j++)
    		if (code[i] == code[j])
    			// Duplicate found
    			return true;

    // No duplicates found
    return false;
}


string Codebreaker::firstGuess()
{
	guess = "abcd";
	string hint = "";
	string code = mCode;
    
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
                hint += '+';
                guess[i] = 'x';
                code[j] = 'y';
            }
    
    return hint;
}

string Codebreaker::makeGuess()
{
	srand(time(NULL));

	return possibleCodes[rand() % possibleCodes.size()];
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
                hint += '+';
                guess[i] = 'x';
                code[j] = 'y';
            }
    
    return hint;
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


bool Codebreaker::gameOver(string hint)
{
	if (hint.length() != codeLength)
		return false;

	for (int i = 0; i < hint.length(); i++)
		if (hint[i] != '*')
			return false;

	return true;
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


int Codebreaker::calcNumPosCodes()
{
	if (duplicates)
		return pow(8, codeLength);

	int x = 1;
	for (int i = 8; i > 8 - codeLength; i--)
		x = x * i;

	return x;
}












