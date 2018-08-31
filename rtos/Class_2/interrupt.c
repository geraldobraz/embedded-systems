// Includes
#include <avr/io.h>
#include <avr/cpufunc.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Defines  *********//
#define FOSC 16000000ul  // Clock Speed
#define BAUD 115200      // Baudrate
#define BAUDRATE (FOSC/(8ul*BAUD) - 1) 

// end Defines  *********//

// Global Variables *********//
int count = 0;

// end Global Variables  *********//

// ***** Configurations ***** //
// System  *********//
void systemInit(){
	DDRB = (1 << PB5); // Configure PB5 as Output
	DDRD = ~(1 << PD2); // INT0 as INPUT
    // Configuration of Uart Transmition RX-TX
	DDRD = (1 << PD1); // Configure Tx as Output
	DDRD = ~(1 << PD0); // Configure Rx as Input
	// sei(); // Enable all interrupts
}
 
// Uart  *********//
void uartInit(){
	UBRRH = (BAUDRATE >> 8);
	UBRRL = BAUDRATE;
	UCSRA = 0x00; // Clear the USART status register
	UCSRB = (1 << TXEN); // Enable the Tx and Rx 
	UCSRC = (1 << URSEL) | (1 << UCSZ1) | (UCSZO); 
	
}
// Interruption  *********//
void interruptInit(){
    /*
    Registers: 
    1. GICR
    2. MCUCR
    3. GIFR
    */
    GICR = 0x40;
    MCUCR = 0x01;
    sei();
}

ISR (INT0_vect){ //External interrupt_zero ISR
    PORTD = (1 << PD5);
}

// end Interruptions  *********// 

// ***** End of configurations ***** //

// Functions  *********//

// Main  *********//
int main(){

	systemInit(); 
    interruptInit();
    PORTD = ~(1 << PD5); // Start the program with the LED in LOW state

	


	// Endless Loop  *********//
	while(1){
	
	}

	return 0;
}