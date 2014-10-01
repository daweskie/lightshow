/*
 * Copyright (C)  2011 Zoltan Zidarics (Zamek)
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
#include <ch.h>
#include <chstreams.h>
#include <chprintf.h>

void hexDump(BaseSequentialStream *chp, uint8_t *address, int len) {
    int i=0;
    for (;i<len;i++)
        chprintf(chp, "%-2x ", *(address+i));
    chprintf(chp, "\r\n");
}

void cmd_dump(BaseSequentialStream *chp, int argc, char *argv[]) {
    if (argc==0) {
        chprintf(chp,"usage: dump <0x<mem>> [len] (default len is 16)\r\n");
        return;
    }
    char *endptr;
    uint8_t *address = (uint8_t *) strtol(argv[0],&endptr,16);
    int len = argc == 2 ? atoi(argv[1]) : 16;
    hexDump(chp, address, len);
}
