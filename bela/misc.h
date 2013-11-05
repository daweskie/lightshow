#ifndef MISC_H_INCLUDED
#define MISC_H_INCLUDED

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
/** \file misc.h
    \brief miscellaneous system commands
*/

void cmd_mem(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_threads(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_blinkspeed(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_dump(BaseSequentialStream *chp, int argc, char *argv[]);
void miscInit(void);

#endif // MISC_H_INCLUDED
