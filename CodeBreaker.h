// Mastermind CodeBreaker Class File
// Ian Cramer
// 9/6/18
//



#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>

#include "MastermindClass.h"

using namespace std;



class CodeBreaker
{
public:
  
  
  
private:
  
  int codeLength;
  bool duplicates;
  string mCode = "";
  vector<char> mLetters = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
  string guess = "";
  bool won = false;
  
  
};