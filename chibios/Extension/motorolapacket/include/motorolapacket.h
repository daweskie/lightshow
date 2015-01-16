#ifndef MOTOROLAPACKET_H_INCLUDED
#define MOTOROLAPACKET_H_INCLUDED

#include "serialpacket.h"

#define _motorola_packet_device_methods                                                                     \
    _serial_packet_device_methods                                                                           \
    void (*motSend)(void *instance, uint8_t address, uint8_t command, \
                    uint16_t dataLength, uint8_t *data);

typedef enum {
    psNone,
    ps55,
    ps1,
    psAddress,
    psCmd,
    psDlenLo,
    psDlenHi,
    psData,
    psCrcLo,
    psCrcHi
} PacketState;


#define _motorola_packet_device_data(poolListType,packetListType)    \
    _base_serial_packet_device_data(poolListType,packetListType)     \
    PacketState state;                                               \
    uint8_t preambleCounts

struct MotorolaPacketDeviceVMT {
    _motorola_packet_device_methods
};


typedef struct {
    const struct MotorolaPacketDeviceVMT *vmt;
    _motorola_packet_device_data(PoolType,PacketType);
} MotorolaPacketDriver;


void motorolaObjectInit(MotorolaPacketDriver *driver, char *name, SerialDriver *serialDriver,
                        uint8_t maxPackets, uint16_t mtu, uint8_t preambleCounts);

int motSend(MotorolaPacketDriver *driver, uint8_t address, uint8_t command, uint16_t dataLength, uint8_t *data);

#endif // MOTOROLAPACKET_H_INCLUDED
