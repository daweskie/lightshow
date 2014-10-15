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
#include <stdint.h>
#include <string.h>
#include <bela.h>
#include <model.h>
#include <modbus.h>

typedef struct {
    float k;        // (t1-t0)/(x1-x0)
    uint16_t x0;
    uint16_t x1;
    float t0;
    float t1;
    float needTemperature;
    float normalizedTemperature;
} ChannelParam;

typedef struct {
    modbus_float_register_t needTemperature;
    modbus_float_register_t temperature;
} ModbusParams;


struct BelaModel {
    ChannelParam channelsParam[BELA_CHANNELS_NUM];
    uint16_t averages[BELA_CHANNELS_NUM][BELA_ADC_AVERAGE_COUNTS];
    uint16_t averageIndexes[BELA_CHANNELS_NUM];
    ModbusParams modbusParams[BELA_CHANNELS_NUM];
};

static struct BelaModel belaModel;


void initModel(void) {
    bzero(&belaModel, sizeof(struct BelaModel));
}

void setAdValue(uint8_t channel, uint16_t value) {

    if (channel > BELA_CHANNELS_NUM)
        return;
    uint16_t idx=belaModel.averageIndexes[channel];
    belaModel.averages[channel][idx++] = value;
    belaModel.averageIndexes[channel] = idx % BELA_CHANNELS_NUM;

    uint32_t sum = 0;
    uint8_t i;
    for (i=0;i<BELA_ADC_AVERAGE_COUNTS;i++)
        sum+=belaModel.averages[channel][i];

    sum /= BELA_CHANNELS_NUM;

    ChannelParam *cp = &belaModel.channelsParam[channel];
    cp->normalizedTemperature = cp->k * (sum-cp->x0) + cp->t0;
}

float getChannelValue(uint8_t channel) {
    return channel < BELA_CHANNELS_NUM
            ? belaModel.channelsParam[channel].normalizedTemperature
            : 0;
}

void setChannelNeedTemperature(uint8_t channel, float value) {
    if (channel < BELA_CHANNELS_NUM && value >= BELA_ADC_MIN_TEMP && value <= BELA_ADC_MAX_TEMP)
        belaModel.channelsParam[channel].needTemperature = value;
}
