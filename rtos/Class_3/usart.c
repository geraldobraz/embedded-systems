//***********************************//
// Includes
    #include <avr/io.h>
    #include <avr/cpufunc.h>
    #include <avr/interrupt.h>
    #include <stdio.h>
//***********************************//
// Defines
    #define FOSC 16000000ul  // Clock Speed
    #define BAUD 115200      // Baudrate
    #define BAUDRATE (FOSC/(8ul*BAUD) - 1) 
    #define BUFFER_SIZE 20   // USART buffer size
//***********************************//

//***********************************//
// Global Variables
    uint8_t rx_buffer[BUFFER_SIZE];     /* buffer para transmissão */
    uint8_t rx_head, rx_tail;   /* ponteiros para o buffer circular */

//***********************************//

//***********************************//
// Functions
    void delay_ms(uint16_t ms)
    {
        uint16_t i, j;
        for(i=0; i<ms; i++)
            for(j=0; j<4000; j++)
                _NOP();
    }
//***********************************//

// ********* Configurations *********//
    //***********************************//
    // System
        void systemInit(){
            DDRB = (1 << PB0); // Configure PB0 as Output
            sei();
        }
    //***********************************//
    // Uart
        void uartInit(){
            // Configuration of Uart Transmition RX-TX
            DDRD = (1 << PD1); // Configure Tx as Output
            DDRD = ~(1 << PD0); // Configure Rx as Input
            // Configurate the Baudrate
            UBRR0H = (BAUDRATE >> 8);
            UBRR0L = BAUDRATE;
            
            UCSR0A = (1 << U2X0); /* double speed */
            UCSR0B = (1 << RXCIE0) | (1 << RXEN0);
            UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
            
        }
    //***********************************//
    // Interruptions
        ISR(USART_RX_vect){

        rx_buffer[rx_head] = UDR0;


        }
    //***********************************//
//***********************************//

//***********************************//
// Main 
    int main(){

        systemInit();
        uartInit();
        while(1){

        }
    
        return 0;
    }
//***********************************//