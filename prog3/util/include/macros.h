#ifndef MACROS_H_INCLUDED
#define MACROS_H_INCLUDED

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
/** \file macros.h
    \brief Commonly used macros
*/

#include <stdlib.h>
#include <strings.h>

#define MALLOC(ptr,size) \
    do { \
        ptr=malloc((size)); \
        if (!ptr) \
            abort(); \
    } while(0)

#define MALLOCZ(ptr,size) \
    do {  \
        MALLOC(ptr,(size));  \
        bzero(ptr,(size)); \
    }while(0)

#define FREE(ptr) \
    do { \
        free(ptr); \
        ptr = NULL; \
    } while(0)

#define MAX(a,b) ((a)<(b)) ? (b) : (a)

#define MIN(a,b) ((a)>(b)) ? (b) : (a)

#define AVERAGE(x,y) (((x)+(y))/2.0)

#define LOBYTE_16(w) ((uint8_t)(w))

#define HIBYTE_16(w) ((uint8_t)((((uint16_t)(w)>>8) & 0xff)))

#define BIT_CLEAR(val,mask) ((val) &=(~(mask)))

#define BIT_SET(val,mask) ((val) |= (mask))

#define BIT_TOGGLE(val,mask) ((val) ^= (mask))

#define BYTES_AS_UINT16(bytes) ((*(bytes) << 8) | (*((bytes)+1) & 0xff))


#endif // MACROS_H_INCLUDED
