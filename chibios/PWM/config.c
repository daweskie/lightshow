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
#include "ch.h"
#include "hal.h"
#include <chprintf.h>

#include "config.h"

int maxduty = MAXDUTY_D;
int minduty = MINDUTY_D;
int ledduty = LEDDUTY_D;
int timebase = DELAY_3;


static void pwmpcb(PWMDriver *pwmp)
{

    (void)pwmp;

    /* Starts an asynchronous ADC conversion operation, the conversion
       will be executed in parallel to the current PWM cycle and will
       terminate before the next PWM cycle.*/

}
/*
*PWM configuration structures.
*/
/*pwmcfg1 - to be used for PWM1. */
static PWMConfig pwmcfg1 =
{
    PWM_CLK_FREQUENCY,
    PWM_PERIOD,
    pwmpcb,
    { /* CH1 of Timer4 for is available for PWM. Enable it and disable the rest */
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},
        {PWM_OUTPUT_ACTIVE_HIGH, NULL}
    },
    /*HW Dependent Part */
    0,

};

/*
* PWM cyclic callback.
*/


void initPWM(void)
{
    /*
    *Start PWM1 function associated with Timer 4.
    */
    pwmStart(&PWMD4, &pwmcfg1) ;
    palSetPadMode(LED_PORT, LED_PIN, PAL_MODE_ALTERNATE(2)) ;
    chThdSleepMilliseconds(10) ;
}
