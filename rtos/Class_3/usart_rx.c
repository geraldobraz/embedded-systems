#include <avr/io.h>
#include <avr/cpufunc.h>
#include <avr/interrupt.h>


/* 
 * Constantes utilizadas no código 
 */
#define FOSC 16000000ul  /* velocidade do relógio */
#define BAUD 115200      /* velocidade desejada para a serial */
#define MYUBRR (FOSC/(8ul*BAUD) - 1)  /* valor de 12 bits para configurar o clock da USART */
#define BUFFER_SIZE 20   /* Tamanho do buffer da USART */

/* 
 * Definição de funções 
 */
void setup_usart(void)
{
  
    DDRD &= ~(1 << PD0);  /* USART RX bit para a placa Nano */
    DDRD |= (1 << PD1);  /* USART TX bit para a placa Nano */
    
    /*
     * Agora configuramos a USART
     */
    UBRR0H = (MYUBRR >> 8) & 0xF;
    UBRR0L = MYUBRR & 0xFF;
    UCSR0A = (1 << U2X0); /* double speed */
    UCSR0B = (1 << RXCIE0) | (1 << RXEN0);
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);  /* Asynchronous, 8-N-1 */
}

/* -------------------------------------*/
void setup_main(void)
{
    LED_DDR |= (1 << LED_PINNBR);
    sei(); /* Seta flag principal de habilitação de interrupções */
}

/* -------------------------------------*/
uint8_t rx_buffer[BUFFER_SIZE];     /* buffer para transmissão */
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
/*
 * Interrupt handlers devem ser definidos com a macros
 * ISR(), que é definida no arquivo avr/interrupt.h.
 * Veja também a página
 * http://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
 */
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
int main(void)
{
    uint16_t rc = 0;
    uint8_t c = 0;
    
    ++c; /* para calar o warning do compilador sobre c não estar sendo usado */

    setup_usart();
    setup_main();

    while(1) {
        if (rcvd_byte) {
            rc = read();
            if (rc & 0xFF00)
                continue;
            c = rc;
            rcvd_byte = 0;
        }
    }

    return 0;
}
