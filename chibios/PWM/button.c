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
    int btcnt;
    int updown = 0;

    int K = 0;

    while(TRUE)
    {

        chThdSleepMilliseconds(20);
        btcnt = 0;



        if(palReadPad(GPIOA, GPIOA_BUTTON))
        {
            chThdSleepMilliseconds(DELAY_1);    //Delay1

            while(K==0)
            {
                btcnt++;
                chThdSleepMilliseconds(DELAY_2);          //Delay2

                if(btcnt>THRESHOLD&&updown==0)            //Threshold
                {
                    ledduty=ledduty+SPEED;
                    chThdSleepMilliseconds(timebase);      //Delay3
                    if(ledduty==MAXDUTY_D)
                        updown=1;
                }

                if(btcnt>THRESHOLD&&updown==1)            //Threshold
                {
                    ledduty=ledduty-SPEED;
                    chThdSleepMilliseconds(timebase);      //Delay3
                    if(ledduty==MINDUTY_D)
                        updown=0;
                }

                if((palReadPad(GPIOA, GPIOA_BUTTON)==0))
                {
                    K=1;
                }
            }
            K=0;

            if(btcnt<THRESHOLD && updown==1)              //Threshold
            {
                ledduty = MINDUTY_D;
                updown = 0;
            }

            else if(btcnt<THRESHOLD  && updown==0)       //Threshold
            {
                ledduty = MAXDUTY_D;
                updown = 1;
            }
            btcnt=0;


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
