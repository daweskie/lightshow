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

#include <stdlib.h>
#include <stdint.h>

#include "ch.h"
#include "hal.h"

#include "motorolapacket.h"
#include "crc.h"

#define MOTOROLA_PREAMBLE 0x55

static void motRxchar(void *instance, uint8_t c){
    MotorolaPacketDriver *driver = (MotorolaPacketDriver *)instance;
    if (!driver)
        return;

    switch (driver->state) {
    case psNone : if (c==MOTOROLA_PREAMBLE)
        driver->state = ps55;
        return;

    case ps55 : driver->state = (c & 0xff)==0xff ? ps1 : ps55;
        return;

    case ps1 : driver->state = c== 1 ? psAddress : psNone;
        return;

    case psAddress :
        if (spBeginPacket(instance, (uint8_t) c)) {
            driver->state = psCmd;
            return;
        }
        break;

    case psCmd :
        if (driver->current) {
            driver->current->command = c;
            crcAddByte(&(driver->flyingCrc), c);
            driver->state = psDlenLo;
            return;
        }
        break;

    case psDlenLo :
        if (driver->current) {
            driver->current->len = c & 0xff;
            crcAddByte(&(driver->flyingCrc), c);
            driver->state = psDlenHi;
            return;
        }
        break;

    case psDlenHi :
        if (driver->current) {
            crcAddByte(&(driver->flyingCrc), c);
            driver->current->len |= (c&0xff)<<8;
            if (driver->current->len > driver->mtu)
                break;
            driver->state = driver->current->len > 0 ? psData : psCrcLo;
            driver->dIdx = 0;
            return;
        }
        break;

        case psData :
            if (driver->current && driver->current->len > 0) {
                crcAddByte(&(driver->flyingCrc), c);
                driver->current->data[driver->dIdx++] = c;
                if (driver->dIdx >= driver->current->len)
                    driver->state = psCrcLo;

                return;
            }
            break;

        case psCrcLo :
            if (driver->current) {
                driver->receivedCrc = c & 0xff;
                driver->state = psCrcHi;
                return;
            }
            break;

        case psCrcHi :
            if (driver->current) {
                driver->receivedCrc |= (c&0xff) << 8;
                if (crcCheck(driver->receivedCrc, driver->flyingCrc)) {
                    spAddValidPacket(instance);
                    return;
                }
                break;
            }

        default : break;
    }

    ++driver->errorPacketsCounts;
    driver->state = psNone;
}

int motSend(MotorolaPacketDriver *driver, uint8_t address, uint8_t command, uint16_t dataLength, uint8_t *data) {
    #define sc(crc,ch) \
        do { \
          crcAddByte(&((crc)),(ch)); \
          spSendChar(driver, (ch)); \
        } while(0)

    if(! (address && command && dataLength>0 && (data || (!dataLength && !data))))
        return EXIT_FAILURE;

    char c=MOTOROLA_PREAMBLE;

    int i=0;
    for (;i<driver->preambleCounts;i++)
        spSendChar(driver, c);

    c=0xff;
    spSendChar(driver, c);
    c=1;
    spSendChar(driver, c);

    crc_t crc = 0;
    sc(crc,address);
    sc(crc,command);
    sc(crc,dataLength&0xff);
    sc(crc,(dataLength>>8)&0xff);
    if (dataLength)
        for(;dataLength>0;dataLength--,data++)
            sc(crc,*data);
    spSendChar(driver, crc&0xff);
    spSendChar(driver, (crc>>8)&0xff);
    return EXIT_SUCCESS;
}

const struct MotorolaPacketDeviceVMT motorolaPacketDeviceVMT = {
    .spGetElementFromPool=spGetElementFromPool,
    .spBeginPacket=spBeginPacket,
    .spAddValidPacket=spAddValidPacket,
    .spIsEmpty=spIsEmpty,
    .spGetNextPacket=spGetNextPacket,
    .spRecyclePacket=spRecyclePacket,
    .spGetAllPacketCounts=spGetAllPacketCounts,
    .spGetErrorCounts=spGetErrorCounts,
    .spGetPacketOverrunCounts=spGetPacketOverrunCounts,
    .spRxChar=motRxchar,
    .motSend=motSend
};

void motorolaObjectInit(MotorolaPacketDriver *driver, char *name, SerialDriver *serialDriver,
                        uint8_t maxPackets, uint16_t mtu, uint8_t preambleCounts){

     if (! driver)
        return;
    spObjectInit(driver, name, serialDriver, maxPackets, mtu);
    driver->vmt = &motorolaPacketDeviceVMT;
    driver->state = psNone;
    driver->preambleCounts = preambleCounts;
}

