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

#include "crc.h"

#define POLYNOMIAL 0xd8
#define POLYNOMIAL_MODBUS 0xa001

/**
    Calculate the crc value of a buffer
    \param data address of buffer
    \param d_len length of the buffer
    \return calculated crc
*/
crc16_t crc16CalcBuffer(char *data, size_t len) {
    crc16_t rmdr = 0;
    if (!data || len<=0)
        return 0;

    int i;
    for(i=0;i<len;i++)
        crc16AddByte(&rmdr, *(data+i));

    return rmdr;
}

/** on the fly crc calculation
    add a byte to the crc
    \param crc aggregated value of crc
    \param byte to aggregate to crc
    \return new crc value
*/
void crc16AddByte(crc16_t *crc, unsigned char b) {
    int bits;
    *crc ^= b;
    for (bits=8;bits >0;--bits)
        if (*crc & 0x8000)
            *crc = (*crc << 1) ^ POLYNOMIAL;
        else
            *crc <<= 1;
}

/** on the fly crc calculation for Modbus
    add a byte to the crc
    \param crc aggregated value of crc
    \param byte to aggregate to crc
    \return new crc value
*/
void crcModbusAddByte(crc16_t *crc, uint8_t b) {
    int i;
    *crc ^= b;
    for (i=8;i>0;i--)
        if (*crc & 1)
            *crc = (*crc >> 1) ^ POLYNOMIAL_MODBUS;
        else
            *crc >>= 1;
}

/**
    Crc begin for different methods
    \param method type of crc method
    \param address of crc value
*/
void crcBegin(crc_method_t method, crc16_t *crc) {
    if (!crc)
        return;
    switch(method) {
    case crc8:
    case crc16: *crc = 0;
        return;
    case crcModbus: *crc = (crc16_t) 0xffff;
        return;
    default :
        *crc = 0;
    }
}

void crcAddByte(crc_method_t method, crc16_t *crc, uint8_t value) {
    if (!crc)
        return;
    switch(method) {
    case crc8:
        return;
    case crc16 : crc16AddByte(crc, value);
        return;
    case crcModbus : crcModbusAddByte(crc, value);
        return;
    default:
        return;
    }
}

