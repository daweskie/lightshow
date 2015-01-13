#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "ch.h"
#include "hal.h"

#include "chprintf.h"

#include "myMisc.h"
#include "config.h"
#include "menu.h"
#include <chsem.h>


/*===========================================================================*/
/* Generic code.															 */
/*===========================================================================*/

/* Virtual serial port over USB.*/
static SerialUSBDriver SDU1;


/*
 * Lightshow thread, times are in milliseconds.
 */
static WORKING_AREA(waThread1, 128);

static msg_t Thread1(void *arg)
{
    (void)arg;
    chRegSetThreadName("blinker1");



    int leds[] = { led1 , led2 , led3 , led4 };
    int curLed = 0;
    while (TRUE)
    {

        if (presses == 1)
        {
            presses = 0;
            chBSemSignal(&my_bsem);
            if(dir == 1)
            {
                dir = 0;
            }
            else if(dir == 0)
            {
                dir = 1;
            }
        }

        int i = 0;
        /**
        while ((palReadPad(bt1ch, bt1)) && (i < 30000))
        {
            i++;
            if (i < 3000)
            {
                if(dir == 1)
                {
                    dir = 0;
                }
                else if(dir == 0)
                {
                    dir = 1;
                }
            }

            else if ( i > 5001)
            {
                if (updown == 0)
                {
                    blinkspeed  = blinkspeed + 30;
                    if (blinkspeed > blinkmax)
                        {
                            blinkspeed = blinkmax;
                            updown = 1;
                        }
                }
                else if (updown == 1)
                {
                    blinkspeed = blinkspeed - 30;
                    if (blinkspeed < blinkmin)
                    {
                        blinkspeed = blinkmin;
                        updown = 0;
                    }

                }

            }
            chThdSleepMilliseconds(1);
            //chprintf((BaseSequentialStream*)&SDU1, "value of i : %d \r\n", i);

        }
        */
        palSetPad(GPIOD, leds[ curLed ]);
        chThdSleepMilliseconds(blinkspeed);
        palClearPad(GPIOD, leds[ curLed ]);
        chThdSleepMilliseconds(blinkspeed);

        if (dir == 1 )
        {
            curLed++;
            if (curLed > 3)
            {
                curLed = 0;
            }

        }
        if (dir == 0)
        {
            curLed--;
            if (curLed < 0 )
            {
                curLed = 3;
            }
        }

    }

    return 0;
}


void startBlinker(void)
{
    chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

}







