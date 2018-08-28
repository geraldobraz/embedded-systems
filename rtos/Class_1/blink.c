#include <avr/io.h>
#include <util/delay.h>

// PINO 8 -> PB0

int main()
{
    DDRB = (1 << PB0);       // Configure PB0 as Output

    while(1)
    {
        PORTB = (1 << PB0); // Set PB0 to HIGH
        _delay_ms(50);      // Wait for some time
        PORTB = ~(1 << PB0);  // Set PB0 to LOW
        _delay_ms(500);    // Wait for some time
    }
    return 0;
}