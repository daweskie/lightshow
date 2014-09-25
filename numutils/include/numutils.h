#ifndef NUMUTILS_H_INCLUDED
#define NUMUTILS_H_INCLUDED

/*
 * Copyright (C)  2014 Zoltan Zidarics (Zamek)
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/** \file numutils.h
    \brief Numberutilities
*/

/**
 Convert a string to an int, returning zero if the conversion fails.
 If the string is null, zero is returned.

 @param str the string to convert, may be null
 @return the int represented by the string, or zero if conversion fails
*/
int nu_str_toint(const char *str);

/**
 Convert a String to an int, returning a
* default value if the conversion fails.
*
* If the string is null, the default value is returned.
*
* @param str the string to convert, may be null
* @param defaultValue the default value
* @return the int represented by the string, or the default if conversion fails
*/
int nu_str_toint_default(const char *str, int defaultValue);

/**
 Convert a string to a long, returning zero if the conversion fails.
 If the string is null, zero is returned.

 @param str the string to convert, may be null
 @return the long represented by the string, or zero if conversion fails
*/
long nu_str_tolong(const char *str);

/**
 Convert a String to a long, returning a
* default value if the conversion fails.
*
* If the string is null, the default value is returned.
*
* @param str the string to convert, may be null
* @param defaultValue the default value
* @return the long represented by the string, or the default if conversion fails
*/
long nu_str_tolong_default(const char *str, long defaultValue);

/**
 Convert a string to a float, returning zero if the conversion fails.
 If the string is null, zero is returned.

 @param str the string to convert, may be null
 @return the float represented by the string, or zero if conversion fails
*/
float nu_str_tofloat(const char *str);

/**
 Convert a String to a float, returning a
* default value if the conversion fails.
*
* If the string is null, the default value is returned.
*
* @param str the string to convert, may be null
* @param defaultValue the default value
* @return the float represented by the string, or the default if conversion fails
*/
float nu_str_tofloat_default(const char *str, float defaultValue);

/**
* Convert a String to a byte, returning a
* zero value if the conversion fails.
*
* If the string is null, the zero value is returned.
*
* @param str the string to convert, may be null
* @return the byte represented by the string, or zero if conversion fails
*/
char nu_str_tobyte(const char *str);

/**
* Convert a String to a byte, returning a
* default value if the conversion fails.
*
* If the string is null, the default value is returned.
*
* @param str the string to convert, may be null
* @param defaultValue the default value
* @return the byte represented by the string, or the default if conversion fails
*/
char nu_str_tobyte_default(const char *str, char defaultValue);

/**
* Convert a String to a short, returning a
* zero value if the conversion fails.
*
* If the string is null, the zero value is returned.
*
* @param str the string to convert, may be null
* @return the byte represented by the string, or zero if conversion fails
*/
short nu_str_toshort(const char *str);

/**
* Convert a String to a short, returning a
* default value if the conversion fails.
*
* If the string is null, the default value is returned.
*
* @param str the string to convert, may be null
* @param defaultValue the default value
* @return the short represented by the string, or the default if conversion fails
*/
short nu_str_toshort_default(const char *str, short defaultValue);

/**
* Utility method to check if a string contains only zeros or null
*
* Returns 1 if str null.
*
* @param str the String to check, may be null
* @return 1 if it is all zeros or null
*/
char nu_is_all_zeros(const char *str);

/**
    Returns the less value of a and b
*/
#define nu_min(a,b) a

/**
    Returns the greatest value of a and b
*/
#define nu_max(a,b) b

/**
    Check if all characters of string is numeric (0..9)
    returns 1 if str is not null and all member is numeric (0..9)
    @param str the String to check, may be null
    @return 1 if str is not null and all member is numeric (0..9)
*/
char nu_is_digit(const char *str);


/**
    Check if a str is a valid number.
    Valid number is int including base (123, 0x12, 0b10101, 0o123, float)
    returns 1 if str is not null and is number
    @param str the String to check, may be null
    @return 1 if str is not null and is number
*/
char nu_is_number(const char *str);


#endif // NUMUTILS_H_INCLUDED
