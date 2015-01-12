#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "ch.h"
#include "hal.h"

#include "chprintf.h"

#include "myMisc.h"


/*===========================================================================*/
/* Generic code.															 */
/*===========================================================================*/

/* Virtual serial port over USB.*/
static SerialUSBDriver SDU1;


void cmd_mem(BaseSequentialStream *chp, int argc, char *argv[])
{
    size_t n, size;

    (void)argv;
    if (argc > 0)
    {
        chprintf(chp, "Usage: mem\r\n");
        return;
    }
    n = chHeapStatus(NULL, &size);
    chprintf(chp, "core free memory : %u bytes\r\n", chCoreStatus());
    chprintf(chp, "heap fragments    : %u\r\n", n);
    chprintf(chp, "heap free total  : %u bytes\r\n", size);
}

void cmd_threads(BaseSequentialStream *chp, int argc, char *argv[])
{
    static const char *states[] = {THD_STATE_NAMES};
    Thread *tp;

    (void)argv;
    if (argc > 0)
    {
        chprintf(chp, "Usage: threads\r\n");
        return;
    }
    chprintf(chp, "%10s %10s %6s %6s %11s %7s %17s \r\n",
             "add", "stack", "prio", "refs", "state", "time", "name");
    tp = chRegFirstThread();
    do
    {
        chprintf(chp, "%.10lx %.10lx %6lu %6lu %11s %7lu %17.15s \r\n",
                 (uint32_t)tp, (uint32_t)tp->p_ctx.r13,
                 (uint32_t)tp->p_prio, (uint32_t)(tp->p_refs - 1),
                 states[tp->p_state], (uint32_t)tp->p_time,
                 (uint32_t)tp->p_name);
        tp = chRegNextThread(tp);
    }
    while (tp != NULL);
}

void cmd_toggle(BaseSequentialStream *chp, int argc, char *argv[])
{
    (void)argv;
    if (argc != 1)
    {
        chprintf(chp, "Usage: toggle #led\r\n");
        return;
    }
    if(argv[0][0]=='1')
    {
        palTogglePad(GPIOD, GPIOD_LED3);
    }
    else if(argv[0][0]=='2')
    {
        palTogglePad(GPIOD, GPIOD_LED4);
    }
    else if(argv[0][0]=='3')
    {
        palTogglePad(GPIOD, GPIOD_LED5);
    }
    else if(argv[0][0]=='4')
    {
        palTogglePad(GPIOD, GPIOD_LED6);
    }
}

int blinkspeed = 50;
int blinkmin = 30;
int blinkmax = 300;

void cmd_blinkspeed(BaseSequentialStream *chp, int argc, char *argv[])
{

    (void)argv;
    int speed = 30;
    if (argc != 1)
    {

        return;
    }
    speed = atoi(argv[0]);
    if(speed > 5000)
        speed = 5000;
    if(speed < 5)
        speed = 5;
    blinkspeed = speed;
}
void cmd_blinkspeed_min_ch(BaseSequentialStream *chp, int argc, char *argv[])
{

    (void)argv;
    int speed = 30;
    if (argc != 1)
    {

        return;
    }
    speed = atoi(argv[0]);
    if(speed > 5000)
        speed = 5000;
    if(speed < 5)
        speed = 5;
    blinkmin = speed;
}
void cmd_blinkspeed_max_ch(BaseSequentialStream *chp, int argc, char *argv[])
{

    (void)argv;
    int speed = 30;
    if (argc != 1)
    {

        return;
    }
    speed = atoi(argv[0]);
    if(speed > 5000)
        speed = 5000;
    if(speed < 5)
        speed = 5;
    blinkmax = speed;
}
void cmd_blinkspeed_now(BaseSequentialStream *chp, int argc, char *argv[])
{

    (void)argv;
    if (argc != 1)
    {

        chprintf(chp, "Current speed : %d [ms]\r\n", blinkspeed);
        return;
    }

}

void cmd_blinkspeed_min(BaseSequentialStream *chp, int argc, char *argv[])
{

    (void)argv;
    if (argc != 1)
    {

        chprintf(chp, "Minimum blink speed is %d [ms]\r\n", blinkmin);
        return;
    }

}

void cmd_blinkspeed_max(BaseSequentialStream *chp, int argc, char *argv[])
{

    (void)argv;
    if (argc != 1)
    {

        chprintf(chp, "Maximum blink speed is: %d [ms]\r\n", blinkmax);
        return;
    }

}
int dirway = 0;
int dir = 0;
void cmd_way(BaseSequentialStream *chp, int argc, char *argv[])
{

    (void)argv;
    if (argc != 1)
    {
        if (dir == 1)
        {
         chprintf(chp, "The direction is clockwise.\r\n");
        }
        else if (dir == 0)
        {
        chprintf(chp, "The direction is counter-clockwise.\r\n");
        }

        return;
    }

}

void cmd_way_ch(BaseSequentialStream *chp, int argc, char *argv[])
{

    (void)argv;
    int speed = 30;

    if (argc != 1)
    {
        chprintf(chp, "Usage: 0 = clockwise and 0 = counter-clockwise.\r\n");
        return;

    }
    speed = atoi(argv[0]);
    if(speed > 1)
        speed = 1;
    if(speed < 0)
        speed = 0;
    dir = speed;
}

/*
 * Lightshow thread, times are in milliseconds.
 */
static WORKING_AREA(waThread1, 128);


static msg_t Thread1(void *arg)
{
    (void)arg;
    chRegSetThreadName("blinker1");

    int updown = 0;

    static int leds[] = { GPIOD_LED3 , GPIOD_LED5 , GPIOD_LED6 , GPIOD_LED4 };
    int curLed = 0;
    while (TRUE)
    {

        int i = 0;

        while ((palReadPad(GPIOA, GPIOA_BUTTON)) && (i < 30000))
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







