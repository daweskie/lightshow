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


/**
Configurable variables of the program
*/

blinkspeed = blinkspeed_d;
blinkmin = blinkmin_d;
blinkmax = blinkmax_d;
dir = dir_d;
led1 = led1_d;
led2 = led2_d;
led3 = led3_d;
led4 = led4_d;

presses = 0;

BinarySemaphore my_bsem;


void cmd_config(BaseSequentialStream *chp, int argc, char *argv[])
{
    (void) argc;
    (void) argv;
    int i;
}

