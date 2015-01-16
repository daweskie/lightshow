#ifndef CRC_H_INCLUDED
#define CRC_H_INCLUDED

#include <stdint.h>
#include <stdlib.h>

#define POLYNOMIAL 0xd8 

#define POLYNOMIAL_MODBUS 0xa001

typedef uint8_t crc8_t;
typedef uint16_t crc16_t;

typedef enum {
    cmNone,
    cmCrc16,
    cmCrcmodbus,
    cmChecksum
}checksum_mode;

/*!
    the crc value calculation of a buffer
    @param data address of a buffer
    @param length length of the buffer
    @return the calculated crc value
*/
crc16_t csCrc16Buffer(char *data, size_t length);

/*! on the fly crc calculation, one byte add to the crc
    @param crc the current value of the crc
    @param b the current byte
    @return the new crc value
*/
inline void csCrc16Fly(crc16_t *crc, unsigned char b);

/*!
    the modbus crc value calculation of a buffer
    @param data address of a buffer
    @param length length of the buffer
    @return the calculated crc value
*/
crc16_t csCrcModbusBuffer(char *data, size_t length);

/*! on the fly crc calculation for modbus, one byte add to the crc
    @param crc the current value of the crc
    @param b the current byte
    @return the new crc value
*/
inline void csCrcModbusFly(crc16_t *crc, uint8_t b);

/*!
    the checksum calculation of a buffer
    @param data address of a buffer
    @param length length of the buffer
    @return the calculated checksum value
*/
crc16_t csChecksumBuffer(char *data, size_t length);

/*! on the fly crc calculation for checksum, one byte add to the crc
    @param crc the current value of the crc
    @param b the current byte
    @return the new crc value
*/
inline void csChecksumFly(crc16_t *crc, uint8_t b);

/*! the comparsion of the crc values
    @param crc1 the crc of the sent frame
    @param crc2 the calculated crc
    @return !0 if you match the the two crc values, 0 if not
*/
int csChecksumEnd(crc16_t crc1, crc16_t crc2);

#endif // CRC_H_INCLUDED
