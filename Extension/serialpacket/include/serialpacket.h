#ifndef SERIALPACKET_H_INCLUDED
#define SERIALPACKET_H_INCLUDED

#include <sys/queue.h>
#include "checksum.h"
#include "ch.h"
#include "hal.h"

#if HAL_USE_SERIAL_PACKETS || defined(__DOXYGEN__)

struct SerialPacketItem {
    uint8_t valid;
    uint8_t address;
    uint8_t command;
    uint16_t len;
    uint8_t *data;
    SLIST_ENTRY(SerialPacketItem) next;
};

#define _serial_packet_device_methods                                                           \
    void (*spStart)(void *instance, SerialConfig *config);                                      \
    struct SerialPacketItem *(*spGetElementFromPool)(void *instance);                           \
    int (*spBeginPacket)(void *instance, uint8_t address);                                      \
    int (*spSendChar)(void *instance, uint8_t ch);                                              \
    int (*spSendBuffer)(void *instance, uint16_t len, uint8_t *buffer);                         \
    void (*spAddValidPacket)(void *instance);                                                   \
    int (*spIsEmpty)(void *instance);                                                           \
    struct SerialPacketItem *(*spGetNextPacket)(void *instance);                                \
    int (*spRecyclePacket)(void *instance, struct SerialPacketItem *item);                      \
    uint32_t (*spGetAllPacketCounts)(void *instance);                                           \
    uint32_t (*spGetErrorCounts)(void *instance);                                               \
    uint32_t (*spGetPacketOverrunCounts)(void *instance);                                       \
    void (*spStartReceive)(void *instance);                                                     \
    void (*spStopReceive)(void *instance);                                                      \
    void (*spRxChar)(void *instance, uint8_t ch);                                               \
    void (*spProcessPackets)(void *instance);                                                   \
    void (*spChecksumBegin)(checksum_method_t method, crc16_t *crc);                            \
    void (*spChecksumAddByte) (checksum_method_t method, crc16_t *crc, uint8_t b);


#define _base_serial_packet_device_data(poolListType,packetListType)        \
    /* Name of driver */                                                    \
    char *name;                                                             \
    /* Serial driver */                                                     \
    SerialDriver *serialDriver;                                             \
    /* Serial Config*/                                                      \
    SerialConfig *serialConfig;                                             \
    /* Pool for empty packets  */                                           \
    SLIST_HEAD(poolListType, SerialPacketItem) packetPool;                  \
    /* Valid  packets  */                                                   \
    SLIST_HEAD(packetListType, SerialPacketItem) packets;                   \
    /* max number of packets */                                             \
    uint8_t maxPacketsCounts;                                               \
    /* max data length */                                                   \
    uint16_t mtu;                                                           \
    /* number of all packets */                                             \
    uint32_t packetsCounts;                                                 \
    /* number of error packets */                                           \
    uint32_t errorPacketsCounts;                                            \
    /* number of Packet overruns*/                                          \
    uint32_t packetOverrunsCounts;                                          \
    /* Current packet under receiveing */                                   \
    struct SerialPacketItem *current;                                       \
    /* flying crc during receive */                                         \
    crc16_t flyingCrc;                                                      \
    /* received crc */                                                      \
    crc16_t receivedCrc;                                                    \
    /* Data index during receive */                                         \
    uint16_t dIdx;                                                          \
    /* crc handling method type  */                                         \
    checksum_method_t checksumMethod;                                       \
    /* Thread which handles driver*/                                        \
    Thread *thread;

#define CMD_SP_STAT "spstat"
#define CMD_SP_SHOW_DRIVERS "spshowdrivers"

#define _base_serial_packet_commands                          \
    {CMD_SP_SHOW_DRIVERS, cmd_SPShowDrivers},                 \
    {CMD_SP_STAT, cmd_SPStat },

struct SerialPacketDeviceVMT {
    _serial_packet_device_methods
};

typedef struct {
    const struct SerialPacketDeviceVMT *vmt;
    _base_serial_packet_device_data(PoolList,PacketList)

} SerialPacketDriver;

#ifdef __cplusplus
extern "C" {
#endif

void spStart(void *instance, SerialConfig *config);

struct SerialPacketItem *spGetElementFromPool(void *instance);

int spSendChar(void *instance, uint8_t ch);

int spSendBuffer(void *instance, uint16_t len, uint8_t *buffer);

int spBeginPacket(void *instance, uint8_t address);

void spAddValidPacket(void *instance);

int spIsEmpty(void *instance);

struct SerialPacketItem *spGetNextPacket(void *instance);

int spRecyclePacket(void *instance, struct SerialPacketItem *item);

uint32_t spGetAllPacketCounts(void *instance);

uint32_t spGetErrorCounts(void *instance);

uint32_t spGetPacketOverrunCounts(void *instance);

int spPoolCounts(void *instance);

int spPacketCounts(void *instance);

void spStartReceive(void *instance);

void spStopReceive(void *instance);

void spObjectInit(void *instance, char *name, SerialDriver *serialDriver,
                  uint8_t maxPackets, uint16_t mtu);

struct SerialPacketItem *spGetNextPacket(void *instance);

int spRecyclePacket(void *instance, struct SerialPacketItem *item);

SerialPacketDriver *spGetSerialPacketDriver(char *name);

void cmd_SPStat(BaseSequentialStream *chp, int argc, char *argv[]);

void cmd_SPShowDrivers(BaseSequentialStream *chp, int argc, char *argv[]);


#ifdef __cplusplus
}
#endif

#endif //HAL_USE_SERIAL_PACKETS

#endif // SERIALPACKET_H_INCLUDED
