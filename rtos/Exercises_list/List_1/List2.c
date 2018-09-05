#include <avr/io.h>
#include <avr/cpufunc.h>
#include <avr/interrupt.h>

#define FOSC 16000000ul  /* velocidade do relógio */
#define BAUD 57600      /* velocidade desejada para a serial */
#define MYUBRR (FOSC/(8ul*BAUD) - 1)  /* valor de 12 bits para configurar o clock da USART */

#define BUFFER_SIZE 200   /* Tamanho do buffer da USART */

void uartInit(){
    // Configuration of Uart Transmition RX-TX
    // DDRD = (1 << PD1); // Configure Tx as Output - erro
    // DDRD = ~(1 << PD0); // Configure Rx as Input - erro
    
    DDRD = (1 << PD1); // Configurate PD1 as Output
    DDRD = ~(1 << PD0); // Configurate PD0 as Input
    
    // Configurate the Baudrate
    UBRR0H = (BAUDRATE >> 8);
    UBRR0L = BAUDRATE;
    UBRR0H
    // UCSR0A = (1 << U2X0); /* double speed */
    UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01) | (1 << URSEL);
    
}

void systemInit(){
    // DDRB = (1 << trigger); // Configurate PB0 as Output
    // DDRD = ~(1 << echo);// Configurzte PD2 as Input 
    sei();
    
}

int main(void){

    uartInit();
    systemInit();

    while(1){


    }


}

