#include <iostream>
#include <vector>
#include <string.h>
#include <math.h>
#include <stdlib.h>

// A calculator with state machine

using namespace std;
enum states
{
    S1,
    S2,
    S3 

};

int main()
{
    // Creation of the variables
    int state = S1; // Waiting for the first number
    std::string c, s;
    vector<std::string> expression(0);

    int value1, value2 = 0;
    int op = 0;
    float answer = 0;

    cout << "####### Calculator #######" << endl;
    while (cin >> c)
    {

        switch (state)
        {
        case S1:
            // If c isn't a math operator
            if (c.compare("+") && c.compare("-") && c.compare("*") && c.compare("/")){
                s += c;
            }
            else
            {
                value1 = stoi(s); // Converting string to int
                state = S2;
                s.clear();
                if (!c.compare("+"))
                {
                    cout << "Add!" << endl;
                    op = 1;
                    state = S2;
                }
                else if (!c.compare("-"))
                {
                    op = 2;
                    state = S2;
                }
                else if (!c.compare("*"))
                {
                    op = 3;
                    state = S2;
                }
                else if (!c.compare("/"))
                {
                    op = 4;
                    state = S2;
                }
            }
            break;

        case S2:
        // Wainting for the next number
            cout << "S2" << endl;
            if (c.compare("=")){
                s += c;
            }
            else{
                value2 = stoi(s);
                state = S3;
                cout << state << endl;
            }

            break;

        case S3:
            // FIXME: It's not completing the operation 
            cout << "S3" << endl;
            cout << value1 + value2 << endl;    
            break;
        default:
            break;
        }
        
    }


    return 0;
}