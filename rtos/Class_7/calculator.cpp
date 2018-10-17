#include <iostream>
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

enum operations{
    add = 1,
    sub,
    mult,
    div_
};

int main()
{
    // Creation of the variables
    int state = S1; // Waiting for the first number
    std::string c, s;
    operations op;
    float value1, value2 = 0.0;
    float answer = 0;
    cout << "####### Calculator #######" << endl;
    while (cin >> c)
    {
        if(!c.compare("c") || !c.compare("C")){
            state = S3;
        }
        switch (state)
        {
            case S1:
                // If c isn't a math operator
                if (c.compare("+") && c.compare("-") && c.compare("*") && c.compare("/")){
                    s += c;
                }
                else
                {   
                    if(!s.compare("")){
                        value1 = answer;
                    }else{
                        value1 = stof(s); // Converting string to int
                    }
                    state = S2;
                    s.clear();
                    if (!c.compare("+"))
                    {
                        op = add;
                    }
                    else if (!c.compare("-"))
                    {
                        op = sub;
                    }
                    else if (!c.compare("*"))
                    {
                        op = mult;
                    }
                    else if (!c.compare("/"))
                    {
                        op = div_;
                    }
                    
                }
                break;

            // Wainting for the next number
            case S2:
                if (c.compare("=")){
                    s += c;
                }
                else{
                    value2 = stof(s);
                    cout << "**********\n" << "Value1: " << value1 << "\nValue2: " << value2 << "\n**********\n"<< "s: " << s << "\nc: " << c<< endl;
                    
                    switch(op){
                        case add: 
                            answer = value1+value2;
                            cout << "Result: " << value1 << " + " << value2 << " = " << answer << "\n**********\n" << endl;        
                            break;
                        case sub:
                            answer = value1-value2;
                            cout << "Result: " << value1 << " - " << value2 << " = " << answer << "\n**********\n" << endl;  
                            break;
                        case mult:
                            answer = value1*value2;
                            cout << "Result: " << value1 << " * " << value2 << " = " << answer << "\n**********\n" << endl;  
                            break;
                        case div_:
                            answer = (float)value1/(float)value2;
                            cout << "Result: " << value1 << " / " << value2 << " = " << answer << "\n**********\n" << endl;   
                            break;
                            
                        default:
                            break;

                    } 
                    s.clear();
                    value1 = 0;
                    value2 = 0;
                    state = S1;
                }

                break;

            case S3:
                cout << "# Cancel" << endl;
                s.clear();
                value1 = 0;
                value2 = 0;
                state = S1;
                break;
            default:
                break;
        }
        
    }


    return 0;
}