/* Atividade da Lista 1
    Faça um programa para o Arduino que receba um comando pela porta serial e ative o pino conectado à entrada "trigger" do ultrassom. 
    Conecte a saída "echo" do ultrassom ao pino INT0 e configure sua interrupção para detectar o sinal de "echo" e ativar uma flag. 
    No programa principal, ao detectar a flag ativada, envie um byte com valor 48 pela porta serial.
*/

//***********************************//
// Includes
    #include <avr/io.h>
    #include <avr/cpufunc.h>
    #include <avr/interrupt.h>
    #include <util/delay.h>
//"/usr/lib/avr/include/avr"
//***********************************//
// Defines
    #define FOSC 16000000ul  // Clock Speed
    #define BAUD 115200      // Baudrate
    #define BAUDRATE (FOSC/(8ul*BAUD) - 1) 
    #define BUFFER_SIZE 200   // USART buffer size

//***********************************//

//***********************************//
// Global Variables
    volatile uint8_t flag; 
    uint8_t tx_buffer[BUFFER_SIZE];     /* buffer para transmissão */
    uint8_t tx_head, tx_tail;   /* ponteiros para o buffer circular */
    uint8_t usart_transmitting;
//***********************************//

// ********* Configurations *********//
    //***********************************//
    // Uart
        void uartInit(){
            // Configuration of Uart Transmition RX-TX
            DDRD |= (1 << PD1); // Configure Tx as Output
            DDRD |= ~(1 << PD0); // Configure Rx as Input
            
            // Configurate the Baudrate
            UBRR0H = (BAUDRATE >> 8);
            UBRR0L = BAUDRATE;
            UCSR0A = (1 << U2X0);
            UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);
            UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
            
        }

        //***********************************//
    // Interruption
     void interruptInit(){
            EICRA |= (1 << ISC00);    // set INT0 to trigger on ANY logic change
            EIMSK |= (1 << INT0);     // Turns on INT0
            sei();                    // turn on interrupts
        } 

    // System 
        void systemInit(){
            DDRB |= (1 << PB0); // Trigger as Output
            DDRD &= ~(1 << PD2); // Echo as Input            
            // DDRB |= (1 << PB1); // Trigger as Output
            // DDRB |= (1 << PB2);
        }
       
    //***********************************//
    
    // Functions
        void delay_ms(uint16_t ms){
            uint16_t i, j;
            for(i=0; i<ms; i++)
                for(j=0; j<4000; j++)
                    _NOP();
        }

        // Write on serial one byte
        uint8_t write(uint8_t c){
            uint8_t rc = 0;

            cli();
            if (!usart_transmitting) {
                UDR0 = c;
                UCSR0B |= (1 << UDRIE0);
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
    //***********************************//
    
    //***********************************//
    // Interruption of RX 
        ISR(USART_RX_vect){
            unsigned char data = UDR0; // Recived data
            // Start the Infrared sensor
            if(data == 's'){
                PORTB = 0x01;  // Trigger to HIGH   
                _delay_us(10);
                PORTB = 0x00;  // Trigger to LOW   
            }
            
        }

        // Interrupton of TX
        ISR(USART_UDRE_vect){
            cli();
            PORTB |= (1 << PB2);
            flag = 0;
            delay_ms(10);
            sei();

        }        
        // Interruption of INT0 
        ISR(INT0_vect){
            
            cli();
            flag = 1; // Enable the the flag to send a message
            delay_ms(10);
            sei();
            
        }
       
    //***********************************//
//***********************************//

//***********************************//
// Main 
    int main(void){

        uartInit();
        interruptInit();
        systemInit();
        
        while(1){
        // Nothing to do
            _delay_ms(100);
            PORTB &= ~(1 << PD2);
            if(flaggy){
                write(0x48); 
            }        

        }
    
        return 0;
    }
//***********************************//