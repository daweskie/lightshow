#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED
#include "shell.h"

/*
 * Copyright (C)  2014 Zoltan Zidarics (Zamek)
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/** \file menu
    \brief menu
*/
/* Virtual serial port over USB.*/
static SerialUSBDriver SDU1;

void menuInit(void);

void cmd_menu(BaseSequentialStream *chp, int argc, char *argv[]);

void cmd_mem(BaseSequentialStream *chp, int argc, char *argv[]);

void cmd_threads(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_blinkspeed(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_way(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_blinkspeed_min(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_blinkspeed_now(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_blinkspeed_max(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_blinkspeed_min_ch(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_blinkspeed_max_ch(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_way_ch(BaseSequentialStream *chp, int argc, char *argv[]);

static const ShellCommand commands[] =
{

    {"mem", cmd_mem},
    {"threads", cmd_threads},
    {"blinkspeed", cmd_blinkspeed},
    {"bs", cmd_blinkspeed},
    {"dir", cmd_way},
    {"dirch", cmd_way_ch},
    {"actual", cmd_blinkspeed_now},
    {"bsmin", cmd_blinkspeed_min},
    {"bsmax", cmd_blinkspeed_max},
    {"bsminch", cmd_blinkspeed_min_ch},
    {"bsmaxch", cmd_blinkspeed_max_ch},
    {NULL, NULL}
};

static const ShellConfig shell_cfg1 =
{
    (BaseSequentialStream *)&SDU1,
    commands
};

#endif // MENU_H_INCLUDED
