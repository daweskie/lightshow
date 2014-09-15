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

#include "#!#TASK_NAME#!#.h"

static THD_WORKING_AREA(waThreadPWM, 128);

/**

*/
static msg_t #!#TASK_NAME#!#Process(void *arg) {
    (void) arg;
    chRegSetThreadName("#!#TASK_NAME#!#");

    while(TRUE) {
       chThdSleepMilliseconds(?);
    }
    return 0;
}

/**
    #!#TASK_NAME#!# user interface
*/
void cmd_#!#TASK_NAME#!#(BaseSequentialStream *chp, int argc, char *argv[]) {
    (void) argc;
    (void) argv;
    int i;
}
void #!#TASK_NAME#!#Init(void)
{

  chThdCreateStatic(waThreadPWM, sizeof(waThreadPWM), NORMALPRIO, #!#TASK_NAME#!#Process, NULL);
}
