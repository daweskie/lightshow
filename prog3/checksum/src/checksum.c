#include <checksum.h>

/*!
    the crc value calculation of a buffer
    @param data address of a buffer
    @param length length of the buffer
    @return the calculated crc value
*/
crc16_t csCrc16Buffer(char *data, size_t length)
{
    if (!data||length<=0)
        return 0;

    crc16_t remainder=0;                        // crc initial value

    size_t i;

    for(i=0; i<length; i++)                     // step until the buffer is endid
        csCrc16Fly(&remainder, *(data+i));      // atadja a crc cimet es az aktualis adat cimre mutato mutatot

    return remainder;
}

/*!
    on the fly crc calculation, one byte add to the crc
    @param crc the current value of the crc
    @param b the current byte
    @return the new crc value
*/
inline void csCrc16Fly(crc16_t *crc, unsigned char b)
{
    int bits;

    *crc^=b;                                   // xor

    for(bits=16; bits >0; bits--)              // step through the upper 8 bits
        if(*crc&0x8000)                        // If crc MSB of 1
            *crc=(*crc<<1)^POLYNOMIAL;         // egy bitet balra lepteti crc-t, és bitenkenti kizáro vagy kapcsolatba hozza POLYNOMIAL-al
        else                                   // balra kell leptetni mivel a legkisebb helyierteku bit van az elso helyen
            *crc<<=1;
}

/*!
    the modbus crc value calculation of a buffer
    @param data address of a buffer
    @param length length of the buffer
    @return the calculated crc value
*/
crc16_t csCrcModbusBuffer(char *data, size_t length)
{
    if (!data||length<=0)
        return 0;

    crc16_t remainder=(crc16_t)0xffff;          // crc initial value

    size_t i;

    for(i=0; i<length; i++)                     // step until the buffer is endid
        csCrcModbusFly(&remainder, *(data+i));  // atadja a crc cimet es az aktualis adat cimre mutato mutatot

    return remainder;
}

/*!
    on the fly crc calculation for modbus, one byte add to the crc
    @param crc the current value of the crc
    @param b the current byte
    @return the new crc value
*/
inline void csCrcModbusFly(crc16_t *crc, uint8_t b)
{
    int bits;

    *crc^=b;                                        // xor

    for(bits=16; bits>0; bits--)                    // step through the upper 8 bits
        if(*crc&1)                                  // If crc LSB of 1
            *crc=(*crc>>1)^POLYNOMIAL_MODBUS;       // egy bitet jobbra lepteti crc-t, és bitenkenti kizáro vagy kapcsolatba hozza POLYNOMIAL_MODBUS-al
        else                                        // jobbra kell leptetni mivel a legnagyobb helyierteku bit van az elso helyen
            *crc>>=1;
}

/*!
    the checksum calculation of a buffer
    @param data address of a buffer
    @param length length of the buffer
    @return the calculated checksum value
*/
crc16_t csChecksumBuffer(char *data, size_t length)
{
    if (!data||length<=0)
        return 0;

    crc16_t remainder=0;                             // crc initial value

    size_t i;

    for(i=0; i<length; i++)                         // step until the buffer is endid
        csChecksumFly(&remainder, *(data+i));       // atadja a crc cimet es az aktualis adat cimre mutato mutatot

	return remainder;
}

/*!
    on the fly crc calculation for checksum, one byte add to the crc
    @param crc the current value of the crc
    @param b the current byte
    @return the new crc value
*/
inline void csChecksumFly(crc16_t *crc, uint8_t b)
{
    *crc+=b;		                                // b-t hozzadja crc-hez
}

/*!
    the comparsion of the crc values
    @param crc1 the crc of the sent frame
    @param crc2 the calculated crc
    @return !0 if you match the the two crc values, 0 if not
*/
int csChecksumEnd(crc16_t crc1, crc16_t crc2) {
    return crc1==crc2;
}

