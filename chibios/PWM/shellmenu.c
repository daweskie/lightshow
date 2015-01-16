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

#include "shellmenu.h"
#include "config.h"

void cmd_actual(BaseSequentialStream *chp, int argc, char *argv[])
{

    (void)argv;
    if (argc != 1)
    {

        chprintf(chp, "Current brigthness of the led is : %d percentage \r\n", (ledduty/10));
        return;
    }

}

void cmd_timebase(BaseSequentialStream *chp, int argc, char *argv[])
{

    (void)argv;
    if (argc != 1)
    {

        chprintf(chp, "Current time base : %d ms \r\n", timebase);
        return;
    }

}

void cmd_actual_v(BaseSequentialStream *chp, int argc, char *argv[])
{

    (void)argv;
    int brightness = LEDDUTY_D;

    if (argc != 1)
    {
        chprintf(chp, "Set the value between 10-1000 to get 1-100% brightness\r\n");
        return;

    }
    brightness = atoi(argv[0]);
    if(brightness > MAXDUTY_D)
        brightness = MAXDUTY_D;
    if(brightness < MINDUTY_D)
        brightness = MINDUTY_D;
    ledduty = brightness;
}

void cmd_timebase_w(BaseSequentialStream *chp, int argc, char *argv[])
{

    (void)argv;
    int tbase = DELAY_3;

    if (argc != 1)
    {
        chprintf(chp, "Usage: min =1 and max = 20, min will run the cycle around 0.6sec and max is around 6.6sec . \r\n");
        return;

    }
    tbase = atoi(argv[0]);
    if(tbase > 20)
        tbase = 20;
    if(tbase < 1)
        tbase = 1;
    timebase = tbase;
}

