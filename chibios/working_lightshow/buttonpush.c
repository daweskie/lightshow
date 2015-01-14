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
#include <chsem.h>

//sem1_setup();

static WORKING_AREA(waThread2, 128);

static msg_t Thread2(void *arg)
{
    (void)arg;
    chRegSetThreadName("blinker1");
    int btcnt = 0;
    int updown = 0;

    while (TRUE)
    {
        chThdSleepMilliseconds(50);
        btcnt = 0;
        while (palReadPad(GPIOA, GPIOA_BUTTON))

        {
            chThdSleepMilliseconds(20);
            btcnt++;

            if (btcnt >= 40)
            {
                if (updown == 0)
                {
                    blinkspeed  = blinkspeed + 1;
                    if (blinkspeed > blinkmax)
                    {
                        blinkspeed = blinkmax;
                        updown = 1;
                    }
                }
                else if (updown == 1)
                {
                    blinkspeed = blinkspeed - 1;
                    if (blinkspeed < blinkmin)
                    {
                        blinkspeed = blinkmin;
                        updown = 0;
                    }

                }

            }

        }
        if((btcnt > 3) && (btcnt < 40))
        {
            presses = 1;
        }
        if (presses == 1)
            chBSemWait(&my_bsem);
    }
    return 0;
}
void startButton(void)
{
    chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO, Thread2, NULL);
}
