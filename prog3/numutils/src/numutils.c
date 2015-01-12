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
#include <stdlib.h>
#include <math.h>




/** \file strutils.c
    \brief strberutilities

    AUTHOR : DAVID DOBO
*/

/**
 Convert a string to an int, returning zero if the conversion fails.
 If the string is null, zero is returned.

 @param str the string to convert, may be null
 @return the int represented by the string, or zero if conversion fails
*/
int nu_str_toint(const char *str)
{
    nu_str_toint_default(str,0);
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

    int i1 = 0;
    int str_length = 0;
    int i2=0;
    char storage[20]= {0};


    while(str[i1]!='\0')
    {
        i1++;
        str_length++;
    }
    //printf("\nhossz= %d",str_length); // FOR DEBUG ONLY


    while(str[i2]!='\0')
    {
        storage[i2] = str[i2];
        i2++;
    }
    storage[i2]='\0';  //at the end append null character to mark end of string

    //printf("\nThe entered string is : %s",storage); // FOR DEBUG ONLY





    if (((storage[0] == '0') && ((storage[1] == 'x') || (storage[1] == 'X') )) && (str_length < 5))
    {
        if ((storage[2] == '0') && (storage[3] == '1'))
        {
            memset(storage, 0, 20);
            storage[0] = '1';

        }
        else if ((storage[2] == '1') && (storage[3] == '0'))
        {

            memset(storage, 0, 20);
            storage[0] = '1';
            storage[1] = '6';

        }
        else if ((storage[2] == '0') && (storage[3] == '0'))
        {

            memset(storage, 0, 20);
            storage[0] = '0';
            storage[1] = '0';

        }
        else if ((storage[2] == '1') && (storage[3] == '1'))
        {

            memset(storage, 0, 20);
            storage[0] = '1';
            storage[1] = '7';

        }
        else
        {
            memset(storage, 0, 20);
            storage[0] = '0';
        }
    }


    else if (((storage[0] == '0') && ((storage[1] == 'b') || (storage[1] == 'B'))) && (str_length < 5))
    {
        if ((storage[2] == '0') && (storage[3] == '1'))
        {

            memset(storage, 0, 20);
            storage[0] = '1';

        }
        else if ((storage[2] == '1') && (storage[3] == '0'))
        {


            memset(storage, 0, 20);
            storage[0] = '2';

        }
        else if ((storage[2] == '0') && (storage[3] == '0'))
        {


            memset(storage, 0, 20);
            storage[0] = '0';

        }
        else if ((storage[2] == '1') && (storage[3] == '1'))
        {


            memset(storage, 0, 20);
            storage[0] = '3';

        }
        else
        {
            memset(storage, 0, 20);
            storage[0] = '0';
        }
    }
    else if (((storage[0] == '0') && ((storage[1] == 'o' ) || (storage[1] == 'O' ))) && (str_length < 5))
    {
        if ((storage[2] == '0') && (storage[3] == '1'))
        {
            memset(storage, 0, 20);
            storage[0] = '1';

        }
        else if ((storage[2] == '1') && (storage[3] == '0'))
        {

            memset(storage, 0, 20);
            storage[0] = '8';

        }
        else if ((storage[2] == '0') && (storage[3] == '0'))
        {

            memset(storage, 0, 20);
            storage[0] = '0';

        }
        else if ((storage[2] == '1') && (storage[3] == '1'))
        {

            memset(storage, 0, 20);
            storage[0] = '9';


        }
        else
        {
            memset(storage, 0, 20);
            storage[0] = '0';
        }
    }
    else
    {

    }
    //printf("\nThe entered string after check is : %s",storage); // FOR DEBUG ONLY

    //printf("\nThe entered string after h|o|b is : %s",storage); // FOR DEBUG ONLY

    if (!*storage)
        return defaultValue;

    long long sum=0;
    int i, end;
    end =0;
    i = 0;



    while(storage[i]!='\0')
    {

        if ((storage[i]< '1' || storage[i] > '9')  || (i>10) )
        {

            return defaultValue;
        }
        else
        {

            sum = sum*10 + storage[i] - 48;
            i++;


        }

    }
    if ((sum > 2147483647) || (sum < -2147483647))
    {
        return defaultValue;
    }
    end = sum;
    return end;


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

    long my_atoll(char *instr)
    {
        long long retval;
        int i;


        retval = 0;
        for (; *instr; instr++)
        {
            if ((*instr< '1') || (*instr > '9'))
            {
                return defaultValue;

            }
            else
                retval = 10*retval + (*instr - '0');
        }
        if (retval>2147483647)
        {
            return defaultValue;
        }

        return retval;
    }
    int i1 = 0;
    int str_length = 0;
    int i2=0;
    char storage[20]= {0};


    while(str[i1]!='\0')
    {
        i1++;
        str_length++;
    }
    //printf("\nhossz= %d",str_length); // FOR DEBUG ONLY


    while(str[i2]!='\0')
    {
        storage[i2] = str[i2];
        i2++;
    }
    storage[i2]='\0';  //at the end append null character to mark end of string

    // printf("\nThe entered string is : %s",storage); // FOR DEBUG ONLY





    if (((storage[0] == '0') && ((storage[1] == 'x') || (storage[1] == 'X') )) && (str_length < 5))
    {
        if ((storage[2] == '0') && (storage[3] == '1'))
        {
            memset(storage, 0, 20);
            storage[0] = '1';

        }
        else if ((storage[2] == '1') && (storage[3] == '0'))
        {

            memset(storage, 0, 20);
            storage[0] = '1';
            storage[1] = '6';

        }
        else if ((storage[2] == '0') && (storage[3] == '0'))
        {

            memset(storage, 0, 20);
            storage[0] = '0';
            storage[1] = '0';

        }
        else if ((storage[2] == '1') && (storage[3] == '1'))
        {

            memset(storage, 0, 20);
            storage[0] = '1';
            storage[1] = '7';

        }
        else
        {
            memset(storage, 0, 20);
            storage[0] = '0';
        }
    }


    else if (((storage[0] == '0') && ((storage[1] == 'b') || (storage[1] == 'B'))) && (str_length < 5))
    {
        if ((storage[2] == '0') && (storage[3] == '1'))
        {

            memset(storage, 0, 20);
            storage[0] = '1';

        }
        else if ((storage[2] == '1') && (storage[3] == '0'))
        {


            memset(storage, 0, 20);
            storage[0] = '2';

        }
        else if ((storage[2] == '0') && (storage[3] == '0'))
        {


            memset(storage, 0, 20);
            storage[0] = '0';

        }
        else if ((storage[2] == '1') && (storage[3] == '1'))
        {


            memset(storage, 0, 20);
            storage[0] = '3';

        }
        else
        {
            memset(storage, 0, 20);
            storage[0] = '0';
        }
    }
    else if (((storage[0] == '0') && ((storage[1] == 'o' ) || (storage[1] == 'O' ))) && (str_length < 5))
    {
        if ((storage[2] == '0') && (storage[3] == '1'))
        {
            memset(storage, 0, 20);
            storage[0] = '1';

        }
        else if ((storage[2] == '1') && (storage[3] == '0'))
        {

            memset(storage, 0, 20);
            storage[0] = '8';

        }
        else if ((storage[2] == '0') && (storage[3] == '0'))
        {

            memset(storage, 0, 20);
            storage[0] = '0';

        }
        else if ((storage[2] == '1') && (storage[3] == '1'))
        {

            memset(storage, 0, 20);
            storage[0] = '9';


        }
        else
        {
            memset(storage, 0, 20);
            storage[0] = '0';
        }
    }
    else
    {

    }

    //printf("\nThe entered string after check is : %s",storage); //FOR DEBUG ONLY

    //printf("\nThe entered string after h|o|b is : %s",storage); //FOR DEBUG ONLY

    long value2;

    value2 = my_atoll(storage);

    return value2;

}

/**
 Convert a string to a long, returning zero if the conversion fails.
 If the string is null, zero is returned.

 @param str the string to convert, may be null
 @return the long represented by the string, or zero if conversion fails
*/
long nu_str_tolong(const char *str)
{
    nu_str_tolong_default(str,0);

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
    if (!str || !*str)
    {
        return defaultValue;
    }

    int i1 = 0;
    int str_length = 0;
    int i2=0;
    char storage[20]= {0};


    while(str[i1]!='\0')
    {
        i1++;
        str_length++;
    }
    //printf("\nhossz= %d",str_length); // FOR DEBUG ONLY


    while(str[i2]!='\0')
    {
        storage[i2] = str[i2];
        i2++;

    }
    storage[i2]='\0';

    //printf("\nThe entered string is : %s",storage); FOR DEBUG ONLY

    float result= 0.0f;

    int dotpos=0;
    int n;
    if (storage[0]=='-'||storage[0]=='+')
        n=1;                 //set n to position 1.
    else                     //(number is not signed)
        n=0;                 //set n to position 0.

    for (; n < str_length; n++)
    {
        if (((storage[n]< '1' || storage[n] > '9')) && (storage[n] != '.'))
        {

            return defaultValue;
        }
        else if (storage[n] == '.')
        {
            dotpos = str_length - n  - 1;
        }
        else
        {
            result = result * 10.0f + (storage[n]-'0');
        }
    }
    while ( dotpos--)
    {
        result /= 10.0f;
    }
    if (storage[0]=='-')
        result*=(-1);

    return result;

}

/**
 Convert a string to a float, returning zero if the conversion fails.
 If the string is null, zero is returned.

 @param str the string to convert, may be null
 @return the float represented by the string, or zero if conversion fails
*/

float nu_str_tofloat(const char *str)
{
    nu_str_tofloat_default(str,0);
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
    int result;

    if (!str || !*str || (nu_str_toint(str)>255))
    {
        return defaultValue;
    }

    result = nu_str_toint(str);

    return result;
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
    nu_str_tobyte_default(str,0);
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
    short result = 0;

    result = nu_str_toint_default(str,0);

    if ((result>32767) || (result< -32767))
    {
        return defaultValue;
    }
    return result;
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
    nu_str_toshort_default(str,0);


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
    if (!str || !*str)
        return 1;

    int i=0;


    while(str[i]!='\0')
    {

        if (str[i] == 48 )
        {
            i++;
        }
        else
        {
            return 0;
        }
    }
    return 1;
}

/**
    Check if all characters of string is numeric (0..9)
    returns 1 if str is not null and all member is numeric (0..9)
    @param str the String to check, may be null
    @return 1 if str is not null and all member is numeric (0..9)
*/
char nu_is_digit(const char *str)
{
    if (!str || !*str)
        return 0;

    int i=0;


    if (str[0] == '-')
    {
        //sign = -1;
        i++;
    }

    while(str[i]!='\0')
    {

        if (str[i]> 47 && str[i] < 58)
        {
            i++;
        }
        else
        {
            return 0;
        }

    }

    return 1;


}



/**
    Check if a str is a valid number.
    Valid number is int including base (123, 0x12, 0b10101, 0o123, float)
    returns 1 if str is not null and is number
    @param str the String to check, may be null
    @return 1 if str is not null and is number
*/
char nu_is_number(const char *str)
{
    if (!str || !*str)
        return 0;

    int i=0;
    int cnt=0;


    while(str[i]!='\0')
    {
        i++;
        if(str[i] == '.')
        {
            cnt++;
        }
    }
    i = 0; //reset i

    if (str[0] == '-')
    {
        i++;
    }



    while(str[i]!='\0')
    {
        if (((str[1]== ('b' || 'o' || 'x')) || ((str[i] == 46)&& cnt == 1) || (str[i]> 47 && str[i] < 58)))
        {
            i++;
        }

        else
        {
            return 0;
        }

    }
    return 1;

}
