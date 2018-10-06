#include <stdio.h>
// A calculator with state machine

typedef enum {
    S1,// Esperando o primeiro numero
    S2,// Esperando o operador
    S3,// Esperando o segundo numero
    S4,// Esperando o Igual
    S5 // Esperando o C 
}state;


int main(){
      // Creation of the variables
    int estado = S1; // Waiting for the first number
    char c;
    float answer = 0; 

    while(1){
        scanf("%c", c);
        switch(estado){
                case S1:
                    if (c == '-'){
                        // May the first number is negative or it is trying to subtract from the last answer
                        
                    }
                    break;
                case S2:
                    if(c == '*'){
                        estado = S3;
                    }else if(c == '/'){
                        
                    }else{
                        estado = S1;
                        
                    }
                    break;
                case S3:
                    if(c == '*'){
                        estado = S4;
                    }
                    break;
                case S4:
                    if(c == '/'){
                        estado = S1;
                    }else if(c!= '/' && c != '*'){
                        estado = S3;
                    }
                    break;
                default:
                    break;
            }
    }
            




    


    return 0;
}