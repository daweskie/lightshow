/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include <stdio.h>
#include <termios.h>
#include <assert.h>
#include <bitset.h>
#include "include/crc.h"


void cmd_bitset_test()
{
    printf("Begin bitset test\n");
    bitsCreate(b14,14);
    bitsCreate(b42,42);
    bitsCreate(b130,130);

    bitsErase(b14);
    bitsErase(b42);
    bitsErase(b130);

    bitsSet(b14,2);
    bitsSet(b14,4);
    bitsSet(b14,6);
    bitsSet(b14,8);
    bitsSet(b14,10);
    bitsSet(b14,20);

    assert(bitsTest(b14,2));
    assert(bitsTest(b14,4));
    assert(bitsTest(b14,6));
    assert(bitsTest(b14,8));
    assert(!bitsTest(b14,0));
    assert(!bitsTest(b14,1));
    assert(!bitsTest(b14,3));
    assert(!bitsTest(b14,5));
    assert(!bitsTest(b14,20));

    bitsClear(b14,2);
    bitsClear(b14,4);
    bitsClear(b14,6);
    bitsClear(b14,8);
    bitsClear(b14,10);
    bitsClear(b14,20);

    int i=0;
    for (;i<20;i++)
        assert(!bitsTest(b14,i));

    bitsSet(b42,0);
    bitsSet(b42,10);
    bitsSet(b42,20);
    bitsSet(b42,30);
    bitsSet(b42,40);
    bitsSet(b42,50);

    assert(bitsTest(b42,0));
    assert(bitsTest(b42,10));
    assert(bitsTest(b42,20));
    assert(bitsTest(b42,30));
    assert(bitsTest(b42,40));
    assert(!bitsTest(b42,50));
    assert(!bitsTest(b42,1));
    assert(!bitsTest(b42,9));
    assert(!bitsTest(b42,11));
    assert(!bitsTest(b42,19));
    assert(!bitsTest(b42,21));
    assert(!bitsTest(b42,29));
    assert(!bitsTest(b42,31));
    assert(!bitsTest(b42,39));
    assert(!bitsTest(b42,41));

    bitsClear(b42,0);
    bitsClear(b42,10);
    bitsClear(b42,20);
    bitsClear(b42,30);
    bitsClear(b42,40);
    bitsClear(b42,50);

    for (i=0;i<50;i++)
        assert(!bitsTest(b42,i));

    for (i=0;i<140;i++)
        if (i%2)
            bitsSet(b130,i);

    for (i=0;i<130;i++)
        if (i%2)
            assert(bitsTest(b130,i));
        else
            assert(!bitsTest(b130,i));

    bitsErase(b14);
    bitsCreate(bMask, 14);

    for (i=0;i<14;i++)
        if(i%2)
            bitsSet(bMask,i);

    bitsOR(b14,bMask);

    bitsNegate(bMask);

    bitsAND(b14,bMask);

    for (i=0;i<14;i++)
        assert(!bitsTest(b14,i));

    bitsXOR(b14,bMask);

    printf("Successfully end of Bitset test\n");
}


void crc_test() {
    printf("Begin crc test\n");
    char buffer[] = { 1, 3, 0xf, 0xa0, 0, 1 };
    crc16_t CRC = 0x873c;

    crc16_t calculated;
    crcModbusBegin(calculated);
    int i=0;
    for (;i<sizeof(buffer);i++)
        crcModbusAddByte(&calculated, buffer[i]);

    printf("calculated crc is %x\n", calculated);
    assert(crcCheck(calculated, CRC));

}



int main(void) {
  crc_test();
  return 0;
}
