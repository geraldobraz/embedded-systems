#include <ch.h>
#include <hal.h>

#define LED_PAD PORTB_LED1
#define LED_PORT IOPORT2

// Starting the new Thread
static THD_WORKING_AREA(waThread1, 32);
static THD_FUNCTION(Thread1, arg)
{
    while (true) {
        palTogglePad(LED_PORT, LED_PAD);// Toggle at the LED
        chThdSleepMilliseconds(100);// Thread sleeping
    }
}


// Main
int main(void)
{
    int step = 6;
    int width = step;

    static PWMConfig pwmcfg = {
        15625, 0x3FF, 0,
        {{PWM_OUTPUT_DISABLED, 0}, {PWM_OUTPUT_ACTIVE_HIGH, 0}}
    };

    halInit();
    chSysInit();

    pwmStart(&PWMD1, &pwmcfg); // Configure and start the pwm

    palSetPadMode(IOPORT2, 2, PAL_MODE_OUTPUT_PUSHPULL); // Setting the led pin as Output
    palClearPad(LED_PORT, LED_PAD);// Clearing the state

    chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO+1, Thread1, NULL);

    while (1) {        
        pwmEnableChannel(&PWMD1, 1, width);
        width += step;
        if ((width >= 0x3FF) || (width < 10)) {
            width -= step;
            step = -step;
        }
        chThdSleepMilliseconds(10);
    }

    return 0;
}
