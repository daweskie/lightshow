#ifndef CHECKSUM_H_INCLUDED
#define CHECKSUM_H_INCLUDED

#include <stdint.h>
#include <stdlib.h>

typedef uint8_t crc8_t;

typedef uint16_t crc16_t;

typedef enum {
    csNone,
    csChecksum,
    csCrc8,
    csCrc16,
    csCrcModbus,
    csMd5,
    csSha1
}checksum_method_t;

/**
    Calculate the crc value of a buffer
    \param data address of buffer
    \param d_len length of the buffer
    \return calculated crc
*/
crc16_t crc16CalcBuffer(char *data, size_t d_len);

/** on the fly crc calculation
    add a byte to the crc
    \param crc aggregated value of crc
    \param byte to aggregate to crc
    \return new crc value
*/
void crcModbusAddByte(crc16_t *crc, uint8_t b);

/** on the fly crc calculation
    add a byte to the crc
    \param crc aggregated value of crc
    \param byte to aggregate to crc
    \return new crc value
*/
void crc16AddByte(crc16_t *crc, uint8_t b);

/** check two crc value
    \param crc_a the first crc value
    \param crc_b the second crc value
*/
#define crcCheck(crc_a,crc_b) ((crc_a)==(crc_b))

/**
    Modbus using reverse algorythm, its startvalue is 0xffff
*/
#define crcModbusBegin(crc) crc=(crc16_t)0xffff;

void checksumBegin(checksum_method_t method, crc16_t *crc);

void checksumAddByte(checksum_method_t method, crc16_t *crc, uint8_t value);

void checksumEnd(checksum_method_t method, crc16_t *crc);

#endif // CHECKSUM_H_INCLUDED
