#ifndef BITSET_H_INCLUDED
#define BITSET_H_INCLUDED


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
/** \file bitset.h
    \brief Bitset handling module. Statically defined bitsets, set/clear/test/shift/OR/AND/XOR operations.
    All functions realized with macros.

    Creating bitset:

    bitsCreate(variableName,numberOfMaxbits);

    Erase all of bits:

    bitsErase(variable);

    Set all of bits to 1:

    bitsFill(variable);

    Negate all of bits:

    bitsNegate(variable);

    Test if bit is set:

    bitsTest(variable,bitIndex);

    Set a bit to 1:

    bitsSet(variable,bitIndex);

    Clear a bit:

    bitsClear(variable,bitIndex);

    Toggle a bit:

    bitsToggle(variable,bitIndex);

    Shift left a bitset:

    bitsLeft(variable);

    Shift right a bitset:

    bitsRight(variable);

    And 2 bitset:

    bitsAND(variable,mask);

    OR  2 bitset:

    bitsOR(variable,mask);

    XOR  2 bitset:

    bitsXOR(variable,mask);

*/

#include <stdint.h>
#include <assert.h>
#include <string.h>

/**
    Create a set of bits. Warning! Corresponding the C convention it doesn't clear the bitset!
    After create you need to call bitsErase to clear all bits!
    \param name name of the bitset variable
    \param maxbits number of bit capacity
*/
#define bitsCreate(name,maxbits) \
    struct {  \
       int maxBits; \
       int arraySize; \
       uint32_t bits[maxbits/32u+1]; \
    } name= { \
        .maxBits=maxbits, \
        .arraySize=maxbits/32u+1 \
    };

/**
    bitsErase. Erase a bitset. All bits will be 0
    \param ba target Bitset variable.
*/
#define bitsErase(ba) \
        memset(&ba.bits, 0, ba.arraySize*sizeof(uint32_t))

/**
    bitsFill. Fill 1 to bitset. All bits will be 1
    \param ba target Bitset variable.
*/
#define bitsFill(ba) \
        memset(&ba.bits, (uint32_t)0xffffffff, \
               ba.arraySize*sizeof(uint32_t))

/**
    Negate all of bits in bitset.
    \param ba target Bitset variable.
*/
#define bitsNegate(ba) \
    do { \
        int _i=0; \
        for (;_i<ba.arraySize;_i++) \
            ba.bits[_i] = ~ ba.bits[_i]; \
    } while(0)

/**
    Test if selected bit is set.
    \param ba target bitset.
    \param bit index of the selected bit. If index > number of bits in bitset, value id 0
*/
#define bitsTest(ba,bit) \
        (ba.maxBits >= (bit) && \
            ba.bits[(bit)/32u] & (1u<<((bit)%32u)))

/**
    Set the selected bit to 1.
    \param ba target bitset
    \param bit index of the selected bit. If bit > number of bits nothing happend.
*/
#define bitsSet(ba,bit) \
    do { \
        if (ba.maxBits>=(bit)) \
            ba.bits[(bit)/32u] |= 1u<<((bit)%32u); \
    }while(0)

/**
    Clear the selected bit to 1.
    \param ba target bitset
    \param bit index of the selected bit. If bit > number of bits nothing happend.
*/
#define bitsClear(ba,bit) \
    do { \
        if (ba.maxBits >= (bit)) \
            ba.bits[(bit)/32u] &= ~(1u<<((bit)%32u)); \
    } while(0)

/**
    Toggle the selected bit to 1.
    \param ba target bitset
    \param bit index of the selected bit. If bit > number of bits nothing happend.
*/
#define bitsToggle(ba,bit) \
    do { \
        if (ba.maxBits >= (bit)) \
            ba.bits[(bit)/32u] ^= 1u<<((bit)%32u); \
    } while(0)

/**
    Shift left the bitset.
    \param ba target bitset
    \param shfts number of shift steps.
*/
#define bitsLeft(ba,shfts) \
    do { \
        int shft=shfts; \
        while((shft)--) { \
            uint _idx=ba.arraySize-1; \
            do { \
                ba.bits[_idx] <<= 1; \
                if (_idx && ba.bits[_idx-1u] & (1u<<31u)) \
                    ba.bits[_idx] |= 1; \
            }while(_idx--); \
        } \
    } while(0)

/**
    Shift right the bitset.
    \param ba target bitset
    \param shfts number of shift steps.
*/
#define bitsRight(ba,shfts) \
    do { \
        int _s=shfts; \
        while((_s)--) { \
            uint _idx=0; \
            do { \
                ba.bits[_idx++] >>= 1; \
                if (_idx < ba.arraySize && ba.bits[_idx] & 1) \
                    ba.bits[_idx-1] |= (1<<31); \
            }while(_idx<ba.arraySize); \
        } \
    }while(0)

/**
    Compute OR relation of 2 bitset. ba |= mask.
    \param ba the target bitset
    \param mask the mask bitset.
*/
#define bitsOR(ba,mask) \
    do { \
        int _i=0; \
        for (;_i<ba.arraySize; _i++) \
            ba.bits[_i] |= mask.bits[_i]; \
    } while(0)

/**
    Compute AND relation of 2 bitset. ba &= mask.
    \param ba the target bitset
    \param mask the mask bitset.
*/
#define bitsAND(ba,mask) \
    do { \
        int _i=0; \
        for (;_i<ba.arraySize;_i++) \
            ba.bits[_i] &= mask.bits[_i]; \
    } while(0)

/**
    Compute XOR relation of 2 bitset. ba ^= mask.
    \param ba the target bitset
    \param mask the mask bitset.
*/
#define bitsXOR(ba,mask) \
    do { \
        int _i=0; \
        for (;_i<ba.arraySize;_i++) \
            ba.bits[_i] ^= mask.bits[_i]; \
    } while(0)

#endif // BITSET_H_INCLUDED
