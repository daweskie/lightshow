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

#include "menu.h"
#include "config.h"


/**
    menu user interface
*/
void cmd_menu(BaseSequentialStream *chp, int argc, char *argv[])
{
    (void) argc;
    (void) argv;
    int i;
}

void cmd_mem(BaseSequentialStream *chp, int argc, char *argv[])
{
    size_t n, size;

    (void)argv;
    if (argc > 0)
    {
        chprintf(chp, "Usage: mem\r\n");
        return;
    }
    n = chHeapStatus(NULL, &size);
    chprintf(chp, "core free memory : %u bytes\r\n", chCoreStatus());
    chprintf(chp, "heap fragments    : %u\r\n", n);
    chprintf(chp, "heap free total  : %u bytes\r\n", size);
}

void cmd_threads(BaseSequentialStream *chp, int argc, char *argv[])
{
    static const char *states[] = {THD_STATE_NAMES};
    Thread *tp;

    (void)argv;
    if (argc > 0)
    {
        chprintf(chp, "Usage: threads\r\n");
        return;
    }
    chprintf(chp, "%10s %10s %6s %6s %11s %7s %17s \r\n",
             "add", "stack", "prio", "refs", "state", "time", "name");
    tp = chRegFirstThread();
    do
    {
        chprintf(chp, "%.10lx %.10lx %6lu %6lu %11s %7lu %17.15s \r\n",
                 (uint32_t)tp, (uint32_t)tp->p_ctx.r13,
                 (uint32_t)tp->p_prio, (uint32_t)(tp->p_refs - 1),
                 states[tp->p_state], (uint32_t)tp->p_time,
                 (uint32_t)tp->p_name);
        tp = chRegNextThread(tp);
    }
    while (tp != NULL);
}


void cmd_blinkspeed(BaseSequentialStream *chp, int argc, char *argv[])
{

    (void)argv;
    int speed = 30;
    if (argc != 1)
    {

        return;
    }
    speed = atoi(argv[0]);
    if(speed > 5000)
        speed = 5000;
    if(speed < 5)
        speed = 5;
    blinkspeed = speed;
}
void cmd_blinkspeed_min_ch(BaseSequentialStream *chp, int argc, char *argv[])
{

    (void)argv;
    int speed = 30;
    if (argc != 1)
    {

        return;
    }
    speed = atoi(argv[0]);
    if(speed > 5000)
        speed = 5000;
    if(speed < 5)
        speed = 5;
    blinkmin = speed;
}
void cmd_blinkspeed_max_ch(BaseSequentialStream *chp, int argc, char *argv[])
{

    (void)argv;
    int speed = 30;
    if (argc != 1)
    {

        return;
    }
    speed = atoi(argv[0]);
    if(speed > 5000)
        speed = 5000;
    if(speed < 5)
        speed = 5;
    blinkmax = speed;
}
void cmd_blinkspeed_now(BaseSequentialStream *chp, int argc, char *argv[])
{

    (void)argv;
    if (argc != 1)
    {

        chprintf(chp, "Current speed : %d [ms]\r\n", blinkspeed);
        return;
    }

}

void cmd_blinkspeed_min(BaseSequentialStream *chp, int argc, char *argv[])
{

    (void)argv;
    if (argc != 1)
    {

        chprintf(chp, "Minimum blink speed is %d [ms]\r\n", blinkmin);
        return;
    }

}

void cmd_blinkspeed_max(BaseSequentialStream *chp, int argc, char *argv[])
{

    (void)argv;
    if (argc != 1)
    {

        chprintf(chp, "Maximum blink speed is: %d [ms]\r\n", blinkmax);
        return;
    }

}

void cmd_way(BaseSequentialStream *chp, int argc, char *argv[])
{

    (void)argv;
    if (argc != 1)
    {
        if (dir == 1)
        {
            chprintf(chp, "The direction is clockwise.\r\n");
        }
        else if (dir == 0)
        {
            chprintf(chp, "The direction is counter-clockwise.\r\n");
        }

        return;
    }

}

void cmd_way_ch(BaseSequentialStream *chp, int argc, char *argv[])
{

    (void)argv;
    int speed = 30;

    if (argc != 1)
    {
        chprintf(chp, "Usage: 0 = clockwise and 0 = counter-clockwise.\r\n");
        return;

    }
    speed = atoi(argv[0]);
    if(speed > 1)
        speed = 1;
    if(speed < 0)
        speed = 0;
    dir = speed;
}


