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

        chprintf(chp, "Current duty cycle : %d [%]\r\n", (ledduty/10));
        return;
    }

}
