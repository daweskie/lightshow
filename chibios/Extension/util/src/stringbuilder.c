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
/** \file strinbuilder.c
    \brief Stringbuilder for string appends
*/

#include "stringbuilder.h"

struct StringBuilder *sbInit(struct StringBuilder *builder, char * buffer, size_t size)
{
    if (! (builder&&buffer))
        return builder;

    builder->chars = buffer;
    builder->size = size;
    builder->pos = 0;
    *builder->chars='\0';
    return builder;
}

struct StringBuilder *sbClear(struct StringBuilder *builder)
{
    if (builder&&builder->chars) {
        builder->pos = 0;
        *(builder->chars) = '\0';
    }
    return builder;
}

struct StringBuilder *sbAddString(struct StringBuilder *builder, char *str)
{
    if (!(builder&&builder->chars&&str&&builder->pos<builder->size))
        return builder;

    register char c;
    while((c=*str++) && builder->pos<builder->size-1)
        *(builder->chars+builder->pos++) = c;

    *(builder->chars+builder->pos)='\0';

    return builder;
}

struct StringBuilder *sbAddChar(struct StringBuilder *builder, char ch)
{
    if (!(builder&&builder->chars&&builder->pos<builder->size-1))
        return builder;

    *(builder->chars+builder->pos++)=ch;
    *(builder->chars+builder->pos)='\0';
    return builder;

}
