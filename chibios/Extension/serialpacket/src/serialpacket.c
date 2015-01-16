/*
 * Copyright (C)  2011 Zoltan Zidarics (Zamek)
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY{} without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/** \file serialpacket.c
    \brief Abstract class of serial packet administration.
    This module can controlling packets storing and recycling for the child classes for example Modbus or Motorola protocols.
*/

#include <stdlib.h>
#include <stdint.h>
#include <sys/queue.h>
#include <ch.h>
#include <hal.h>
#include <chthreads.h>
#include <chmemcore.h>
#include <string.h>
#include <cmdutil.h>
#include <macros.h>
#include <serialpacket.h>
#include <checksum.h>
#include <chprintf.h>

#if defined(HAL_USE_SERIAL_PACKETS) || defined(__DOXYGEN__)

#define SERIAL_PACKET_WA_SIZE THD_WA_SIZE(1024)

struct SPDriverListItem {
    SerialPacketDriver *driver;
    SLIST_ENTRY(SPDriverListItem) next;
};


/**
    List of existing serialpacket driver
    User can get a list about the existing driver with
    spstat shell command.
*/
static struct {
    SLIST_HEAD(SPList, SPDriverListItem) spItems;
}
sPLists = {
    {NULL}
};

/**
    Thread for processing incoming chars and incoming packets
*/
msg_t spThread(void *args) {

    SerialPacketDriver *drv = (SerialPacketDriver *) args;
    if (! (drv && drv->vmt->spRxChar) )
        return 0;

    while(1) {
#if !SERIAL_PACKETS_POSIX_TEST
        if (drv->vmt->spRxChar && !sdGetWouldBlock(drv->serialDriver))
            drv->vmt->spRxChar(drv, sdGet(drv->serialDriver));
#endif
        if (drv->vmt->spProcessPackets)
            drv->vmt->spProcessPackets(drv);
        chThdSleepMilliseconds(1);
    }
    return 0;
}

void spStartReceive(void *instance) {
    SerialPacketDriver *drv = (SerialPacketDriver *) instance;
    if (!drv)
        return;

#if !SERIAL_PACKETS_POSIX_TEST
    sdStart(drv->serialDriver, drv->serialConfig);
#endif

    chThdResume(drv->thread);
}

void spStopReceive(void *instance) {
    SerialPacketDriver *drv = (SerialPacketDriver *) instance;
    if (!drv)
        return;

#if !SERIAL_PACKETS_POSIX_TEST
    sdStop(drv->serialDriver);
#endif

    chThdSleep(TIME_INFINITE);
}

void spChecksumBegin(checksum_method_t method, crc16_t *crc){
    checksumBegin(method, crc);
}

void spChecksumAddByte(checksum_method_t method, crc16_t *crc, uint8_t b) {
    checksumAddByte(method, crc, b);
}

void spStart(void *instance, SerialConfig *config) {
    SerialPacketDriver *drv = (SerialPacketDriver *) instance;

#if !SERIAL_PACKETS_POSIX_TEST
    if (drv && drv->serialDriver && config) {
        drv->serialConfig = config;
        sdStart(drv->serialDriver, drv->serialConfig);
#else
    if (drv) {
#endif
        drv->serialConfig = config;
        sdStart(drv->serialDriver, drv->serialConfig);
        if (!drv->thread)
            drv->thread=chThdCreateFromHeap(NULL, SERIAL_PACKET_WA_SIZE, chThdGetPriority(), spThread, drv);
        else
            chThdResume(drv->thread);
    }
}

struct SerialPacketItem *spGetElementFromPool(void *ip){
    SerialPacketDriver *drv = (SerialPacketDriver *) ip;
    if (!drv || SLIST_EMPTY(&(drv->packetPool)))
            return NULL;

    struct SerialPacketItem *item = SLIST_FIRST(&(drv->packetPool));
    SLIST_REMOVE_HEAD(&(drv->packetPool), next);
    SLIST_NEXT(item, next) = NULL;
    return item;
}

int spPoolCounts(void *ip) {
    SerialPacketDriver *drv = (SerialPacketDriver *) ip;
    if (!drv || SLIST_EMPTY(&(drv->packetPool)))
            return 0;
    int cnts=0;
    if (SLIST_EMPTY(&(drv->packetPool)))
        return 0;
    struct SerialPacketItem *item = NULL;
    SLIST_FOREACH(item, &(drv->packetPool), next)
        cnts++;

    return cnts;
}

int spPacketCounts(void *ip) {
    SerialPacketDriver *drv = (SerialPacketDriver *) ip;
    if (!drv || SLIST_EMPTY(&(drv->packets)))
            return 0;
    int cnts=0;
    if (SLIST_EMPTY(&(drv->packets)))
        return 0;
    struct SerialPacketItem *item = NULL;
    SLIST_FOREACH(item, &(drv->packets), next)
        cnts++;

    return cnts;
}

int spBeginPacket(void *ip, uint8_t address){
    SerialPacketDriver *drv = (SerialPacketDriver *) ip;
    if (!drv)
        return 0;

   ++drv->packetsCounts;
   crcModbusBegin(drv->flyingCrc);
   crcModbusAddByte(&(drv->flyingCrc), address);
   if (!drv->current) {
        drv->current = spGetElementFromPool(ip);
        if (!drv->current) {
            ++drv->packetOverrunsCounts;
            return 0;
        }
   }
   drv->current->address = address;
   drv->current->valid = 0;
   return 1;
}

int spSendChar(void *ip, uint8_t ch) {
    SerialPacketDriver *drv = (SerialPacketDriver *) ip;
    if (!drv)
        return EXIT_FAILURE;
#if SERIAL_PACKETS_POSIX_TEST
    drv->vmt->spRxChar(drv, ch);
    return 1;
#else
    return sdPut(drv->serialDriver, ch);
#endif
}

int spSendBuffer(void *ip, uint16_t len, uint8_t *buffer) {
    SerialPacketDriver *drv = (SerialPacketDriver *) ip;
    if (! (drv && len>0 && buffer) )
        return 0;
#if SERIAL_PACKETS_POSIX_TEST
    int i=0;
    for (;i<len;i++)
        drv->vmt->spRxChar(drv, *(buffer+i));

    return len;
#else
    return sdWrite(drv->serialDriver, buffer, len);
#endif
}

void spAddValidPacket(void *ip) {
    SerialPacketDriver *drv = (SerialPacketDriver *) ip;
    if (drv->current) {
        drv->current->valid = 1;
        if (SLIST_EMPTY(&(drv->packets)))
            SLIST_INSERT_HEAD(&(drv->packets), drv->current, next);
        else {
            struct SerialPacketItem *item;
            struct SerialPacketItem *last=NULL;
            SLIST_FOREACH(item, &(drv->packets), next)
                last=item;

            SLIST_INSERT_AFTER(last, drv->current, next);
        }
        drv->current = NULL;
        drv->dIdx = 0;
        drv->flyingCrc = 0;
        drv->receivedCrc = 0;
    }
}

int spIsEmpty(void *ip){
    SerialPacketDriver *drv = (SerialPacketDriver *) ip;
    return SLIST_EMPTY(&(drv->packets));
}

struct SerialPacketItem *spGetNextPacket(void *ip){
    SerialPacketDriver *drv = (SerialPacketDriver *) ip;
    if (!drv || SLIST_EMPTY(&(drv->packets)))
        return NULL;

    struct SerialPacketItem *result = SLIST_FIRST(&(drv->packets));

    SLIST_REMOVE_HEAD(&(drv->packets),next);
    return result;
}

int spRecyclePacket(void *ip, struct SerialPacketItem *item){
    SerialPacketDriver *drv = (SerialPacketDriver *) ip;
    if (! (drv && item))
        return EXIT_FAILURE;

    item->valid = 0;
    SLIST_INSERT_HEAD(&(drv->packetPool), item, next);
    return EXIT_SUCCESS;
}

uint32_t spGetAllPacketCounts(void *ip){
    SerialPacketDriver *drv = (SerialPacketDriver *) ip;
    return drv ? drv->packetsCounts:0;
}

uint32_t spGetErrorCounts(void *ip){
    SerialPacketDriver *drv = (SerialPacketDriver *) ip;
    return drv ? drv->errorPacketsCounts : 0;
}

uint32_t spGetPacketOverrunCounts(void *ip){
    SerialPacketDriver *drv = (SerialPacketDriver *) ip;
    return drv ? drv->packetOverrunsCounts : 0;
}

const struct SerialPacketDeviceVMT serialPacketDeviceVMT = {
    .spGetElementFromPool=spGetElementFromPool,
    .spBeginPacket=spBeginPacket,
    .spSendChar=spSendChar,
    .spAddValidPacket=spAddValidPacket,
    .spIsEmpty=spIsEmpty,
    .spGetNextPacket=spGetNextPacket,
    .spRecyclePacket=spRecyclePacket,
    .spGetAllPacketCounts=spGetAllPacketCounts,
    .spGetErrorCounts=spGetErrorCounts,
    .spGetPacketOverrunCounts=spGetPacketOverrunCounts,
    .spStartReceive=spStartReceive,
    .spStopReceive=spStopReceive,
    .spChecksumBegin=spChecksumBegin,
    .spChecksumAddByte=spChecksumAddByte,
};

void spObjectInit(void *instance, char *name, SerialDriver *serialDriver,
                  uint8_t maxPackets, uint16_t mtu) {
    if (!instance)
        return;
    SerialPacketDriver *drv = (SerialPacketDriver *) instance;
    drv->name=name;

    struct SPDriverListItem *it = chCoreAlloc(sizeof(struct SPDriverListItem));
    bzero(it, sizeof(struct SPDriverListItem));
    it->driver = drv;
    SLIST_INSERT_HEAD(&(sPLists.spItems), it, next);

    drv->vmt = &serialPacketDeviceVMT;
    SLIST_INIT(&(drv->packets));
    SLIST_INIT(&(drv->packetPool));
    drv->current=NULL;
    drv->maxPacketsCounts = maxPackets;
    drv->mtu = mtu;
    drv->packetsCounts = 0;
    drv->errorPacketsCounts = 0;
    drv->packetOverrunsCounts = 0;
    drv->serialDriver = serialDriver;
    drv->thread = NULL;
    int i=0;
    for (;i<maxPackets;i++) {
        struct SerialPacketItem *item;
        MALLOC(item,sizeof(struct SerialPacketItem));
        MALLOC(item->data, mtu);
        item->next.sle_next = NULL;
        SLIST_INSERT_HEAD(&(drv->packetPool),item,next);
    }
}

SerialPacketDriver *spGetSerialPacketDriver(char *name) {
    if (!name || SLIST_EMPTY(&sPLists.spItems))
        return NULL;
    struct SPDriverListItem *it=NULL;
    SLIST_FOREACH(it, &(sPLists.spItems), next)
        if (it && strcmp(it->driver->name, name)==0)
            return it->driver;

    return NULL;
}

static void showDriverStat(SerialPacketDriver *drv, BaseSequentialStream *chp, uint8_t detailed) {
    if (!(drv && chp))
        return;

    chprintf(chp, "name:%s, maxPacketCounts:%d, mtu:%d, allCounts:%d, errors:%d, overruns:%d\r\n",
             drv->name//, drv->maxPacketsCounts, drv->mtu, drv->packetsCounts,
             //drv->errorPacketsCounts, drv->packetOverrunsCounts
             );
    int poolSize=0, currents=0;
    struct SerialPacketItem *it=NULL;
    SLIST_FOREACH(it, &(drv->packetPool), next)
        poolSize++;
    SLIST_FOREACH(it, &(drv->packets), next) {
        if (!detailed) {
            chprintf(chp, "%d: address:%d, command:%d, datalength:%d\r\n",
                     currents, it->address, it->command, it->len);
            if (it->len>0)
                hexDump(chp, it->data, it->len);
        }
        currents++;
    }

    chprintf(chp, "poolsize is:%d, current packets counts is:%d\r\n",poolSize,currents);
}

void cmd_SPStat(BaseSequentialStream *chp, int argc, char *argv[]) {
   if (!argc) {
        chprintf(chp, "usage: %s <name> [-d]\r\n",CMD_SP_STAT);
        return;
   }
   SerialPacketDriver *it = spGetSerialPacketDriver(argv[0]);
   if (!it) {
        chprintf(chp, "Unknown driver:%s, use %s to show drivers\r\n", argv[0], CMD_SP_SHOW_DRIVERS);
        return;
   }
   showDriverStat(it, chp, argc == 2 && strcmp(argv[1], "-d")==0);
}

void cmd_SPShowDrivers(BaseSequentialStream *chp, int argc, char *argv[]) {
    (void) argv;
    if (argc > 0) {
        chprintf(chp, "usage: %s\r\n", CMD_SP_SHOW_DRIVERS);
        return;
    }
    struct SPDriverListItem  *it;
    int i=0;
    SLIST_FOREACH(it, &(sPLists.spItems), next)
        chprintf(chp, "%d:%s\r\n", i++, it->driver->name);
}

#endif //#if HAL_USE_SERIAL_PACKETS || defined(__DOXYGEN__)
