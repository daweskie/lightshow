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
/** \file ModbusPacket.c
    \brief Modbus driver for ChibiOS
*/

#include <stdlib.h>
#include <stdint.h>

#include <ch.h>
#include <hal.h>
#include <chprintf.h>

#include <endian.h>
#include <checksum.h>
#include <macros.h>
#include <cmdutil.h>
#include <modbus.h>

#if HAL_USE_MODBUS

#define MAX_MODBUS_DEVICE_ADDRESS 247
#define EXCEPTION_INVALID_FUNCTION_CODE 1
#define EXCEPTION_INVALID_ADDRESS 2
#define EXCEPTION_INVALID_DATA_VALUE 3
#define EXCEPTION_INVALID_MB_FUNCTION 4
#define EXCEPTION_OFFSET 0x80
#define MODBUS_MAX_REGSCOUNTS_PER_FRAME 0x7d

/**
    Set a modbus holding register
    \param driver, Modbus driver NULL safe
    \param address of the register. This is a modbus address including MODBUS_REGISTER_BASE
           valid address range is from drv->registersBaseAddress to drv->registersBaseAddress+drv->regsCounts
    \param value the 16 bit value of register. It is a native endian format and function converts it to Modbus Big endian format to store.
    \return EXIT_SUCCESS if ok, EXIT_FAILURE if driver is NULL or address is out of range
*/
int modbusSetRegister(ModbusDriver *driver, uint16_t address, uint16_t value) {
    if (! (driver && address >= driver->registersBaseAddress && address <= driver->registersBaseAddress+driver->regsCounts) )
        return EXIT_FAILURE;

    uint16_t offset = address - driver->registersBaseAddress;
    *(driver->registers+offset) = htobe16(value);
    return EXIT_SUCCESS;
}

/**
    Get a value from register of modbus.
    \param driver, Modbus driver NULL safe
    \param address of the register. This is a modbus address including MODBUS_REGISTER_BASE
           valid address range is from drv->registersBaseAddress to drv->registersBaseAddress+drv->regsCounts
    \return native endian value of register (not big endian!) or 0 if driver is NULL or address is out of range
*/
uint16_t modbusGetRegister(ModbusDriver *driver, uint16_t address) {
    if (! (driver && address >= driver->registersBaseAddress && address <= driver->registersBaseAddress+driver->regsCounts) )
        return 0;

    uint16_t offset = address - driver->registersBaseAddress;
    return be16toh(*(driver->registers+offset));
}


static int processFunctionCode(ModbusDriver *drv, uint8_t code) {
    if (! (drv && drv->current))
        return 0;

    switch (code) {
    case modReadHoldingRegisters :
            drv->current->command = modReadHoldingRegisters;
            drv->state = psmRegisterHi;
            return 1;

    case modWriteMultipleRegisters :
            drv->current->command = modWriteMultipleRegisters;
            drv->state = psmRegisterHi;
            return 1;

    default :
        return 0;
    }
}

static void modbusRxchar(void *instance, uint8_t c){
    ModbusDriver *drv = (ModbusDriver *)instance;
    if (!drv)
        return;

    switch(drv->state) {
    case psmNone :
        if (c >= 1 && c <= MAX_MODBUS_DEVICE_ADDRESS) {
            if (!spBeginPacket(instance, c))
                break;

            drv->state = psmFunction;
            return;
        }
        break;

    case psmFunction :
        if (processFunctionCode(drv, c)) {
                            crcModbusAddByte(&(drv->flyingCrc), c);
                            return;
                        }

                      break;

    case psmRegisterHi :
        if (drv->current) {
            crcModbusAddByte(&(drv->flyingCrc), c);
            ((ModbusAddressLength_t *)drv->current->data)->regAddress = (c & 0xff) << 8;
            drv->state = psmRegisterLo;
            return;
        }
        break;

    case psmRegisterLo :
        if (drv->current) {
            crcModbusAddByte(&(drv->flyingCrc), c);
            ((ModbusAddressLength_t *)drv->current->data)->regAddress |= (c & 0xff);
            drv->state = psmDataLenHi;
            return;
        }
        break;

    case psmDataLenHi :
        if (drv->current) {
            crcModbusAddByte(&(drv->flyingCrc), c);
            ((ModbusAddressLength_t *)drv->current->data)->len = (c & 0xff) << 8;
            drv->state = psmDataLenLo;
            return;
        }
        break;

    case psmDataLenLo:
        if (drv->current) {
            crcModbusAddByte(&(drv->flyingCrc), c);
            ((ModbusAddressLength_t *)drv->current->data)->len |= (c & 0xff);
            drv->state = psmCrcHi;
            return;
        }
        break;

    case psmCrcHi:
        if (drv->current) {
            drv->receivedCrc = (c & 0xff) << 8;
            drv->state = psmCrcLo;
            return;
        }
        break;

    case psmCrcLo:
        if (drv->current) {
            drv->receivedCrc |= (c & 0xff);
            if (crcCheck(drv->receivedCrc, htobe16(drv->flyingCrc))) {
                spAddValidPacket(instance);
                drv->state = psmNone;
                return;
            }
        }
        break;

    default:break;

    }
    drv->state = psmNone;
    ++drv->errorPacketsCounts;
}

static void modbusError(ModbusDriver *drv, uint8_t command, uint8_t exceptionCode) {
    spSendChar(drv, drv->modbusAddress);
    spSendChar(drv, command+EXCEPTION_OFFSET);
    spSendChar(drv, exceptionCode);
}

static void readHoldingRegisters(ModbusDriver *drv, struct SerialPacketItem *item) {
    if (! (drv && item))
        return;

    ModbusAddressLength_t *ma = (ModbusAddressLength_t *) item->data;

    uint16_t addr = ma->regAddress;
    uint8_t len = ma->len;

    if (len <=0 || len >= MODBUS_MAX_REGSCOUNTS_PER_FRAME) {
        modbusError(drv, modReadHoldingRegisters, EXCEPTION_INVALID_DATA_VALUE);
        ++drv->excInvalidData;
        goto endfunc;
    }
    if (addr<drv->registersBaseAddress || len > drv->regsCounts) {
        modbusError(drv, modReadHoldingRegisters, EXCEPTION_INVALID_ADDRESS);
        ++drv->excInvalidAddress;
        goto endfunc;
    }

    modbusSend(drv, drv->modbusAddress, modReadHoldingRegisters,
               len, &drv->registers[addr-drv->registersBaseAddress]);

endfunc:
    spRecyclePacket(drv, item);
}

static void writeMultipleRegisters(ModbusDriver *drv, struct SerialPacketItem *item) {
    if (! (drv && item))
        return;

    ModbusAddressLength_t *ma = (ModbusAddressLength_t *) item->data;
    uint8_t addr = be16toh(ma->regAddress);
    uint16_t len = be16toh(ma->len);
    if (len <=0 || len >= MODBUS_MAX_REGSCOUNTS_PER_FRAME) {
        modbusError(drv, modReadHoldingRegisters, EXCEPTION_INVALID_DATA_VALUE);
        ++drv->excInvalidData;
        goto endfunc;
    }
    if (addr<drv->registersBaseAddress || len > drv->regsCounts) {
        modbusError(drv, modReadHoldingRegisters, EXCEPTION_INVALID_ADDRESS);
        ++drv->excInvalidAddress;
        goto endfunc;
    }

    int i=len;

    addr -= drv->registersBaseAddress;

    for (;i<len;i++) {
        uint16_t value = be16toh(*(item->data+i));
        modbusSetRegister(drv, addr++, value);
    }

    modbusSend(drv, drv->modbusAddress, modWriteMultipleRegisters, 0, NULL);

endfunc:
    spRecyclePacket(drv, item);
}

static void modbusProcessPackets(void *instance) {
    ModbusDriver *drv = (ModbusDriver *)instance;
    do {
        struct SerialPacketItem *item = spGetNextPacket(instance);
        if (! (item && item->address == drv->modbusAddress))
            return;

        switch (item->command) {
        case modReadHoldingRegisters : readHoldingRegisters(drv, item);
            break;
        case modWriteSingleRegister :
        case modWriteMultipleRegisters : writeMultipleRegisters(drv, item);
            break;

        default:
            modbusError(drv, item->command, EXCEPTION_INVALID_FUNCTION_CODE);
            ++drv->excInvalidFunction;
            spRecyclePacket(drv, item);
        }
    }while(1);
}

/**
    Send a modbus frame to serial.
    \param driver, Modbus driver NULL safe
    \param address modbus address of this device
    \param command reply to this command
    \param regCounts size of data in this replay
    \param address of data
    \return EXIT_SUCCESS if ok, or EXIT_FAILURE if driver is null
*/
int modbusSend(ModbusDriver *driver, uint8_t address, uint8_t command,
               uint16_t regCounts, uint16_t *data) {
#define sc(crc,ch) \
        do { \
          crcModbusAddByte((&(crc)),(ch)); \
          spSendChar(driver, (ch)); \
        } while(0)

    if (!driver)
        return EXIT_FAILURE;

    crc16_t crc=0;
    crcModbusBegin(crc);

    sc(crc, address);
    sc(crc, command);
    uint8_t dl=regCounts * 2;
    sc(crc,dl);
    if (regCounts > 0) {
        int i=0;
        for (;i<regCounts;i++) {
            char ch = (*(data+i) >> 8) & 0xff;
            sc(crc, ch);
            ch = *(data+i) & 0xff;
            sc(crc, ch);
        }
    }
    spSendChar(driver, crc & 0xff);
    spSendChar(driver, (crc >> 8) & 0xff);
    return EXIT_SUCCESS;
}

const struct ModbusPacketDeviceVMT modbusPacketDeviceVMT = {
    .spGetElementFromPool=spGetElementFromPool,
    .spBeginPacket=spBeginPacket,
    .spAddValidPacket=spAddValidPacket,
    .spIsEmpty=spIsEmpty,
    .spGetNextPacket=spGetNextPacket,
    .spRecyclePacket=spRecyclePacket,
    .spGetAllPacketCounts=spGetAllPacketCounts,
    .spGetErrorCounts=spGetErrorCounts,
    .spGetPacketOverrunCounts=spGetPacketOverrunCounts,
    .spRxChar=modbusRxchar,
    .spProcessPackets=modbusProcessPackets,
    .modbusSend=modbusSend
};

/**
    Initialize modbus module
    \param driver, Modbus driver NULL safe
    \param name of this module for find by name
    \param serialDriver physical io assigned this modbus module
    \param maxPackets maximum number of packets, this is the size of packet pool
    \param modbusAddress of this module
    \param registers address of holdingregisters for this module. Registers are uint16_t
    \param regCounts of registers
    \param regsBaseAddress offset of holdingregisters. Normally it is 0x4000
*/
void modbusObjectInit(ModbusDriver *driver, char *name, SerialDriver *serialDriver,
                        uint8_t maxPackets, uint8_t modbusAddress,
                        modbus_register_t *registers, uint16_t regCounts, uint16_t regsBaseAddress){

     if (! driver)
        return;

    uint16_t mtu = min(regCounts, MODBUS_MAX_REGSCOUNTS_PER_FRAME) * 2
            + 1         // address
            + 2         // regbase
            + 2         // length
            + 2;        // crc
    spObjectInit(driver, name, serialDriver, maxPackets, mtu);
    driver->vmt = &modbusPacketDeviceVMT;
    driver->state = psmNone;
    driver->modbusAddress = modbusAddress;
    driver->registers = registers;
    driver->regsCounts = regCounts;
    driver->registersBaseAddress = regsBaseAddress;
    driver->excInvalidFunction = 0;
    driver->excInvalidAddress=0;
    driver->excInvalidData=0;
    driver->excInvalidMBFunction=0;
}

uint16_t modRegisterToNative(modbus_register_t reg) {
    return be16toh(reg);
}

modbus_register_t modNativeToRegister(uint16_t value) {
    return htobe16(value);
}

modbus_float_register_t modNativeToRegisterFloat(float value) {
    modbus_float_register_t result;
    result.float_bits = value;
    return result;
}

float modRegisterToFloat(modbus_float_register_t reg) {
    return reg.float_bits;
}

/*
Modbus related shell commands
shmodreg driver address count
setmodreg driver address value
*/

struct ModbusCommandParams {
    ModbusDriver *drv;
    uint16_t address;
};

static int extractModbusCommandParams(char *commandName, BaseSequentialStream *chp, int argc, char **argv,
                                      struct ModbusCommandParams *params) {
    if (!(params && chp && argv))
        return 0;

    params->drv=NULL;
    params->address=-1;

    if (argc<3) {
        chprintf(chp, "usage: %s address(hex) counts or value\r\n", commandName);
        return 0;
    }
    SerialPacketDriver *spd = spGetSerialPacketDriver(argv[0]);
    if (!spd) {
        chprintf(chp, "Unknown device:%s\r\n", argv[0]);
        return 0;
    }
    params->drv=(ModbusDriver *)spd;

    long address = strtol(argv[1],NULL,16);
    if (address < params->drv->registersBaseAddress ||
        address >= params->drv->registersBaseAddress+params->drv->regsCounts) {
        chprintf(chp, "valid address ranges are:%x to %x\r\n",
                 params->drv->registersBaseAddress,
                 params->drv->registersBaseAddress+params->drv->regsCounts);
        return 0;
    }
    params->address = address & 0xffff;
    return 1;
}

/**
    Show modbus registers
    format: shmodreg device address counts
*/
void cmd_ModShowReg(BaseSequentialStream *chp, int argc, char *argv[]) {
    struct ModbusCommandParams params;
    if (!extractModbusCommandParams(MODBUS_CMD_SHOW_REGS, chp, argc, argv,&params))
        return;

    int cnts=atoi(argv[2]);
    if (cnts<0 || cnts > params.drv->regsCounts) {
        chprintf(chp, "valid regscounts is 0 to %d\r\n", params.drv->regsCounts);
        return;
    }
    uint16_t i=params.address;
    for (;i<params.drv->registersBaseAddress+cnts;i++) {
        uint16_t val = modbusGetRegister(params.drv, i);
        chprintf(chp,"register %x is: %d (%x)\r\n", i, val, val);
    }
}

/**
    Set modbus registers
    format: setmodreg device address value
*/

void cmd_ModSetReg(BaseSequentialStream *chp, int argc, char *argv[]) {
    struct ModbusCommandParams params;
    if (!extractModbusCommandParams(MODBUS_CMD_SET_REGS, chp, argc, argv,&params))
        return;

    char *tail;
    uint16_t value=strtol(argv[2], &tail, 16);
    modbusSetRegister(params.drv, params.address, value);
}

#endif //HAL_USE_MODBUS
