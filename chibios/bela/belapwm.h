#ifndef PWM_H_INCLUDED
#define PWM_H_INCLUDED

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
/** \file pwm.h
    \brief PWM for temperature controlling
*/

void pwmInitilaize (void);
/**
    PWM user interface
*/
void cmd_pwm(BaseSequentialStream *chp, int argc, char *argv[]);

#endif // PWM_H_INCLUDED
