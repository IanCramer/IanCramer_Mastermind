// Mastermind Game Class Header
// Ian Cramer
//

#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <algorithm>

using namespace std;


// -------------------- Mastermind Class Signature --------------------

class Mastermind
{
public:
    // constructor
    Mastermind();
    
    // Rules
    static void getRules();
    
private:
    // Helper Methods
    bool allowDuplicates();
    int getCodeLength();
    string makeCode();
    bool validCode(string code);
    void startGame();
    bool playGame();
    void gameOver(bool result);

    
    
    
    
    // Member Variables
    int codeLength;
    bool duplicates;
    string mCode = "";
    vector<char> mLetters = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    string guess = "";
    bool won = false;
    
};

// -------------------- End Mastermind Class Signature --------------------



Mastermind::Mastermind()
{
    // Get Code Length
    codeLength = getCodeLength();
    // Allow Duplicates?
    duplicates = allowDuplicates();
    // Make Code
    mCode = makeCode();
    // Start Game
    startGame();
    // Play Game
    won = playGame();
    // Game Over
    gameOver(won);
}



void Mastermind::getRules()
{
    cout << endl << "Rules:" << endl <<
        "Mastermind is a code breaking game. "
        "The computer will generate a 4, 6, or 8 letter code using the first 8 letters of the alphabet (a, b, c, d, e, f, g, h). "
        "You may determine whether the code can contain duplicate letters "
        "(an example code with duplicates: adab; an example code without duplicates: agcf). "
        "You will have 10 attempts to guess the code. "
        "After each guess, the computer will tell you how many letters you have that are in the code and whether they are in the right place. "
        "A '*' indicates one guessed letter is in the code and in the correct place. "
        "A '+' indicates one guessed letter is in the code but not in the correct place. "
        << endl << endl;
}



int Mastermind::getCodeLength()
{
    string in;
    
    while (true)
    {
        cout << endl << "Please enter code length (4, 6, 8): ";
        cin >> in;
        
        if (in[0] == '4' || in[0] == '6' || in[0] == '8')
            return int(in[0]) - '0';
        
        cout << "Invalid Code Length" << endl;
    }
    
    return 4;
}



bool Mastermind::allowDuplicates()
{
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



string Mastermind::makeCode()
{
    srand(time(NULL));
    
    string code = "";
    vector<char> letters = mLetters;

    for (int i = 0; i < codeLength; i++)
    {
        // Pick a new letter
        char newLetter = mLetters[rand() % mLetters.size()];
        
        // Duplicates Not Allowed?
        if (!duplicates)
            // Find and remove each new letter from the code building blocks vector
            letters.erase( find(letters.begin(), letters.end(), newLetter) );
        
        // Add new letter to code
        code += newLetter;
    }

    return code;
}

bool Mastermind::validCode(string code)
{
    // Bad code length, make user reenter
    if (code.length() != codeLength)
        return false;

    // Bad code letters, make user reenter
    for (int i = 0; i < code.length(); i++)
        if (code[i] < 'a' || code[i] > 'h')
            return false;

    // If duplicates not allowed, check for duplicates
    if (!duplicates)
        for (int i = 0; i < code.length(); i++)
            for (int j = i + 1; j < code.length(); j++)
                if (code[i] == code[j])
                    return false;

    return true;
}




void Mastermind::startGame()
{
    cout << endl;
    cout << "I've made a code using the letters 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'." << endl
             << "It will look something like: ";
    for (int i = 0; i < codeLength; i++)
        cout << mLetters[i];
    cout << "." << endl;
    cout << "For each letter you've guessed that is in the code I will respond with an asterisk (*) or a hyphen (+)." << endl;
    cout << "A '*' indicates one guessed letter is in the code and in the correct place." << endl;
    cout << "A '+' indicates one guessed letter is in the code but not in the correct place." << endl;
    cout << "Try to guess it!" << endl;
}



bool Mastermind::playGame()
{
    int turns = 0;
    while (turns < 10)
    {
        cout << endl;
        turns++;
        string hint = "";
        string code = mCode;
        
        // Receive Guess
        cout << "Your Guess:    ";
        cin >> guess;

        // Check for perfect guess
        if (guess == code)
            return true;
        // Check for Forfeit
        if (guess == "Forfeit" || guess == "Lose")
            return false;
        // check valid guess
        if (!validCode(guess))
        {
            cout << "Invalid Guess. Guess Again." << endl;
            continue;
        }
        
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
        
        cout << hint << endl;
        
        if (hint == "****")
            return true;
    }
    return false;
}



void Mastermind::gameOver(bool result)
{
    cout << endl;
    
    if (result)
        cout << "Congratulations! You Guessed It! You Win!" << endl;
    else
        cout << "Too Bad, You Lose. Better Luck Next Time." << endl;
    
    cout << "The code was: " << mCode << endl;
}


