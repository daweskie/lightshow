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
#include <ch.h>
#include <hal.h>
#include <chprintf.h>

#include "config.h"
//#include "menu.h"

/**
Configurable variables of the program
*/

blinkspeed = 100;
blinkmin = 30;
blinkmax = 300;
dir = 0;
led1 = GPIOD_LED3;
led2 = GPIOD_LED5;
led3 = GPIOD_LED6;
led4 = GPIOD_LED4;
//bt1 = "GPIOA_BUTTON";
//bt1ch = "GPIOA";
presses = 0;



chBSemInit(&my_bsem, FALSE);


/**
static THD_WORKING_AREA(waThreadconfig, 128);


static THD_FUNCTION(Threadconfig, arg) {
    (void) arg;
    chRegSetThreadName("config");

    while(TRUE) {
       chThdSleepMilliseconds(50);
    }
    return 0;
}
*/

/**
    config user interface
*/
void cmd_config(BaseSequentialStream *chp, int argc, char *argv[]) {
    (void) argc;
    (void) argv;
    int i;
}
/**
void configInit(void)
{

  chThdCreateStatic(waThreadconfig, sizeof(waThreadconfig), NORMALPRIO, Threadconfig, NULL);
}
*/
