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
#include <stdlib.h>

#include "ch.h"
#include "hal.h"

#include "chprintf.h"

#include "misc.h"
#include "config.h"
#include "button.h"




/** \file misc.c
    \brief miscellaneous system commands
*/
static WORKING_AREA(PWM_WA, 128) ;

static msg_t PWMThread(void *arg)
{
    (void)arg ;
    chRegSetThreadName("PWMTHREAD") ;

    while(TRUE)
    {
        chThdSleepMilliseconds(50) ;
        pwmEnableChannel(&PWMD4, CH_D, ledduty) ;

    }
    return 0 ;
}

void startPWMThread(void)
{
    chThdCreateStatic(PWM_WA, sizeof(PWM_WA), NORMALPRIO, PWMThread, NULL) ;
}
