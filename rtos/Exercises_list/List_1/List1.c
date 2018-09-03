/* Atividade da Lista 1
    Faça um programa para o Arduino que receba um comando pela porta serial e ative o pino conectado à entrada "trigger" do ultrassom. 
    Conecte a saída "echo" do ultrassom ao pino INT0 e configure sua interrupção para detectar o sinal de "echo" e ativar uma flag. 
    No programa principal, ao detectar a flag ativada, envie um byte com valor 48 pela porta serial.
*/

//***********************************//
// Includes
    #include <io.h>
    #include <cpufunc.h>
    #include <interrupt.h>
//"/usr/lib/avr/include/avr"
//***********************************//
// Defines
    #define FOSC 16000000ul  // Clock Speed
    #define BAUD 115200      // Baudrate
    #define BAUDRATE (FOSC/(8ul*BAUD) - 1) 
    #define BUFFER_SIZE 20   // USART buffer size
    #define echo PD2
    #define trigger PB0
//***********************************//

//***********************************//
// Global Variables
    uint8_t rx_buffer[BUFFER_SIZE];     /* buffer para transmissão */
    uint8_t rx_head, rx_tail;   /* ponteiros para o buffer circular */
    uint8_t flag = 0;
//***********************************//

//***********************************//
// Functions
    void delay_ms(uint16_t ms){
        uint16_t i, j;
        for(i=0; i<ms; i++)
            for(j=0; j<4000; j++)
                _NOP();
    }

    void send(){
        UDR = 48;
    }
    
//***********************************//

// ********* Configurations *********//
    //***********************************//
    // System
        void systemInit(){
            DDRB = (1 << trigger); // Configurate PB0 as Output
            DDRD = ~(1 << echo);// Configurzte PD2 as Input 
            sei();
            
        }
    //***********************************//
    // Uart
        void uartInit(){
            // Configuration of Uart Transmition RX-TX
            // DDRD = (1 << PD1); // Configure Tx as Output - erro
            // DDRD = ~(1 << PD0); // Configure Rx as Input - erro
            
            // Configurate the Baudrate
            UBRR0H = (BAUDRATE >> 8);
            UBRR0L = BAUDRATE;
            UBRR0H
            // UCSR0A = (1 << U2X0); /* double speed */
            UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);
            UCSR0C = (1 << UCSZ00) | (1 << UCSZ01) | (1 << URSEL);
            
        }
    //***********************************//
    // Interruptions
        // Interruption of RX 
        ISR(USART_RX_vect){

            unsigned char data = UDR0; // Recived data

            if(data == 's'){
                // Set High to trigger
                PORTB = (1 << trigger); // Trigger to HIGH
                flag = 1;
            }
        }

        // Interrupton of TX
            // ISR(USART_UDRE_vect){
            //     // send "48" throught 


            // }        
        // Interruption of INT0
        ISR(INT0_vect){
            // Sending 48 throught serial
            flag = 1;
        }
       
    //***********************************//
//***********************************//

//***********************************//
// Main 
    int main(){

        systemInit();
        uartInit();
        while(1){
        // Nothing to do
        if(flag){
            send();
            flag = 0;
        }

        }
    
        return 0;
    }
//***********************************//