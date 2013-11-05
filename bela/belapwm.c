/*
 * Copyright (C) 2011 Zoltan Zidarics (Zamek)
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/** \file pwm.c
    \brief PWM for temperature controlling
*/

#include <stdlib.h>
#include <string.h>

#include <mcuconf.h>
#include <ch.h>
#include <hal.h>
#include <chprintf.h>

#include "bela.h"
#include "adconv.h"

static void pwmCallback(PWMDriver *pwm);

/**
Configuration of first 4 pwm channel
*/
static const PWMConfig pwmConfig0 = {
    BELA_PWM_CLOCK,             // clock is 1Mhz
    BELA_PWM_INITIAL_PERIOD,    // period time is 10mSec
    pwmCallback,                // callback for every period
    {
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},  // active 1 and no callback at level changing
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},  // active 1 and no callback at level changing
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},  // active 1 and no callback at level changing
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},  // active 1 and no callback at level changing
    },
    0   // TIMER register initialization data
};

/**
Configuration of second 4 pwm channel
*/
static const PWMConfig pwmConfig1 = {
    BELA_PWM_CLOCK,             // clock is 1Mhz
    BELA_PWM_INITIAL_PERIOD,    // period time is 10mSec
    pwmCallback,                // callback for every period
    {
        {PWM_OUTPUT_ACTIVE_HIGH, NULL}, // active 1 and no callback at level changing
        {PWM_OUTPUT_ACTIVE_HIGH, NULL}, // active 1 and no callback at level changing
        {PWM_OUTPUT_ACTIVE_HIGH, NULL}, // active 1 and no callback at level changing
        {PWM_OUTPUT_ACTIVE_HIGH, NULL}, // active 1 and no callback at level changing
    },
    0   // TIMER register initialization data
};

/**
    Channel parameters for Fuzzy controller

*/
typedef struct PWMChannelPar {
    float tk;           // base temperature
    float dt;           // delta temp for fuzzy area
    float dutyCycle;    // processed duty cycle
    PWMDriver *driver;  // pwm driver for this channel
} PWMChannelParam;

static struct {
    volatile int running;
    PWMChannelParam channelParams[BELA_CHANNELS_NUM];
} pwmParams;

/**
    No need callback at every period
*/
static void pwmCallback(PWMDriver *pwm) {
    (void) pwm;
}

/**
    Process duty cycle of a channel

    Fuzzy idea:
    T<=Tk-dt :  dc = 1
    T>tk:       dc = 0
    Tk-dt<T<=Tk dc=(Tk-T)/dt

    \param ch selected channel to processing
*/
static void processChannel(int ch) {
    float temp = adGetChannelValue(ch);
    PWMChannelParam *cp = &pwmParams.channelParams[ch]; //
    float tkdt = cp->tk-cp->dt;

    if (temp <= tkdt)
        cp->dutyCycle = 1.0f;
    else if (temp > cp->tk)
            cp->dutyCycle = 0;
         else
            cp->dutyCycle = (cp->tk - temp)/cp->dt;

    pwmEnableChannel(cp->driver, ch%PWM_CHANNELS,
                     PWM_PERCENTAGE_TO_WIDTH(cp->driver, cp->dutyCycle*BELA_PWM_INITIAL_PERIOD));
}

static WORKING_AREA(waThreadPWM, 128);

/**
    PWM controlling task.
*/
static msg_t ThreadPWM(void *arg) {
    (void) arg;
    chRegSetThreadName("pwmcontrolling");

    while(TRUE) {
      int i;
      for (i=0;i<BELA_CHANNELS_NUM;i++)
         processChannel(i);

       chThdSleepMilliseconds(BELA_PWM_TASK_SLEEP_MS);
    }
    return 0;
}

/**
    PWM user interface
*/
void cmd_pwm(BaseSequentialStream *chp, int argc, char *argv[]) {
    (void) argc;
    (void) argv;
    int i;
    for (i=0;i<BELA_CHANNELS_NUM;i++) {
        PWMChannelParam *cp = &pwmParams.channelParams[i];
        chprintf(chp, "channel %d: tk=%f, dt=%f, dc=%f\r\n",i,cp->tk,cp->dt,cp->dutyCycle);
    }
}

void pwmInitilaize (void) {
    bzero(&pwmParams, sizeof(pwmParams));
    pwmParams.running = 1;

    palSetPadMode(BELA_PWM_PORT, BELA_PWM_CHANNEL_0, PAL_MODE_ALTERNATE(1));    // AF1 mode: TIM1/TIM2 to out
    palSetPadMode(BELA_PWM_PORT, BELA_PWM_CHANNEL_1, PAL_MODE_ALTERNATE(1));    // AF1 mode: TIM1/TIM2 to out
    palSetPadMode(BELA_PWM_PORT, BELA_PWM_CHANNEL_2, PAL_MODE_ALTERNATE(1));    // AF1 mode: TIM1/TIM2 to out
    palSetPadMode(BELA_PWM_PORT, BELA_PWM_CHANNEL_3, PAL_MODE_ALTERNATE(1));    // AF1 mode: TIM1/TIM2 to out
    palSetPadMode(BELA_PWM_PORT, BELA_PWM_CHANNEL_4, PAL_MODE_ALTERNATE(1));    // AF1 mode: TIM1/TIM2 to out
    palSetPadMode(BELA_PWM_PORT, BELA_PWM_CHANNEL_5, PAL_MODE_ALTERNATE(1));    // AF1 mode: TIM1/TIM2 to out
    palSetPadMode(BELA_PWM_PORT, BELA_PWM_CHANNEL_6, PAL_MODE_ALTERNATE(1));    // AF1 mode: TIM1/TIM2 to out
    palSetPadMode(BELA_PWM_PORT, BELA_PWM_CHANNEL_7, PAL_MODE_ALTERNATE(1));    // AF1 mode: TIM1/TIM2 to out

    pwmStart(&PWMD1, &pwmConfig0);
    pwmStart(&PWMD2, &pwmConfig1);

    int i;
    for (i=0;i<BELA_CHANNELS_NUM;i++) {
        PWMDriver *drv = i / PWM_CHANNELS ? &PWMD2 : &PWMD1;
        pwmParams.channelParams[i].driver = drv;
        pwmParams.channelParams[i].dt = BELA_PWM_DELTA_T;
        pwmEnableChannel(drv, i%PWM_CHANNELS, 5000);
    }

    chThdCreateStatic(waThreadPWM, sizeof(waThreadPWM), NORMALPRIO, ThreadPWM, NULL);

#ifdef BELA_PWM_DEBUGGING
    palClearPad(GPIOD, GPIOD_LED5);
    for (i=0;i<BELA_CHANNELS_NUM;i++)
        pwmParams.channelParams[i].tk = i*5.0f;
#endif

}
