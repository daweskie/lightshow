#ifndef TASK1_H_INCLUDED
#define TASK1_H_INCLUDED

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
/** \file task1
    \brief task1
*/



#define TASK1_CMD "ac" //parancsok, itt lehet ujakat is hozzáadni itt kell boviteni
#define TASK1_CMD_A "aca"

#define task1_commands \
    {TASK1_CMD, cmd_task1}, \
    {TASK1_CMD, cmd_task1}

#define TASK_STACK_SIZE 128

typedef struct AcData (
     /** */
    int32_t xAvg, yAvg;
     /** */
    int32_t xMax, yMax;
     /** */
    int32_t xMin, yMin;
);



/** getting accelerator data
    \param data ointer to store values. Nullsafe
    \return 0  return successfully
            -1 if data is null
            -2 lock error

*/
int task1GetData(AcData* data);


void task1Init(void);

void cmd_task1(BaseSequentialStream *chp, int argc, char *argv[]); //ebbe irkálhatóak a parancsok
oid cmd_task1_a(BaseSequentialStream *chp, int argc, char *argv[]);

#endif // TASK1_H_INCLUDED
