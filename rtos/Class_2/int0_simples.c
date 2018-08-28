#include <avr/io.h>
#include <avr/interrupt.h>

avrdude -P /dev/ttyACM0 -c arduino -p atmega328p -b115200    -v -v -v -v -D -U flash:w:int0_simples.hex:i

/* -------------------------------------*/
void setup_int0(void)
{
    /*
     * Para as macros abaixo e outras, veja o arquivo
     * usr/lib/avr/include/avr/iom328p.h
     */
    EICRA |= (1 << ISC01) | (1<< ISC00);
    EIFR =  (1 << INTF0); //não precisa de |= porque é uma flag, e se tem o valor zero nada vai acontecer.
    EIMSK |= (1 << INT0);
    //EICRA |= 0x02;
}

/* -------------------------------------*/
void setup_main(void)
{
    LED_PORT &= ~(1 << LED_PINNBR);
    LED_DDR |= (1 << LED_PPINNBR);
    sei(); /* Seta flag principal de habilitação de interrupções */
}

/* -------------------------------------*/
volatile uint8_t has_changed;

ISR(INT0_vect)
{
    LED_PIN = (1 << LED_PINNBR);
    has_changed = 1;
}

/* -------------------------------------*/
int main(void)
{
    setup_int0();
    setup_main();

    while(1)
        ;

    return 0;
}
