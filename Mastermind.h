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
  string makeCode(int length, bool dupes);
  void startGame();
  bool playGame(string theCode);
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
  mCode = makeCode(codeLength, duplicates);
  // Start Game
  startGame();
  // Play Game
  won = playGame(mCode);
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
    "A '-' indicates one guessed letter is in the code but not in the correct place. "
    << endl << endl;
}



int Mastermind::getCodeLength()
{
  int length = 0;
  string in;
  
  while (true)
  {
    cout << endl << "Please enter code length (4, 6, 8): ";
    cin >> in;
    
    if (isdigit(in[0]))
    {
      length = (int)in[0] - '0';
      if (length == 4 || length == 6 || length == 8)
        return length;
    }
    
    cout << "Invalid Code Length" << endl;
  }
  
  return 4;
}



bool Mastermind::allowDuplicates()
{
  string in;
  int dup = 0;
  
  while (true)
  {
    cout << endl << "Allow Duplicates?" << endl << "1. Yes" << endl << "2. No" << endl;
    cin >> in;
    
    if (isdigit(in[0]))
      dup = int(in[0]) - '0';
    else
    {
      if (in == "y" || in == "Y" || in == "yes" || in == "Yes")
        dup = 1;
      if (in == "n" || in == "N" || in == "no" || in == "No")
        dup = 2;
    }
    
    if (dup == 1)
      return true;
    if (dup == 2)
      return false;
    cout << "Invalid Response" << endl;
  }
  
  return false;
}



string Mastermind::makeCode(int length, bool dupes)
{
  srand(time(NULL));
  
  string code = "";
  vector<char> letters = mLetters;
  for (int i = 0; i < length; i++)
  {
    // Pick a new letter
    char newLetter = letters[rand() % letters.size()];
    
    // Duplicates Not Allowed?
    if (!dupes)
      // Find and remove each new letter from the code building blocks vector
      letters.erase( find(letters.begin(), letters.end(), newLetter) );
    
    code += newLetter;
  }
  return code;
}



void Mastermind::startGame()
{
  cout << endl;
  cout << "I've made a code using the letters 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'." << endl
       << "It will look something like: ";
  for (int i = 0; i < codeLength; i++)
    cout << mLetters[i];
  cout << "." << endl;
  cout << "For each letter you've guessed that is in the code I will respond with an asterisk (*) or a hyphen (-)." << endl;
  cout << "A '*' indicates one guessed letter is in the code and in the correct place." << endl;
  cout << "A '-' indicates one guessed letter is in the code but not in the correct place." << endl;
  cout << "Try to guess it!" << endl;
}



bool Mastermind::playGame(string theCode)
{
  int turns = 0;
  while (turns < 10)
  {
    cout << endl;
    turns++;
    string hint = "";
    string code = theCode;
    
    // Receive Guess
    cout << "Your Guess:  ";
    cin >> guess;
    // Check for perfect guess
    if (guess == code)
      return true;
    // Check for Forfeit
    if (guess == "Forfeit" || guess == "Lose")
      return false;
    // check valid guess
    if (guess.length() != codeLength)
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
          hint += '-';
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