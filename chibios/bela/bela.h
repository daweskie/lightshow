#ifndef BELA_H_INCLUDED
#define BELA_H_INCLUDED

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
/** \file bela.h
    \brief Common definitions for Bela projects
*/

#define BELA_FLASH_CONFIG_SECTOR FLASH_Sector_10

/**
    Number of channels for each device
*/
#define BELA_CHANNELS_NUM 8

/**
Max temperature of a channel
*/
#define BELA_MAX_TEMPERATURE 40

/**
Min temperature of a channel
*/
#define BELA_MIN_TEMPERATURE 0

/**
    Delta t value of fuzzy logic controller.
*/
#define BELA_FUZZY_DELTA_T 0.5f

/**
    Granularity of fuzzy logic controller.
*/
#define BELA_FUZZY_GRANULARITY 256

/**
    AD conversion for temperature controlling.

    Idea: There are 8 multiplexed channel outside of controller.
    The analog input is in BELA_ADC_PORT_BIT of BELA_ADC_PORT
    There 3 bit of channel selection in BELA_ADC_CHANNEL_MASK bits of BELA_ADC_CHANNEL_PORT

    The software have to select each channel and measure it.
*/
#undef BELA_ADC_DEBUGGING

#define BELA_ADC_RESOLUTION ((uint16_t)(1<<12))

#define BELA_ADC_MIN_TEMP -10.0  // Celsius

#define BELA_ADC_MAX_TEMP 40.0 // Celsius

#define BELA_ADC_INPUT ADC_CHANNEL_11  // input portbit of ad conversion

#define BELA_ADC_SAMPLES_COUNTS 32     // samples of each conversion

#define BELA_ADC_AVERAGE_COUNTS 10     // running average

#define BELA_ADC_PORT GPIOC            // adc input port

#define BELA_ADC_PORT_BIT 1            // adc input bit

#define BELA_ADC_CHANNEL_PORT GPIOD    // adc channel multiplexer

#define BELA_ADC_CHANNEL_MASK  7       // 0,1,2 bit

#define BELA_ADC_NORMALIZE_X0 ((uint16_t)0)

#define BELA_ADC_NORMALIZE_X1 ((uint16_t)((BELA_ADC_RESOLUTION)-1))

#define BELA_ADC_NORMALIZE_T0 BELA_ADC_MIN_TEMP

#define BELA_ADC_NORMALIZE_T1 BELA_ADC_MAX_TEMP

#define BELA_ADC_FLASH_CONFIG_PREFIX "ad"

/**
    PWM for temperature controlling
*/

#define BELA_PWM_DEBUGGING

#define BELA_PWM_TASK_SLEEP_MS  10        // 10

#define BELA_PWM_CLOCK 1000000           // 1Mhz

#define BELA_PWM_INITIAL_PERIOD 10000  // 1/100 Sec

#define BELA_PWM_DELTA_T 1.0f

#define BELA_PWM_PORT GPIOE

#define BELA_PWM_CHANNEL_0 GPIOE_PIN8

#define BELA_PWM_CHANNEL_1 GPIOE_PIN9

#define BELA_PWM_CHANNEL_2 GPIOE_PIN10

#define BELA_PWM_CHANNEL_3 GPIOE_PIN11

#define BELA_PWM_CHANNEL_4 GPIOE_PIN12

#define BELA_PWM_CHANNEL_5 GPIOE_PIN13

#define BELA_PWM_CHANNEL_6 GPIOE_PIN14

#define BELA_PWM_CHANNEL_7 GPIOE_PIN15

/**
    Communication
*/

#define BELA_COMM_BAUD 115200

#define BELA_COMM_MAX_PACKETS 4 // max packet in queue

#define BELA_MODBUS_ADDRESS 1 // modbus address of device

#define BELA_MODBUS_MAX_REGISTERS 16 // max number of registers

#endif // BELA_H_INCLUDED

