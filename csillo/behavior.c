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

#include "behavior.h"
#include "pwm_csillo.h"

static BaseSequentialStream *console;

static struct Channel channels[MAX_CHANNELS] = {
    {.id=0, .inputState=0, .lastState=1, .lastChange=0, .pwmValue=0,
        .neighbors={1,2,3,4}
    },
    { .id=1, .inputState=0, .lastState=1, .lastChange=0, .pwmValue=0,
        .neighbors={1,2,3,4}
    },
    { .id=2, .inputState=0, .lastState=1, .lastChange=0, .pwmValue=0,
        .neighbors={1,2,3,4}
    },
    { .id=3, .inputState=0, .lastState=1, .lastChange=0, .pwmValue=0,
        .neighbors={1,2,3,4}
    },
    { .id=4, .inputState=0, .lastState=1, .lastChange=0, .pwmValue=0,
        .neighbors={1,2,3,4}
    },
    { .id=5, .inputState=0, .lastState=1, .lastChange=0, .pwmValue=0,
        .neighbors={1,2,3,4}
    },
    { .id=6, .inputState=0, .lastState=1, .lastChange=0, .pwmValue=0,
        .neighbors={1,2,3,4}
    },
    { .id=7, .inputState=0, .lastState=1, .lastChange=0, .pwmValue=0,
        .neighbors={1,2,3,4}
    },
    { .id=8, .inputState=0, .lastState=1, .lastChange=0, .pwmValue=0,
        .neighbors={1,2,3,4}
    },
    { .id=9, .inputState=0, .lastState=1, .lastChange=0, .pwmValue=0,
        .neighbors={1,2,3,4}
    },
    { .id=10, .inputState=0, .lastState=1, .lastChange=0, .pwmValue=0,
        .neighbors={1,2,3,4}
    },
    { .id=11, .inputState=0, .lastState=1, .lastChange=0, .pwmValue=0,
        .neighbors={1,2,3,4}
    },
    { .id=12, .inputState=0, .lastState=1, .lastChange=0, .pwmValue=0,
        .neighbors={1,2,3,4}
    },
    { .id=13, .inputState=0, .lastState=1, .lastChange=0, .pwmValue=0,
        .neighbors={1,2,3,4}
    },
    { .id=14, .inputState=0, .lastState=1, .lastChange=0, .pwmValue=0,
        .neighbors={1,2,3,4}
    },
    { .id=15, .inputState=0, .lastState=1, .lastChange=0, .pwmValue=0,
        .neighbors={1,2,3,4}
    },
    { .id=16, .inputState=0, .lastState=1, .lastChange=0, .pwmValue=0,
        .neighbors={1,2,3,4}
    },
    { .id=17, .inputState=0, .lastState=1, .lastChange=0, .pwmValue=0,
        .neighbors={1,2,3,4}
    }
};

static struct {
    enum Behavior strategia;
} behaviors = { .strategia=male };

static WORKING_AREA(waThreadPWM, 128);

void readInput(void) {
    if (ADDRESS==0) {
        channels[0].inputState = palReadPad(PORT_INPUT_0, BIT_INPUT_0);
        if (channels[0].lastState == channels[0].inputState) {
            channels[0].lastChange++;
        }
        else {
            channels[0].lastChange = 0;
        }

        channels[1].inputState = palReadPad(PORT_INPUT_1, BIT_INPUT_1);
        channels[2].inputState = palReadPad(PORT_INPUT_2, BIT_INPUT_2);
        channels[3].inputState = palReadPad(PORT_INPUT_3, BIT_INPUT_3);
        channels[4].inputState = palReadPad(PORT_INPUT_4, BIT_INPUT_4);
        channels[5].inputState = palReadPad(PORT_INPUT_5, BIT_INPUT_5);
        channels[6].inputState = palReadPad(PORT_INPUT_6, BIT_INPUT_6);
        channels[7].inputState = palReadPad(PORT_INPUT_7, BIT_INPUT_7);
        channels[8].inputState = palReadPad(PORT_INPUT_8, BIT_INPUT_8);
        return;
    }
    channels[9].inputState =  palReadPad(PORT_INPUT_9,  BIT_INPUT_9);
    channels[10].inputState = palReadPad(PORT_INPUT_10, BIT_INPUT_10);
    channels[11].inputState = palReadPad(PORT_INPUT_11, BIT_INPUT_11);
    channels[12].inputState = palReadPad(PORT_INPUT_12, BIT_INPUT_12);
    channels[13].inputState = palReadPad(PORT_INPUT_13, BIT_INPUT_13);
    channels[14].inputState = palReadPad(PORT_INPUT_14, BIT_INPUT_14);
    channels[15].inputState = palReadPad(PORT_INPUT_15, BIT_INPUT_15);
    channels[16].inputState = palReadPad(PORT_INPUT_16, BIT_INPUT_16);
    channels[17].inputState = palReadPad(PORT_INPUT_17, BIT_INPUT_17);
}


void behaveAsFemale() {
}

void behaveAsMale() {
}

void behaveAsChild() {
}

void behaveAsSchizophrenic() {
}

void behaveAsDog() {
}

void behavior() {

    switch(behaviors.strategia) {
    case female : behaveAsFemale();
        return;
    case child : behaveAsChild();
        return;
    case schizophrenic : behaveAsSchizophrenic();
        return;
    default: behaveAsMale();
    }
}

void setPwmValues() {
    int i;
    for (i=0;i<MAX_CHANNELS;i++)
        pwmSetValue(i,channels[i].pwmValue);
}

/**

*/
static msg_t behaviorProcess(void *arg) {
    (void) arg;
    chRegSetThreadName("behavior");

    while(TRUE) {
       readInput();
       behavior();
       setPwmValues();
       chThdSleepMilliseconds(100);

    }
    return 0;
}

/**
    behavior user interface
*/
void cmd_behavior(BaseSequentialStream *chp, int argc, char *argv[]) {
    (void) argc;
    (void) argv;
    int i;
}

void initBehavior(BaseSequentialStream *c)
{
  console = c;
  chThdCreateStatic(waThreadPWM, sizeof(waThreadPWM), NORMALPRIO, behaviorProcess, NULL);
}
