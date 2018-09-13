/* Atividade da Lista 1
    Faça um programa para o Arduino que receba um comando pela porta serial e ative o pino conectado à entrada "trigger" do ultrassom. 
    Conecte a saída "echo" do ultrassom ao pino INT0 e configure sua interrupção para detectar o sinal de "echo" e ativar uma flag. 
    No programa principal, ao detectar a flag ativada, envie um byte com valor 48 pela porta serial.
*/

// Geraldo Braz
//***********************************//
// Includes
    #include <avr/io.h>
    #include <avr/cpufunc.h>
    #include <avr/interrupt.h>
    #include <util/delay.h>
//"/usr/lib/avr/include/avr"
// Defines
    #define FOSC 16000000ul  // Clock Speed
    #define BAUD 115200      // Baudrate
    #define BAUDRATE (FOSC/(8ul*BAUD) - 1) 
    #define BUFFER_SIZE 200   // USART buffer size

//***********************************//

//***********************************//
// Global Variables
    volatile uint8_t flag; 
    uint8_t tx_buffer[BUFFER_SIZE]; 
    uint8_t tx_head, tx_tail; 
    uint8_t usart_transmitting;
//**************************sistema*********//

// ********* Configurations *********//
    //***********************************//
    // Usart
        void usartInit(){
            // Configuration of Usart Transmition RX-TX
            DDRD |= (1 << PD1); // Configure Tx as Output
            DDRD |= ~(1 << PD0); // Configure Rx as Input
            
            // Configurate the Baudrate
            UBRR0H = (BAUDRATE >> 8);
            UBRR0L = BAUDRATE;
            // Configurate the registers
            UCSR0A = (1 << U2X0);
            UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);
            UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
            
        }

        //***********************************//
    // Interruption Configurations
        void interruptInit(){
            EICRA |= (1 << ISC00);    // set INT0 to trigger on ANY logic change
            EIMSK |= (1 << INT0);     // Turns on INT0
            sei();                    // turn on interrupts
        } 

    // System 
        void systemInit(){
            DDRB |= (1 << PB0); // Trigger as Output
            DDRD &= ~(1 << PD2); // Echo as Input            
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
    // Interruptons    
        // Interruption of RX 
        ISR(USART_RX_vect){
            // unsigned char data = UDR0; // Recived data
            // Start the Infrared sensor
            if(UDR0 == 's'){
                PORTB |= (1 << PB0);  // Trigger to HIGH   
                _delay_us(10);
                PORTB &= ~(1 << PB0);  // Trigger to LOW   
            }
            
        }
        // Interrupton of TX
        ISR(USART_UDRE_vect){
            cli();//Disables all interrupts by clearing the global interrupt mask.
            flag = 0; // Desables the the flag to send a message
            delay_ms(10);
            sei(); //Enables interrupts by setting the global interrupt mask

        }        
        // Interruption of INT0 
        ISR(INT0_vect){
            
            cli(); //Disables all interrupts by clearing the global interrupt mask.
            flag = 1; // Enables the the flag to send a message
            delay_ms(10); 
            sei(); //Enables interrupts by setting the global interrupt mask
            
        }
       
    //***********************************//
//***********************************//

//***********************************//
// Main 
    int main(void){

        usartInit(); // Initialaze the Uart parameters
        interruptInit(); // Initialize the Interruptions parameters
        systemInit(); // Initializer the System parameters
        
        while(1){
            _delay_ms(100);
            if(flag){
                write(0x48); // Sending 0x48 to the serial
            }        

        }
    
        return 0;
    }
//***********************************//

