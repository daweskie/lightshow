#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <checksum.h>


void test_csCrc16Buffer()
{
    printf("test_csCrc16Buffer started...\n");

    crc16_t calcvalue=0;

    crc16_t crcvalue=0x7668;

    char buffer[] ={0xa3};

	calcvalue=csCrc16Buffer(buffer, sizeof(buffer));

    printf("the crc value is: %x\n", crcvalue);
    printf("the calculated crc value is: %x\n", calcvalue);

    assert(csChecksumEnd(calcvalue, crcvalue));

    printf("passed\n\n");
}

void test_csCrc16Fly()
{
    printf("test_csCrc16Fly started...\n");

    crc16_t calcvalue=0;

    crc16_t crcvalue=0x7668;

    //unsigned char gh=0xa3;

	char buffer[] ={0xa3};

	//printf("%x\n", calcvalue^=gh);

	//printf("%x\n", calcvalue^=gh);

    int i=0;

    for( ; i<sizeof(buffer); i++)
        csCrc16Fly(&calcvalue, buffer[i]);

    printf("the crc value is: %x\n", crcvalue);
    printf("the calculated crc value is: %x\n", calcvalue);

    assert(csChecksumEnd(calcvalue, crcvalue));

    printf("passed\n\n");
}

void test_csCrcModbusBuffer()
{
    printf("test_csCrcModbusBuffer started...\n");

    crc16_t calcvalue=0;

    crc16_t crcvalue=0x4079;

    char buffer[]={0xa3};

	calcvalue=csCrcModbusBuffer(buffer, sizeof(buffer));

    printf("the crc value is: %x\n", crcvalue);
    printf("the calculated crc value is: %x\n", calcvalue);

    assert(csChecksumEnd(calcvalue, crcvalue));

    printf("passed\n\n");
}

void test_csCrcModbusFly()
{
    printf("test_csCrcModbusFly started...\n");

    crc16_t calcvalue=(crc16_t)0xffff;

    crc16_t crcvalue=0x4079;

	//uint8_t gh=0xa3;

    char buffer[]={0xa3};

	//printf("%x\n", calcvalue^=gh);
	//printf("%x\n", calcvalue^=gh);

    int i=0;

    for( ; i<sizeof(buffer); i++)
        csCrcModbusFly(&calcvalue, buffer[i]);

    printf("the crc value is: %x\n", crcvalue);
    printf("the calculated crc value is: %x\n", calcvalue);

    assert(csChecksumEnd(calcvalue, crcvalue));

    printf("passed\n\n");
}


void test_csChecksumBuffer()
{
    printf("test_csChecksumBuffer started...\n");

    crc16_t calcvalue=0;

    crc16_t crcvalue=0xa3;

    char buffer[]={0xa3};

	calcvalue=csChecksumBuffer(buffer, sizeof(buffer));

    printf("the crc value is: %x\n", crcvalue);
    printf("the calculated crc value is: %x\n", calcvalue);

    assert(csChecksumEnd(calcvalue, crcvalue));

    printf("passed\n\n");
}

void test_csChecksumFly()
{
    printf("test_csChecksumFly started...\n");

    crc16_t calcvalue=0;

    crc16_t crcvalue=0xa3;

    char buffer[]={0xa3};

    int i=0;

    for( ; i<sizeof(buffer); i++)
        csChecksumFly(&calcvalue, buffer[i]);

    printf("the crc value is: %x\n", crcvalue);
    printf("the calculated crc value is: %x\n", calcvalue);

    assert(csChecksumEnd(calcvalue, crcvalue));

    printf("passed\n");
}

int main()
{
    test_csCrc16Buffer();
    test_csCrc16Fly();
    test_csCrcModbusBuffer();
    test_csCrcModbusFly();
    test_csChecksumBuffer();
    test_csChecksumFly();
    return 0;
}


