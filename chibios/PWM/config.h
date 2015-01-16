#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#define LEDDUTY_D 500
#define MAXDUTY_D 1000
#define MINDUTY_D 0
#define DELAY_1 200
#define DELAY_2 3
#define DELAY_3 10
#define THRESHOLD 300
#define SPEED 3

#define LED_PORT GPIOD
#define LED_PIN GPIOD_LED5

#define PWM_CLK_FREQUENCY 100000 /* Clock Frequency of PWM Timer. Higher Frequency better resolution. 1000000 = 1MHz */
#define PWM_PERIOD 1000 /* Time Period of PWM Cycle. 20000 = 20 ms with PWM_Clock_Frequency = 1 MHz */

//*CH --> Channel of the LED

#define CH_D 2
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

/* Virtual serial port over USB.*/

extern int ledduty;
extern int minduty;
extern int maxduty;
extern int timebase;


void initPWM(void) ;

#endif // CONFIG_H_INCLUDED
