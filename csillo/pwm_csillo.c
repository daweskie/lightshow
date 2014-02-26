/*
 * Copyright (C)  2013 Zoltan Zidarics (Zamek)
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/** \file
    \brief
*/

#include <stdlib.h>
#include <string.h>

#include <mcuconf.h>
#include <chconf.h>
#include <halconf.h>

#include <ch.h>
#include <hal.h>
#include <chprintf.h>
#include <pwm.h>

#include "csillo.h"

#include "pwm_csillo.h"

#define CSILLO_PWM_CLOCK 1000000
#define CSILLO_PWM_INITIAL_PERIOD 10000

#define CSILLO_PWM_PORT0 GPIOE
#define CSILLO_PWM_PORT1 GPIOB

#define CSILLO_PWM_CHANNEL_0 GPIOE_PIN9
#define CSILLO_PWM_CHANNEL_1 GPIOE_PIN11
#define CSILLO_PWM_CHANNEL_2 GPIOE_PIN13
#define CSILLO_PWM_CHANNEL_3 GPIOE_PIN14
#define CSILLO_PWM_CHANNEL_4 6
#define CSILLO_PWM_CHANNEL_5 7
#define CSILLO_PWM_CHANNEL_6 8
#define CSILLO_PWM_CHANNEL_7 9
#define CSILLO_PWM_CHANNEL_8 GPIOB_PIN11

static BaseSequentialStream *console;

static void pwmCallback(PWMDriver *pwm) {

}

static const PWMConfig pwmConfig0 = {
    CSILLO_PWM_CLOCK,             // clock is 1Mhz
    CSILLO_PWM_INITIAL_PERIOD,    // period time is 10mSec
    pwmCallback,                // callback for every period
    {
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},  // active 1 and no callback at level changing
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},  // active 1 and no callback at level changing
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},  // active 1 and no callback at level changing
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},  // active 1 and no callback at level changing
    },
    0   // TIMER register initialization data
};

static const PWMConfig pwmConfig1 = {
    CSILLO_PWM_CLOCK,             // clock is 1Mhz
    CSILLO_PWM_INITIAL_PERIOD,    // period time is 10mSec
    pwmCallback,                // callback for every period
    {
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},  // active 1 and no callback at level changing
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},  // active 1 and no callback at level changing
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},  // active 1 and no callback at level changing
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},  // active 1 and no callback at level changing
    },
    0   // TIMER register initialization data
};

static const PWMConfig pwmConfig2 = {
    CSILLO_PWM_CLOCK,             // clock is 1Mhz
    CSILLO_PWM_INITIAL_PERIOD,    // period time is 10mSec
    pwmCallback,                // callback for every period
    {
        {PWM_OUTPUT_DISABLED, NULL},  // active 1 and no callback at level changing
        {PWM_OUTPUT_DISABLED, NULL},  // active 1 and no callback at level changing
        {PWM_OUTPUT_DISABLED, NULL},  // active 1 and no callback at level changing
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},  // active 1 and no callback at level changing
    },
    0   // TIMER register initialization data
};

static int channels[MAX_CHANNELS/2];

static WORKING_AREA(waThreadPWM, 128);

/**

*/
static msg_t pwmProcess(void *arg) {
    (void) arg;
    chRegSetThreadName("pwm");

    static int cnt=0;

    while(TRUE) {
        int i=0;

        for (;i<MAX_CHANNELS/2;i++)
            pwmEnableChannel(i<4 ? &PWMD1 : &PWMD4, i%4, channels[i]);

        pwmEnableChannel(&PWMD2, 3, channels[MAX_CHANNELS/2]);

        chThdSleepMilliseconds(1000);
    }
    return 0;
}

void pwmSetValue(int channel, int value) {
    if (ADDRESS==0 && channel < 9) {
        channels[channel] = value;
        return;
    }
    if (ADDRESS==1 && channel > 8)
        channels[channel-MAX_CHANNELS/2]=value;
}

/**
    pwm user interface
*/
void cmd_pwm(BaseSequentialStream *chp, int argc, char *argv[]) {
    if (argc!=2) {
        chprintf(chp, "usage: pwm <channel> <value>\r\n");
        return;
    }

    int channel = atoi(argv[0]);
    int value = atoi(argv[1]);
    if (channel <0 || channel > 8) {
        chprintf(chp, "channel must be 0 between 8\r\n");
        return;
    }

    if (value <0 || value > CSILLO_PWM_INITIAL_PERIOD) {
        chprintf(chp, "value must be 0 between and %d\r\n", CSILLO_PWM_INITIAL_PERIOD);
        return;
    }

    chprintf(chp, "PWM%d will be set to %d\r\n", channel, value);
    if (channel <8)
        pwmEnableChannel(channel<4 ? &PWMD1 : &PWMD4, channel%4, value);

    pwmEnableChannel(&PWMD2, 3, value);
}

void pwmCsilloInit(BaseSequentialStream *c)
{
  console = c;
  bzero(channels, sizeof(channels));

  palSetPadMode(CSILLO_PWM_PORT0, CSILLO_PWM_CHANNEL_0, PAL_MODE_ALTERNATE(1));
  palSetPadMode(CSILLO_PWM_PORT0, CSILLO_PWM_CHANNEL_1, PAL_MODE_ALTERNATE(1));
  palSetPadMode(CSILLO_PWM_PORT0, CSILLO_PWM_CHANNEL_2, PAL_MODE_ALTERNATE(1));
  palSetPadMode(CSILLO_PWM_PORT0, CSILLO_PWM_CHANNEL_3, PAL_MODE_ALTERNATE(1));
  palSetPadMode(CSILLO_PWM_PORT1, CSILLO_PWM_CHANNEL_4, PAL_MODE_ALTERNATE(2));
  palSetPadMode(CSILLO_PWM_PORT1, CSILLO_PWM_CHANNEL_5, PAL_MODE_ALTERNATE(2));
  palSetPadMode(CSILLO_PWM_PORT1, CSILLO_PWM_CHANNEL_6, PAL_MODE_ALTERNATE(2));
  palSetPadMode(CSILLO_PWM_PORT1, CSILLO_PWM_CHANNEL_7, PAL_MODE_ALTERNATE(2));
  palSetPadMode(CSILLO_PWM_PORT1, CSILLO_PWM_CHANNEL_8, PAL_MODE_ALTERNATE(1));


  pwmStart(&PWMD1, &pwmConfig0);
  pwmStart(&PWMD4, &pwmConfig1);
  pwmStart(&PWMD2, &pwmConfig2);

  chThdCreateStatic(waThreadPWM, sizeof(waThreadPWM), NORMALPRIO, pwmProcess, NULL);
}

