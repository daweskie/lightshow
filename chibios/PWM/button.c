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

#include "button.h"
#include "config.h"

static WORKING_AREA(waThreadbutton, 128);

/**

*/
static msg_t Threadbutton(void *arg)
{
    (void) arg;
    chRegSetThreadName("button");
    int reg = 0;
    int btcnt;
    int updown = 0;
    while(TRUE)
    {

        chThdSleepMilliseconds(20);
        btcnt = 0;
        while (palReadPad(GPIOA, GPIOA_BUTTON))

        {
            chThdSleepMilliseconds(20);
            btcnt++;

            if (btcnt >= 40)
            {
                if (updown == 0)
                {
                    ledduty  = ledduty + 5;
                    if (ledduty > MAXDUTY_D)
                    {
                        ledduty = MAXDUTY_D;
                        updown = 1;
                    }
                }
                else if (updown == 1)
                {
                    ledduty = ledduty - 5;
                    if (ledduty < MINDUTY_D)
                    {
                        ledduty = MINDUTY_D;
                        updown = 0;
                    }

                }

            }


            if((btcnt > 10) && (btcnt < 40))
            {

                if(reg == 0)
                {
                    while (ledduty < MAXDUTY_D)
                    {
                        //chThdSleepMilliseconds(3);
                        ledduty++;

                    }
                    reg = 1;
                }
                else if ( reg == 1 )
                {
                    while (ledduty > MINDUTY_D)
                    {
                        //chThdSleepMilliseconds(3);
                        ledduty--;

                    }
                    reg = 0;
                }
            }
        }
        chThdSleepMilliseconds(50);
    }
    return 0;
}

/**
    button user interface
*/

void buttonInit(void)
{

    chThdCreateStatic(waThreadbutton, sizeof(waThreadbutton), NORMALPRIO, Threadbutton, NULL);
}
