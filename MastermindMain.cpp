// Mastermind Game
// Ian Cramer
//

#include "MastermindClass.h"

using namespace std;

int main()
{
  string in;
  int playGame = 0;
  int playAgain = 0;
  
  
  
  while (playGame != 1 && playGame != 2)
  {
    cout << endl << "Would you like to play a game of Mastermind?" << endl;
    cout << "1. Yes" << endl << "2. No" << endl << "3. What are the rules?" << endl;
    cin >> in;
    
    if (isdigit(in[0]))
      playGame = int(in[0]) - '0';
    else
    {
      if (in == "y" || in == "Y" || in == "yes" || in == "Yes")
        playGame = 1;
      if (in == "n" || in == "N" || in == "no" || in == "No")
        playGame = 2;
    }
    
    if (playGame == 3)
      Mastermind::getRules();
  }
  
  if (playGame == 2)
    return 0;
  
  // Mastermind Game
  while (playGame == 1)
  {
    Mastermind game;
    cout << "Would you like to play again?" << endl << "1. Yes" << endl << "2. No" << endl;
    
    bool response = false;
    while (!response)
    {
      cin >> in;
      // Responded with number
      if ( isdigit(in[0]) )
        playGame = int(in[0]) - '0';
      // Responded with string
      else if (in == "y" || in == "Y" || in == "yes" || in == "Yes")
        playGame = 1;
      // said no
      else if (in == "n" || in == "N" || in == "no" || in == "No")
        return 0;
      
      if (playGame == 1 || playGame == 2)
        response = true;
      else
        cout << "Invalid Response. Try Again: " << endl;
    }
  }
  
  // End Mastermind Game
  
  
  return 0;
}