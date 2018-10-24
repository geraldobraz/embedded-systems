#include <avr/io.h>
#include <util/delay.h>

// PINO 8 -> PB0

int main()
{
    DDRB = (1 << PB0);       // Configure PB0 as Output
    DDRB = (1 << PB1); 
    DDRB = (1 << PB2); 
    while(1)
    {
        PORTB |= (1 << PB0); // Set PB0 to HIGH
        _delay_ms(250);      // Wait for some time
        PORTB |= (1 << PB1); // Set PB0 to HIGH
        _delay_ms(250);      // Wait for some time
        PORTB |= (1 << PB2); // Set PB0 to HIGH
        _delay_ms(250);      // Wait for some time
        
        PORTB &= ~(1 << PB0);  // Set PB0 to LOW
        _delay_ms(250);    // Wait for some time
        PORTB &= ~(1 << PB1);  // Set PB0 to LOW
        _delay_ms(250);    // Wait for some time
        PORTB &= ~(1 << PB2);  // Set PB0 to LOW
        _delay_ms(250);    // Wait for some time

    }
    return 0;
}