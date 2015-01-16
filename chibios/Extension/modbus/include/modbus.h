#ifndef MODBUSPACKET_H_INCLUDED
#define MODBUSPACKET_H_INCLUDED

#include "serialpacket.h"

#if HAL_USE_MODBUS

#define MODBUS_REGISTER_BASE 0x4000
#define MODBUS_CMD_SHOW_REGS "shmodreg"
#define MODBUS_CMD_SET_REGS "setmodreg"

#define _modbus_commands                          \
    {MODBUS_CMD_SHOW_REGS, cmd_ModShowReg},       \
    {MODBUS_CMD_SET_REGS, cmd_ModSetReg },

#define _modbus_packet_device_methods                                       \
    _serial_packet_device_methods                                           \
    void (*modbusSend)(void *instance, uint8_t address, uint8_t command,    \
                    uint16_t dataLength, uint8_t *data);

typedef uint16_t modbus_register_t;

typedef union {
    int32_t int_bits;
    float float_bits;
    modbus_register_t regs[2];
} modbus_float_register_t;


typedef enum  {
        modReadCoilStatus=1,                //Read Coil Status 01 (0x01)
        modReadDiscreteInputs=2,            //Read Discrete Inputs 02 (0x02)
        modReadHoldingRegisters = 3,        //Read Holding Registers 03 (0x03)
        modReadInputRegisters = 4,          //Read Input Registers 04 (0x04)
        modForceSingleCoil = 5,             //Force Single Coil 05 (0x05)
        modWriteSingleRegister = 6,         //Write Single Register 06 (0x06)
        modForceMultipleCoils = 7,          //Force Multiple Coils 15 (0x0F)
        modWriteMultipleRegisters = 0x16    //Write Multiple Registers 16 (0x10)
} ModbusCommands;

typedef struct {
    uint16_t regAddress;
    uint16_t len;
} ModbusAddressLength_t;

typedef enum {
    psmNone,
    psmFunction,
    psmRegisterHi,
    psmRegisterLo,
    psmDataLenHi,
    psmDataLenLo,
    psmCrcHi,
    psmCrcLo,
} PacketState;

#define _modbus_packet_device_data(poolListType,packetListType)    \
    _base_serial_packet_device_data(poolListType,packetListType)   \
    uint8_t modbusAddress;                                         \
    PacketState state;                                             \
    uint16_t registersBaseAddress;                                 \
    uint16_t regsCounts;                                           \
    uint16_t *registers;                                           \
    uint32_t excInvalidFunction;                                   \
    uint32_t excInvalidAddress;                                    \
    uint32_t excInvalidData;                                       \
    uint32_t excInvalidMBFunction;

struct ModbusPacketDeviceVMT {
    _modbus_packet_device_methods
};


typedef struct {
    const struct ModbusPacketDeviceVMT *vmt;
    _modbus_packet_device_data(PoolType,PacketType);
} ModbusDriver;


void modbusObjectInit(ModbusDriver *driver, char *name, SerialDriver *serialDriver,
                        uint8_t maxPackets, uint8_t modbusAddress,
                        modbus_register_t *registers, uint16_t regCounts, uint16_t regsBaseAddress);

int modbusSend(ModbusDriver *driver, uint8_t address, uint8_t command,
               uint16_t dataLength, uint16_t *data);

int modbusSetRegister(ModbusDriver *driver, uint16_t address, uint16_t value);

uint16_t modbusGetRegister(ModbusDriver *driver, uint16_t address);

uint16_t modRegisterToNative(modbus_register_t reg);

float modRegisterToFloat(modbus_float_register_t reg);

modbus_register_t modNativeToRegister(uint16_t value);

modbus_float_register_t modNativeToRegisterFloat(float value);

/*
Modbus related shell commands
shmodreg address count
setmodreg address value
*/
void cmd_ModShowReg(BaseSequentialStream *chp, int argc, char *argv[]);

void cmd_ModSetReg(BaseSequentialStream *chp, int argc, char *argv[]);



#endif //HAL_USE_MODBUS

#endif // MODBUSPACKET_H_INCLUDED
