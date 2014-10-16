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

#include <string.h>
#include <stdbool.h>
#define white_space(c) ((c) == ' ' || (c) == '\t')
#define valid_digit(c) ((c) >= '0' && (c) <= '9')

/** \file strutils.c
    \brief strberutilities
*/

/**
 Convert a string to an int, returning zero if the conversion fails.
 If the string is null, zero is returned.

 @param str the string to convert, may be null
 @return the int represented by the string, or zero if conversion fails
*/
int nu_str_toint(const char *str)
{
    if (!str || !*str)
        return 0;

    int i=0,sum=0;


    while(str[i]!='\0')
    {
        if(i>4)
        {
            return 0;
        }

        else if (str[i]< 48 || str[i] > 57)
        {
            //printf("Unable to convert it into integer.\n");
            return 0;
        }
        else
        {
            sum = sum*10 + str[i] - 48;
            i++;
        }

    }

    return sum;


}

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
int nu_str_toint_default(const char *str, int defaultValue)
{
    if (!str || !*str)
        return defaultValue;

    int i=0,sum=0;


    while(str[i]!='\0')
    {
        if(i>4)
        {
            return defaultValue;
        }

        else if (str[i]< 48 || str[i] > 57)
        {
            //printf("Unable to convert it into integer.\n");
            return defaultValue;
        }
        else
        {
            sum = sum*10 + str[i] - 48;
            i++;
        }

    }

    return sum;


}

/**
 Convert a string to a long, returning zero if the conversion fails.
 If the string is null, zero is returned.

 @param str the string to convert, may be null
 @return the long represented by the string, or zero if conversion fails
*/
long nu_str_tolong(const char *str)
{
    if (!str || !*str)
        return 0;

    int i=0,sum=0;


    while(str[i]!='\0')
    {
        if(i>9)
        {
            return 0;
        }

        else if (str[i]< 48 || str[i] > 57)
        {
            //printf("Unable to convert it into integer.\n");
            return 0;
        }
        else
        {
            sum = sum*10 + str[i] - 48;
            i++;
        }

    }

    return sum;


}

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
long nu_str_tolong_default(const char *str, long defaultValue)
{
    if (!str || !*str)
        return defaultValue;

    int i=0,sum=0;


    while(str[i]!='\0')
    {
        if(i>9)
        {
            return defaultValue;
        }

        else if (str[i]< 48 || str[i] > 57)
        {
            //printf("Unable to convert it into integer.\n");
            return defaultValue;
        }
        else
        {
            sum = sum*10 + str[i] - 48;
            i++;
        }

    }

    return sum;


}

/**
 Convert a string to a float, returning zero if the conversion fails.
 If the string is null, zero is returned.

 @param str the string to convert, may be null
 @return the float represented by the string, or zero if conversion fails
*/

float nu_str_tofloat(const char *str)
{
    int i=0; // Initialize result and i
    float res=0,res1=0,res2=0,powa=10;

    if (!str || !*str)
    {
        //printf("Not too much...man. \n");
        return 0;
    }
    while(str[i]!='\0')
    {
        if(str[i]< 46 || str[i] > 57 || i>9)
        {
            return 0;
        }
        i++;
    }
    i==0;

    while(str[i]!='\0')
    {

        while(str[i]!='.')
        {
            res1 = res1*10 + str[i]- 48;
            //printf("%f\n", res1);
            i++;
        }
        i++;
        while(str[i]!='\0')
        {
            res2 = res2 +((str[i]-48)/(powa));
            powa = powa*10;
            //printf("Current: %d\n", str[i]);

            i++;
        }
        res=res1+res2;
    }
    //printf("%d!\n", res); // I was using this shit during trouble shooting.


    //printf("res= %f\n", res);
    return res;

}

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
float nu_str_tofloat_default(const char *str, float defaultValue)
{
}

/**
* Convert a String to a byte, returning a
* zero value if the conversion fails.
*
* If the string is null, the zero value is returned.
*
* @param str the string to convert, may be null
* @return the byte represented by the string, or zero if conversion fails
*/
char nu_str_tobyte(const char *str)
{
    if (!str || !*str)
    {
        return 0;

    }

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
    char nu_str_tobyte_default(const char *str, char defaultValue)
    {
    }

    /**
    * Convert a String to a short, returning a
    * zero value if the conversion fails.
    *
    * If the string is null, the zero value is returned.
    *
    * @param str the string to convert, may be null
    * @return the byte represented by the string, or zero if conversion fails
    */
    short nu_str_toshort(const char *str)
    {
    }

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
    short nu_str_toshort_default(const char *str, short defaultValue)
    {
    }

    /**
    * Utility method to check if a string contains only zeros or null
    *
    * Returns 1 if str null.
    *
    * @param str the String to check, may be null
    * @return 1 if it is all zeros or null
    */
    char nu_is_all_zeros(const char *str)
    {
    }

    /**
        Check if all characters of string is streric (0..9)
        returns 1 if str is not null and all member is streric (0..9)
        @param str the String to check, may be null
        @return 1 if str is not null and all member is streric (0..9)
    */
    char nu_is_digit(const char *str)
    {
    }


    /**
        Check if a str is a valid strber.
        Valid strber is int including base (123, 0x12, 0b10101, 0o123, float)
        returns 1 if str is not null and is strber
        @param str the String to check, may be null
        @return 1 if str is not null and is strber
    */
    char nu_is_strber(const char *str)
    {
    }
