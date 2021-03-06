#include <avr/io.h>
#include <avr/cpufunc.h>
#include <avr/interrupt.h>


/* 
 * Constantes utilizadas no cÃ³digo 
 */
#define FOSC 16000000ul  /* velocidade do relÃ³gio */
#define BAUD 115200      /* velocidade desejada para a serial */
#define MYUBRR (FOSC/(8ul*BAUD) - 1)  /* valor de 12 bits para configurar o clock da USART */

#define BUFFER_SIZE 20   /* Tamanho do buffer da USART */


/* 
 * DefiniÃ§Ã£o de funÃ§Ãµes 
 */
void setup_usart(void)
{
    /*
     * Primeiro setamos a direÃ§Ã£o certa para os pinos RX e TX
     */
    DDRD &= ~_BV(PD0);  /* USART RX bit para a placa Nano */
    DDRD |= _BV(PD1);  /* USART TX bit para a placa Nano */
    
    /*
     * Agora configuramos a USART
     */
    UBRR0H = (MYUBRR >> 8) & 0xF;
    UBRR0L = MYUBRR & 0xFF;
    UCSR0A = _BV(U2X0); /* double speed */
    UCSR0B = _BV(TXEN0) | _BV(RXCIE0) | _BV(RXEN0);
    UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);  /* Asynchronous, 8-N-1 */
}

/* -------------------------------------*/
void setup_timer1(void)
{
    /*
     * Agora configuramos setup_timer1 no modo Fast PWM com overflow
     * a cada 2 centÃ©simos de segundo
     */
    OCR1AH = 1250 >> 8;
    OCR1AL = 1250 & 0xFF;
    TCCR1A = _BV(WGM11) | _BV(WGM10);
    TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS12);
}

/* -------------------------------------*/
void setup_adc(void)
{
    PORTC &= ~_BV(PC0);
    DDRC &= ~_BV(PC0);
    ADMUX = _BV(REFS0); /* V_REF = Vcc, usando ADC0 */
    ADCSRB = _BV(ADTS2) | _BV(ADTS1); /* Trigger no timer1 overflow */
    DIDR0 = _BV(ADC0D); /* disabilita buffer de entrada no pin AC0 */
    /* Habilita mÃ³dulo e inicia conversÃ£o; habilita conversÃ£o automÃ¡tica
     * quando timer1 overflow; habilita interrupÃ§Ã£o */
    ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADATE) | _BV(ADIE) | \
             _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
}

/* -------------------------------------*/
void setup_main(void)
{
    LED_DDR |= _BV(LED_PINNBR);
    sei(); /* Seta flag principal de habilitaÃ§Ã£o de interrupÃ§Ãµes */
}

/* -------------------------------------*/
uint8_t tx_buffer[BUFFER_SIZE];     /* buffer para transmissÃ£o */
uint8_t tx_head, tx_tail;   /* ponteiros para o buffer circular */
uint8_t usart_transmitting;

uint8_t write(uint8_t c)
{
    uint8_t rc = 0;

    cli();
    if (!usart_transmitting) {
        UDR0 = c;
        UCSR0B |= _BV(UDRIE0);
        usart_transmitting = 1;
    } else {
        uint8_t next = tx_head + 1;
        if (next >= BUFFER_SIZE)
            next = 0;
        if (next != tx_tail) {
            tx_buffer[tx_head] = c;
            tx_head = next;
        } else  /* buffer cheio */
            rc = 1;
    }
    sei();
    return rc;
}

/* -------------------------------------*/
ISR(USART_UDRE_vect)
{
    if (tx_head == tx_tail) {
        UCSR0B &= ~_BV(UDRIE0);
        usart_transmitting = 0;
    } else {
        UDR0 = tx_buffer[tx_tail];
        if (++tx_tail >= BUFFER_SIZE)
            tx_tail = 0;
    }
}

/* -------------------------------------*/
uint8_t rx_buffer[BUFFER_SIZE];     /* buffer para transmissÃ£o */
uint8_t rx_head, rx_tail;   /* ponteiros para o buffer circular */

uint16_t read(void)
{
    uint8_t rc = 0, c = 0;

    cli();
    if (rx_head != rx_tail) {
        c = rx_buffer[rx_tail];
        if (++rx_tail >= BUFFER_SIZE)
            rx_tail = 0;
    } else 
        rc = 1;
    sei();
    return ((uint16_t) rc << 8) | c;
}

/* -------------------------------------*/
volatile uint8_t rcvd_byte;

ISR(USART_RX_vect)
{
    uint8_t next = rx_head + 1;
    if (next >= BUFFER_SIZE)
        next = 0;
    
    /* Se o buffer estiver cheio desprezamos o byte recebido mais antigo e
     * colocamos o byte que acabamos de receber em seu lugar */
    rx_buffer[rx_head] = UDR0;
    rx_head = next;
    if (next == rx_tail) { /* buffer cheio */
        rx_tail++;
        if (rx_tail >= BUFFER_SIZE)
            rx_tail = 0;
    }
    rcvd_byte = 1;
}

/* -------------------------------------*/
/*
 * Uma funÃ§Ã£o para dividir rapidamente por 5
 */
uint16_t div5u(uint16_t n)
{
        uint16_t q, r;

        q = (n >> 3) + (n >> 4);
        q = q + (q >> 4);
        q = q + (q >> 8);
        r = n - 5*q;

        return q + (13*r >> 6);
}

/* -------------------------------------*/
volatile uint8_t conversion_complete;
volatile uint16_t data;

ISR(ADC_vect)
{
    conversion_complete = 1;
    data = ADC;
    TIFR1 = _BV(TOV1);
}

/* -------------------------------------*/
#define MAX_DELTA 20
#define MAX_DELAY 200

int main(void)
{
    uint16_t old_data = 0;
    uint16_t counter = 0;
    int16_t delta;

    setup_timer1();
    setup_usart();
    setup_adc();
    setup_main();

    while(1) {
        while(!conversion_complete)
            ;
        conversion_complete = 0;
        delta = data - old_data;
        if (delta < 0)
            delta = -delta;
        old_data = data;
        if ((delta > MAX_DELTA) || (counter++ > MAX_DELAY)) {
            counter = 0;
            write(data & 0xFF);
            write((data >> 8) & 0xFF);
        }
    }

    return 0;
}