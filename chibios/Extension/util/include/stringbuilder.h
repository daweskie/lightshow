#ifndef STRINGBUILDER_H_INCLUDED
#define STRINGBUILDER_H_INCLUDED

/*
 * Copyright (C)  2013 Zoltan Zidarics (Zamek)
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/** \file stringbuilder.h
    \brief Safe String builder

Stringbuilder is a safe string appender helper utility.
ChibiOS doesn't prefer malloc, but local variable is usable to store strings.
There is a predefined buffer to use store strings.
Using Stringbuilder:
<pre>
    {
        char data[BUFFER_SIZE]; // buffer for strings
        struct StringBuilder sb; // Stringbuilder itself
        sbInit(&sb, data, BUFFER_SIZE); // Initialize stringbuilder

        printf("1.:%s\n",sbAddString(&sb, "First")->chars);
        printf("2.:%s\n",sbAddString(&sb,"Second")->chars);

        sbClear(&sb); // clear buffer can be reused from beginning
        printf("clear:%s",sb.chars);

        sbAddString(&sb, "A new string");

    }
</pre>
*/

#include <sys/types.h>
#include <stdlib.h>

/**
    Data structure for String builder.
*/
struct StringBuilder {
/** the full capability of stringbuilder  */
    size_t size;
/** the last used char position the pos-th element of buffer is always '\0' */
    size_t pos;
/** content of the builder as a normal C style string */
    char *chars;
};

/** Initialize a new Stringbuilder
    \param builder the address of Stringbuilder structure
    \param buffer for builder Cannot be NULL
    \param size the capacity of store buffer
    \return address of stringbuilder
*/
struct StringBuilder *sbInit(struct StringBuilder *builder, char *buffer, size_t size);

/**
    Clear the Stringbuilder
    it clears the 0. byte of chars buffer and set pos to 0
    \param address of builder cannot be NULL
    \return address of builder
*/
struct StringBuilder *sbClear(struct StringBuilder *builder);

/**
    Add a string to builder.
    It copies chars until str is ended or there are not enough place in buffer!
    \param builder address of builder. Cannot be NULL
    \param Normal C style string terminated '\0'
    \return address of builder
*/
struct StringBuilder *sbAddString(struct StringBuilder *builder, char *str);

/**
    Add a character to end of builder, if there is enough place.
    \param builder address of builder. Cannot be NULL
    \param character to add
    \return address of builder
*/
struct StringBuilder *sbAddChar(struct StringBuilder *builder, char ch);

#endif // STRINGBUILDER_H_INCLUDED
