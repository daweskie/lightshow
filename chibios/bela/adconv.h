#ifndef ADCONV_H_INCLUDED
#define ADCONV_H_INCLUDED

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
/** \file adc.h
    \brief AD converter constants
*/

void adConvInit(void);
void cmd_ad(BaseSequentialStream *chp, int argc, char *argv[]);
void adStop(void);
int adIsRunning(void);
float adGetChannelValue(int ch);
void cmd_showChannelParams(BaseSequentialStream *chp, int argc, char *argv[]);

#endif // ADC_H_INCLUDED
