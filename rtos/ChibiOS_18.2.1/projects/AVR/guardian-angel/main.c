// Guardian Angel Project
/**
 * Authors: Bianca Lisle
 *          Geraldo Braz
 */

// ************** Includes **************
#include "ch.h"
#include "hal.h"
#include "chprintf.h" 

// ************** Defines **************
#define NBR_CHANNELS 1
#define DEPTH 5
#define ADC_CONVERTER_FACTOR 0.00477922077922078 // ((2,5*1.0552519480519482)/552)

/* Definition of input ports */
#define RAIN_PORT 2 //PD2
#define DOOR_PORT 3 //PD3
#define VELOCITY_PORT_ANALOG 0 // IOPORT3

/* Definition of output ports */
#define BUZZER_PORT 4 //PD4
#define MOTOR_PORT 1 //IOPORT2 - PB1 = Pin 9 (PWM)

// **************************************** 

// Threads 
// FIXME: Study the possibility to create a thread for each sensor event
static THD_WORKING_AREA(waThread1, 32);
static THD_FUNCTION(Thread1, arg) {
  (void)arg;

  char buffer[200];

  chRegSetThreadName("read-high-priority-sensors");
  while (true) {
    /* Read velocity speed */
      //This will determine wether the bus has stoped or not. 
      //Do analog reading here

    /* Read Door sensor */
    palReadPad(IOPORT4, RAIN_PORT) == PAL_HIGH ? strncpy(buffer,"HIGH!\r\n",sizeof(buffer)) : strncpy(buffer,"LOW!\r\n",sizeof(buffer));
    chnWrite(&SD1, (const uint8_t *)buffer, strlen(buffer));
    chThdSleepMilliseconds(1000);
  }
}

// Global Variables
volatile uint8_t flag;

// State Machine
typedef enum{
    normal_state,
    is_door_opened,
    bus_overspeed,
    is_bus_stoped,
    is_raining
}states;


// Functions
void adc_cb(ADCDriver *adcp, adcsample_t *buffer, size_t n){
  flag = 1;
}

void init_ports() {

  /* Initialize input ports */
  palSetPadMode(IOPORT4, RAIN_PORT, PAL_MODE_INPUT);
  palSetPadMode(IOPORT4, DOOR_PORT, PAL_MODE_INPUT);
  /* Initialize output ports */
  palSetPadMode(IOPORT4, BUZZER_PORT, PAL_MODE_OUTPUT_PUSHPULL); //open drain?
  palSetPadMode(IOPORT2, MOTOR_PORT, PAL_MODE_OUTPUT_PUSHPULL); //open drain?

}


// Main
int main(void) {

  halInit();
  chSysInit();
  init_ports();

  // Start the Serial Communication
  sdStart(&SD1, NULL);


  // ADC config
  ADCConfig cfg = {ANALOG_REFERENCE_AVCC};
  ADCConversionGroup group = {0, NBR_CHANNELS, adc_cb, 0x7};
  adcsample_t buffer[DEPTH*NBR_CHANNELS];
  adcStart(&ADCD1, &cfg);


  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  while(TRUE) {
    adcStartConversion(&ADCD1, &group, buffer, DEPTH);
    while(!flag){

    }
      flag =0;
      
      
      for(int i = 0; i < DEPTH; i++){
      
        chprintf((BaseSequentialStream *)&SD1, "%.2f V\n\r",0.1911688311688311*buffer[i]);
        // chprintf((BaseSequentialStream *)&SD1, ">> %d\n\r",buffer[i]);
        chThdSleepMilliseconds(500);
      }
      
    
  }
}
