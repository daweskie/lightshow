#ifndef SHELLMENU_H_INCLUDED
#define SHELLMENU_H_INCLUDED

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
/** \file shellmenu
    \brief shellmenu
*/

void cmd_actual(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_timebase(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_timebase_w(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_actual_v(BaseSequentialStream *chp, int argc, char *argv[]);
/* Virtual serial port over USB.*/
static SerialUSBDriver SDU1;

static const ShellCommand commands[] =
{

    {"aduty",cmd_actual},
    {"tbase",cmd_timebase},
    {"wtbase",cmd_timebase_w},
    {"wduty",cmd_actual_v},
    {NULL, NULL}
};

static const ShellConfig shell_cfg1 =
{
    (BaseSequentialStream *)&SDU1,
    commands
};

void shellmenuInit(void);

#endif // SHELLMENU_H_INCLUDED
