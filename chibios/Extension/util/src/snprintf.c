 /* Define ISO C stdio on top of C++ iostreams.
   Copyright (C) 1991, 1994-2008, 2009, 2010 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

/** \file snprintf.c
    \brief
*/
#include <stdarg.h>
#include <ctype.h>
#include <chtypes.h>
#include <string.h>

/* format read states */
#define DP_S_DEFAULT 0
#define DP_S_FLAGS 1
#define DP_S_MIN 2
#define DP_S_DOT 3
#define DP_S_MAX 4
#define DP_S_MOD 5
#define DP_S_CONV 6
#define DP_S_DONE 7

/* format flags - Bits */
#define DP_F_MINUS (1 << 0)
#define DP_F_PLUS (1 << 1)
#define DP_F_SPACE (1 << 2)
#define DP_F_NUM (1 << 3)
#define DP_F_ZERO (1 << 4)
#define DP_F_UP (1 << 5)
#define DP_F_UNSIGNED (1 << 6)

/* Conversion Flags */
#define DP_C_SHORT 1
#define DP_C_LONG 2
#define DP_C_LDOUBLE 3
#define DP_C_LLONG 4

/* format read states */
#define DP_S_DEFAULT 0
#define DP_S_FLAGS 1
#define DP_S_MIN 2
#define DP_S_DOT 3
#define DP_S_MAX 4
#define DP_S_MOD 5
#define DP_S_CONV 6
#define DP_S_DONE 7

/* format flags - Bits */
#define DP_F_MINUS (1 << 0)
#define DP_F_PLUS (1 << 1)
#define DP_F_SPACE (1 << 2)
#define DP_F_NUM (1 << 3)
#define DP_F_ZERO (1 << 4)
#define DP_F_UP (1 << 5)
#define DP_F_UNSIGNED (1 << 6)

/* Conversion Flags */
#define DP_C_SHORT 1
#define DP_C_LONG 2
#define DP_C_LDOUBLE 3
#define DP_C_LLONG 4

#define char_to_int(p) (p - '0')
#define abs_val(p) (p < 0 ? -p : p)
#define MAX(p,q) (((p) >= (q)) ? (p) : (q))

static void dopr_outch(char *buffer, size_t *currlen, size_t maxlen, char c) {
    if (*currlen < maxlen) {
        buffer[(*currlen)] = c;
    }
    (*currlen)++;
}

static void fmtint(char *buffer, size_t *currlen, size_t maxlen,
                   long value, int base, int min, int max, int flags) {
    int signvalue = 0;
    unsigned long uvalue;
    char convert[20];
    int place = 0;
    int spadlen = 0; /* amount to space pad */
    int zpadlen = 0; /* amount to zero pad */
    int caps = 0;

    if (max < 0)
        max = 0;

    uvalue = value;

    if(!(flags & DP_F_UNSIGNED)) {
        if( value < 0 ) {
            signvalue = '-';
            uvalue = -value;
        } else {
            if (flags & DP_F_PLUS) /* Do a sign (+/i) */
                signvalue = '+';
            else if (flags & DP_F_SPACE)
                signvalue = ' ';
        }
    }

    if (flags & DP_F_UP) caps = 1; /* Should characters be upper case? */

    do {
        convert[place++] =
            (caps? "0123456789ABCDEF":"0123456789abcdef")
            [uvalue % (unsigned)base ];
        uvalue = (uvalue / (unsigned)base );
    } while(uvalue && (place < 20));
    if (place == 20) place--;
    convert[place] = 0;

    zpadlen = max - place;
    spadlen = min - MAX (max, place) - (signvalue ? 1 : 0);
    if (zpadlen < 0) zpadlen = 0;
    if (spadlen < 0) spadlen = 0;
    if (flags & DP_F_ZERO) {
        zpadlen = MAX(zpadlen, spadlen);
        spadlen = 0;
    }
    if (flags & DP_F_MINUS)
        spadlen = -spadlen; /* Left Justifty */

#ifdef DEBUG_SNPRINTF
    printf("zpad: %d, spad: %d, min: %d, max: %d, place: %d\n",
           zpadlen, spadlen, min, max, place);
#endif

    /* Spaces */
    while (spadlen > 0) {
        dopr_outch (buffer, currlen, maxlen, ' ');
        --spadlen;
    }

    /* Sign */
    if (signvalue)
        dopr_outch (buffer, currlen, maxlen, signvalue);

    /* Zeros */
    if (zpadlen > 0) {
        while (zpadlen > 0) {
            dopr_outch (buffer, currlen, maxlen, '0');
            --zpadlen;
        }
    }

    /* Digits */
    while (place > 0)
        dopr_outch (buffer, currlen, maxlen, convert[--place]);

    /* Left Justified spaces */
    while (spadlen < 0) {
        dopr_outch (buffer, currlen, maxlen, ' ');
        ++spadlen;
    }
}

/* a replacement for modf that doesn't need the math library. Should
  be portable, but slow */
static double my_modf(double x0, double *iptr) {
    int i;
    long l;
    double x = x0;
    double f = 1.0;

    for (i=0; i<100; i++) {
        l = (long)x;
        if (l <= (x+1) && l >= (x-1)) break;
        x *= 0.1;
        f *= 10.0;
    }

    if (i == 100) {
        /* yikes! the number is beyond what we can handle. What do we do? */
        (*iptr) = 0;
        return 0;
    }

    if (i != 0) {
        double i2;
        double ret;

        ret = my_modf(x0-l*f, &i2);
        (*iptr) = l*f + i2;
        return ret;
    }

    (*iptr) = l;
    return x - (*iptr);
}

static long long ROUND(long double value) {
    long long intpart;

    intpart = (long long)value;
    value = value - intpart;
    if (value >= 0.5) intpart++;

    return intpart;
}

static long double POW10(int exp) {
    long double result = 1;

    while (exp) {
        result *= 10;
        exp--;
    }

    return result;
}

static void fmtfp (char *buffer, size_t *currlen, size_t maxlen,
                   long double fvalue, int min, int max, int flags) {
    int signvalue = 0;
    double ufvalue;
    char iconvert[311];
    char fconvert[311];
    int iplace = 0;
    int fplace = 0;
    int padlen = 0; /* amount to pad */
    int zpadlen = 0;
    int caps = 0;
    int index;
    double intpart;
    double fracpart;
    double temp;

    /*
    * AIX manpage says the default is 0, but Solaris says the default
    * is 6, and sprintf on AIX defaults to 6
    */
    if (max < 0)
        max = 6;

    ufvalue = abs_val (fvalue);

    if (fvalue < 0) {
        signvalue = '-';
    } else {
        if (flags & DP_F_PLUS) { /* Do a sign (+/i) */
            signvalue = '+';
        } else {
            if (flags & DP_F_SPACE)
                signvalue = ' ';
        }
    }

#if 0
    if (flags & DP_F_UP) caps = 1; /* Should characters be upper case? */
#endif

#if 0
    if (max == 0) ufvalue += 0.5; /* if max = 0 we must round */
#endif

    /*
    * Sorry, we only support 16 digits past the decimal because of our
    * conversion method
    */
    if (max > 16)
        max = 16;

    /* We "cheat" by converting the fractional part to integer by
    * multiplying by a factor of 10
    */

    temp = ufvalue;
    my_modf(temp, &intpart);

    fracpart = ROUND((POW10(max)) * (ufvalue - intpart));

    if (fracpart >= POW10(max)) {
        intpart++;
        fracpart -= POW10(max);
    }


    /* Convert integer part */
    do {
        temp = intpart;
        my_modf(intpart*0.1, &intpart);
        temp = temp*0.1;
        index = (int) ((temp -intpart +0.05)* 10.0);
        /* index = (int) (((double)(temp*0.1) -intpart +0.05) *10.0); */
        /* printf ("%llf, %f, %x\n", temp, intpart, index); */
        iconvert[iplace++] =
            (caps? "0123456789ABCDEF":"0123456789abcdef")[index];
    } while (intpart && (iplace < 311));
    if (iplace == 311) iplace--;
    iconvert[iplace] = 0;

    /* Convert fractional part */
    if (fracpart) {
        do {
            temp = fracpart;
            my_modf(fracpart*0.1, &fracpart);
            temp = temp*0.1;
            index = (int) ((temp -fracpart +0.05)* 10.0);
            /* index = (int) ((((temp/10) -fracpart) +0.05) *10); */
            /* printf ("%lf, %lf, %ld\n", temp, fracpart, index); */
            fconvert[fplace++] =
                (caps? "0123456789ABCDEF":"0123456789abcdef")[index];
        } while(fracpart && (fplace < 311));
        if (fplace == 311) fplace--;
    }
    fconvert[fplace] = 0;

    /* -1 for decimal point, another -1 if we are printing a sign */
    padlen = min - iplace - max - 1 - ((signvalue) ? 1 : 0);
    zpadlen = max - fplace;
    if (zpadlen < 0) zpadlen = 0;
    if (padlen < 0)
        padlen = 0;
    if (flags & DP_F_MINUS)
        padlen = -padlen; /* Left Justifty */

    if ((flags & DP_F_ZERO) && (padlen > 0)) {
        if (signvalue) {
            dopr_outch (buffer, currlen, maxlen, signvalue);
            --padlen;
            signvalue = 0;
        }
        while (padlen > 0) {
            dopr_outch (buffer, currlen, maxlen, '0');
            --padlen;
        }
    }
    while (padlen > 0) {
        dopr_outch (buffer, currlen, maxlen, ' ');
        --padlen;
    }
    if (signvalue)
        dopr_outch (buffer, currlen, maxlen, signvalue);

    while (iplace > 0)
        dopr_outch (buffer, currlen, maxlen, iconvert[--iplace]);

#ifdef DEBUG_SNPRINTF
    printf("fmtfp: fplace=%d zpadlen=%d\n", fplace, zpadlen);
#endif

    /*
    * Decimal point. This should probably use locale to find the correct
    * char to print out.
    */
    if (max > 0) {
        dopr_outch (buffer, currlen, maxlen, '.');

        while (fplace > 0)
            dopr_outch (buffer, currlen, maxlen, fconvert[--fplace]);
    }

    while (zpadlen > 0) {
        dopr_outch (buffer, currlen, maxlen, '0');
        --zpadlen;
    }

    while (padlen < 0) {
        dopr_outch (buffer, currlen, maxlen, ' ');
        ++padlen;
    }
}

static void fmtstr(char *buffer, size_t *currlen, size_t maxlen,
                   char *value, int flags, int min, int max) {
    int padlen, strln; /* amount to pad */
    int cnt = 0;

#ifdef DEBUG_SNPRINTF
    printf("fmtstr min=%d max=%d s=[%s]\n", min, max, value);
#endif
    if (value == 0) {
        value = "<NULL>";
    }

    for (strln = 0; value[strln]; ++strln); /* strlen */
    padlen = min - strln;
    if (padlen < 0)
        padlen = 0;
    if (flags & DP_F_MINUS)
        padlen = -padlen; /* Left Justify */

    while ((padlen > 0) && (cnt < max)) {
        dopr_outch (buffer, currlen, maxlen, ' ');
        --padlen;
        ++cnt;
    }
    while (*value && (cnt < max)) {
        dopr_outch (buffer, currlen, maxlen, *value++);
        ++cnt;
    }
    while ((padlen < 0) && (cnt < max)) {
        dopr_outch (buffer, currlen, maxlen, ' ');
        ++padlen;
        ++cnt;
    }
}

static size_t dopr(char *buffer, size_t maxlen, const char *format, va_list args) {
    char ch;
    long long value;
    long double fvalue;
    char *strvalue;
    int min;
    int max;
    int state;
    int flags;
    int cflags;
    size_t currlen;

    state = DP_S_DEFAULT;
    currlen = flags = cflags = min = 0;
    max = -1;
    ch = *format++;

    while (state != DP_S_DONE) {
        if (ch == '\0')
            state = DP_S_DONE;

        switch(state) {
        case DP_S_DEFAULT:
            if (ch == '%')
                state = DP_S_FLAGS;
            else
                dopr_outch (buffer, &currlen, maxlen, ch);
            ch = *format++;
            break;
        case DP_S_FLAGS:
            switch (ch) {
            case '-':
                flags |= DP_F_MINUS;
                ch = *format++;
                break;
            case '+':
                flags |= DP_F_PLUS;
                ch = *format++;
                break;
            case ' ':
                flags |= DP_F_SPACE;
                ch = *format++;
                break;
            case '#':
                flags |= DP_F_NUM;
                ch = *format++;
                break;
            case '0':
                flags |= DP_F_ZERO;
                ch = *format++;
                break;
            default:
                state = DP_S_MIN;
                break;
            }
            break;
        case DP_S_MIN:
            if (isdigit((unsigned char)ch)) {
                min = 10*min + char_to_int (ch);
                ch = *format++;
            } else if (ch == '*') {
                min = va_arg (args, int);
                ch = *format++;
                state = DP_S_DOT;
            } else {
                state = DP_S_DOT;
            }
            break;
        case DP_S_DOT:
            if (ch == '.') {
                state = DP_S_MAX;
                ch = *format++;
            } else {
                state = DP_S_MOD;
            }
            break;
        case DP_S_MAX:
            if (isdigit((unsigned char)ch)) {
                if (max < 0)
                    max = 0;
                max = 10*max + char_to_int (ch);
                ch = *format++;
            } else if (ch == '*') {
                max = va_arg (args, int);
                ch = *format++;
                state = DP_S_MOD;
            } else {
                state = DP_S_MOD;
            }
            break;
        case DP_S_MOD:
            switch (ch) {
            case 'h':
                cflags = DP_C_SHORT;
                ch = *format++;
                break;
            case 'l':
                cflags = DP_C_LONG;
                ch = *format++;
                if (ch == 'l') { // It's a long long
                    cflags = DP_C_LLONG;
                    ch = *format++;
                }
                break;
            case 'L':
                cflags = DP_C_LDOUBLE;
                ch = *format++;
                break;
            default:
                break;
            }
            state = DP_S_CONV;
            break;
        case DP_S_CONV:
            switch (ch) {
            case 'd':
            case 'i':
                if (cflags == DP_C_SHORT)
                    value = va_arg (args, int);
                else if (cflags == DP_C_LONG)
                    value = va_arg (args, long int);
                else if (cflags == DP_C_LLONG)
                    value = va_arg (args, long long);
                else
                    value = va_arg (args, int);
                fmtint (buffer, &currlen, maxlen, value, 10, min, max, flags);
                break;
            case 'o':
                flags |= DP_F_UNSIGNED;
                if (cflags == DP_C_SHORT)
                    value = va_arg (args, unsigned int);
                else if (cflags == DP_C_LONG)
                    value = (long)va_arg (args, unsigned long int);
                else if (cflags == DP_C_LLONG)
                    value = (long)va_arg (args, unsigned long long);
                else
                    value = (long)va_arg (args, unsigned int);
                fmtint (buffer, &currlen, maxlen, value, 8, min, max, flags);
                break;
            case 'u':
                flags |= DP_F_UNSIGNED;
                if (cflags == DP_C_SHORT)
                    value = va_arg (args, unsigned int);
                else if (cflags == DP_C_LONG)
                    value = (long)va_arg (args, unsigned long int);
                else if (cflags == DP_C_LLONG)
                    value = (long long)va_arg (args, unsigned long long);
                else
                    value = (long)va_arg (args, unsigned int);
                fmtint (buffer, &currlen, maxlen, value, 10, min, max, flags);
                break;
            case 'X':
                flags |= DP_F_UP;
            case 'x':
                flags |= DP_F_UNSIGNED;
                if (cflags == DP_C_SHORT)
                    value = va_arg (args, unsigned int);
                else if (cflags == DP_C_LONG)
                    value = (long)va_arg (args, unsigned long int);
                else if (cflags == DP_C_LLONG)
                    value = (long long)va_arg (args, unsigned long long);
                else
                    value = (long)va_arg (args, unsigned int);
                fmtint (buffer, &currlen, maxlen, value, 16, min, max, flags);
                break;
            case 'f':
                if (cflags == DP_C_LDOUBLE)
                    fvalue = va_arg (args, long double);
                else
                    fvalue = va_arg (args, double);
                // um, floating point?
                fmtfp (buffer, &currlen, maxlen, fvalue, min, max, flags);
                break;
            case 'E':
                flags |= DP_F_UP;
            case 'e':
                if (cflags == DP_C_LDOUBLE)
                    fvalue = va_arg (args, long double);
                else
                    fvalue = va_arg (args, double);
                break;
            case 'G':
                flags |= DP_F_UP;
            case 'g':
                if (cflags == DP_C_LDOUBLE)
                    fvalue = va_arg (args, long double);
                else
                    fvalue = va_arg (args, double);
                break;
            case 'c':
                dopr_outch (buffer, &currlen, maxlen, va_arg (args, int));
                break;
            case 's':
                strvalue = va_arg (args, char *);
                if (max == -1) {
                    max = strlen(strvalue);
                }
                if (min > 0 && max >= 0 && min > max) max = min;
                fmtstr (buffer, &currlen, maxlen, strvalue, flags, min, max);
                break;
            case 'p':
                strvalue = va_arg (args, void *);
                fmtint (buffer, &currlen, maxlen, (long) strvalue, 16, min, max, flags);
                break;
            case 'n':
                if (cflags == DP_C_SHORT) {
                    short int *num;
                    num = va_arg (args, short int *);
                    *num = currlen;
                } else if (cflags == DP_C_LONG) {
                    long int *num;
                    num = va_arg (args, long int *);
                    *num = (long int)currlen;
                } else if (cflags == DP_C_LLONG) {
                    long long *num;
                    num = va_arg (args, long long *);
                    *num = (long long)currlen;
                } else {
                    int *num;
                    num = va_arg (args, int *);
                    *num = currlen;
                }
                break;
            case '%':
                dopr_outch (buffer, &currlen, maxlen, ch);
                break;
            case 'w':
                // not supported yet, treat as next char
                ch = *format++;
                break;
            default:
                // Unknown, skip
                break;
            }
            ch = *format++;
            state = DP_S_DEFAULT;
            flags = cflags = min = 0;
            max = -1;
            break;
        case DP_S_DONE:
            break;
        default:
            // hmm?
            break; // some picky compilers need this
        }
    }
    if (maxlen != 0) {
        if (currlen < maxlen - 1)
            buffer[currlen] = '\0';
        else if (maxlen > 0)
            buffer[maxlen - 1] = '\0';
    }

    return currlen;
}

int snprintf(char *str,size_t count,const char *fmt,...) {
    size_t ret;
    va_list ap;

    va_start(ap, fmt);
    ret = dopr(str, count, fmt, ap);
    va_end(ap);
    return ret;
}
