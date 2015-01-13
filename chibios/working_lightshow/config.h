#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED
#include <chsem.h>

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
/** \file config
    \brief config
*/



extern int blinkspeed;
extern int blinkmin;
extern int blinkmax;
extern int dir;
extern int led1;
extern int led2;
extern int led3;
extern int led4;
extern char *bt1;
extern char *bt1ch;
extern int presses;
extern BinarySemaphore my_bsem;


void configInit(void);

void cmd_config(BaseSequentialStream *chp, int argc, char *argv[]);

#endif // CONFIG_H_INCLUDED
