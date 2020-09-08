// Mastermind Game
// Ian Cramer
//

#include "Mastermind.h"
#include "Codebreaker.h"

using namespace std;

int main()
{
    string in;
    
    
    
    while (true)
    {
        cout << endl << "Would you like to play a game of Mastermind?" << endl;
        cout << "1. Yes" << endl << "2. No" << endl << "3. What are the rules?" << endl;
        cin >> in;
        
        // Would user like to play?
        // No
        if (in[0] == '2' || tolower(in[0]) == 'n')
            return 0;
        // Get Rules
        else if (in[0] == '3')
        {
            Mastermind::getRules();
            continue;
        }
        // Invalid Response
        else if (in[0] != '1' && tolower(in[0]) != 'y')
            continue;

        // A valid response of yes is handled automatically.

        // Would user like to be code maker or breaker?
        int role = 0;
        while (role != 1 && role != 2)
        {
            cout << "Which would you like to do?" << endl;
            cout << "1. Code Breaker" << endl;
            cout << "2. Code Maker" << endl;

            cin >> in;

            if (isdigit(in[0]))
                role = int(in[0]) - '0';
        }

        if (role == 1)
            Mastermind game;
        else
            Codebreaker game;
    }
    
    // End Mastermind Game
    
    
    return 0;
}