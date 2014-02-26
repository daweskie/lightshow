#ifndef BEHAVIOR_H_INCLUDED
#define BEHAVIOR_H_INCLUDED

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
/** \file behavior
    \brief behavior
*/

#include "csillo.h"

enum Behavior {
    female, male, schizophrenic, child, dog
};

struct Channel {
    int id;
    int inputState; // 0=off 1=on
    int lastState;
    int lastChange;
    int pwmValue;
    int neighbors[MAX_NEIGHBOR];
};

void cmd_behavior(BaseSequentialStream *chp, int argc, char *argv[]);

void initBehavior(BaseSequentialStream *c);


#endif // BEHAVIOR_H_INCLUDED
