#ifndef PWM_H_INCLUDED
#define PWM_H_INCLUDED

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
/** \file pwm
    \brief pwm
*/

#define CSILLO_SET_PWM "pwm"

#define _csillo_commands \
        {CSILLO_SET_PWM, cmd_pwm}

void pwmSetValue(int channel, int value);

void pwmCsilloInit(BaseSequentialStream *console);

void cmd_pwm(BaseSequentialStream *chp, int argc, char *argv[]);


#endif // PWM_H_INCLUDED
