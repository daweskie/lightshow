#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <numutils.h>


#define FLOAT_DIFF_LIMIT 0.001

#define FL_COMPARE_D(a,b,diff) (a-b)<=diff

#define FL_COMPARE(a,b) FL_COMPARE_D(a,b,FLOAT_DIFF_LIMIT)

#define MINIMAL_VERSION

void test_nu_str_toint() {
    assert(nu_str_toint(NULL)==0);
    assert(nu_str_toint("")==0);
    assert(nu_str_toint("123")==123);
    assert(nu_str_toint("123V5")==0);
    assert(nu_str_toint("123567891234567")==0);
#ifndef MINIMAL_VERSION
    assert(nu_str_toint("0x10")==16);
    assert(nu_str_toint("x10")==0);
    assert(nu_str_toint("0x10g")==0);
    assert(nu_str_toint("0b10")==2);
    assert(nu_str_toint("b10")==0);
    assert(nu_str_toint("0b102")==0);
    assert(nu_str_toint("0o10")==8);
    assert(nu_str_toint("o10")==0);
    assert(nu_str_toint("0o109")==0);
    assert(nu_str_toint("0X10")==16);
    assert(nu_str_toint("X10")==0);
    assert(nu_str_toint("0B10")==2);
    assert(nu_str_toint("B10")==0);
    assert(nu_str_toint("0O10")==8);
    assert(nu_str_toint("0f12")==0);
#endif // MINIMAL_VERSION
}

void test_nu_str_toint_default() {
    assert(nu_str_toint_default(NULL,-1)==-1);
    assert(nu_str_toint_default("",-1)==-1);
    assert(nu_str_toint_default("123",-1)==123);
    assert(nu_str_toint_default("123V5",-1)==-1);
    assert(nu_str_toint_default("123567891234567",-1)==-1);
#ifndef MINIMAL_VERSION
    assert(nu_str_toint_default("0x10",-1)==16);
    assert(nu_str_toint_default("x10",-1)==-1);
    assert(nu_str_toint_default("0b10",-1)==2);
    assert(nu_str_toint_default("b10",-1)==-1);
    assert(nu_str_toint_default("0o10",-1)==8);
    assert(nu_str_toint_default("o10",-1)==-1);
    assert(nu_str_toint_default("0X10",-1)==16);
    assert(nu_str_toint_default("X10",-1)==-1);
    assert(nu_str_toint_default("0B10",-1)==2);
    assert(nu_str_toint_default("B10",-1)==-1);
    assert(nu_str_toint_default("0O10",-1)==8);
    assert(nu_str_toint_default("0f12",-1)==-1);
#endif // MINIMAL_VERSION
}

void test_nu_str_tolong() {
    assert(nu_str_tolong(NULL)==0);
    assert(nu_str_tolong("")==0);
    assert(nu_str_tolong("123")==123);
    assert(nu_str_tolong("123V5")==0);
    assert(nu_str_tolong("12356789567")==0);
#ifndef MINIMAL_VERSION
    assert(nu_str_tolong("0x10")==16);
    assert(nu_str_tolong("x10")==0);
    assert(nu_str_tolong("0b10")==2);
    assert(nu_str_tolong("b10")==0);
    assert(nu_str_tolong("0o10")==2);
    assert(nu_str_tolong("o10")==0);
    assert(nu_str_tolong("0X10")==16);
    assert(nu_str_tolong("X10")==0);
    assert(nu_str_tolong("0B10")==2);
    assert(nu_str_tolong("B10")==0);
    assert(nu_str_tolong("0O10")==2);
    assert(nu_str_tolong("0f12")==0);
#endif // MINIMAL_VERSION
}

void test_nu_str_tolong_default() {
    assert(nu_str_tolong_default(NULL,-1)==-1);
    assert(nu_str_tolong_default("",-1)==-1);
    assert(nu_str_tolong_default("123",-1)==123);
    assert(nu_str_tolong_default("123V5",-1)==-1);
    assert(nu_str_tolong_default("12356789567",-1)==-1);
#ifndef MINIMAL_VERSION
    assert(nu_str_tolong_default("0x10",-1)==16);
    assert(nu_str_tolong_default("x10",-1)==-1);
    assert(nu_str_tolong_default("0b10",-1)==2);
    assert(nu_str_tolong_default("b10",-1)==-1);
    assert(nu_str_tolong_default("0o10",-1)==2);
    assert(nu_str_tolong_default("o10",-1)==-1);
    assert(nu_str_tolong_default("0X10",-1)==16);
    assert(nu_str_tolong_default("X10",-1)==0);
    assert(nu_str_tolong_default("0B10",-1)==2);
    assert(nu_str_tolong_default("B10",-1)==-1);
    assert(nu_str_tolong_default("0O10",-1)==2);
    assert(nu_str_tolong_default("0f12",-1)==-1);
#endif // MINIMAL_VERSION
}


void test_nu_str_tofloat() {
    assert(FL_COMPARE(nu_str_tofloat(NULL),0.0));
    assert(nu_str_tofloat("")==0.0);
    assert(nu_str_tofloat("123.12")==123.12);
    assert(nu_str_tofloat("123V5.0")==0);
    assert(nu_str_tofloat("123567.32")==123567.32);
}

void test_nu_str_tofloat_default() {
    assert(nu_str_tofloat_default(NULL,2.0)==2.0);
    assert(nu_str_tofloat_default("",2.0)==2.0);
    assert(nu_str_tofloat_default("123.12",2.0)==123.12);
    assert(nu_str_tofloat_default("123V5.0",2.0)==2.0);
    assert(nu_str_tofloat_default("123567.32",2.0)==123567.32);
}

void test_nu_str_tobyte() {
    assert(nu_str_tobyte(NULL)==0);
    assert(nu_str_tobyte("")==0);
    assert(nu_str_tobyte("1")==1);
    assert(nu_str_tobyte("327")==0);
#ifndef MINIMAL_VERSION
    assert(nu_str_tobyte("0x10")==16);
    assert(nu_str_tobyte("x10")==0);
    assert(nu_str_tobyte("0b10")==2);
    assert(nu_str_tobyte("b10")==0);
    assert(nu_str_tobyte("0o10")==8);
    assert(nu_str_tobyte("o10")==0);
    assert(nu_str_tobyte("0X10")==16);
    assert(nu_str_tobyte("X10")==0);
    assert(nu_str_tobyte("0B10")==2);
    assert(nu_str_tobyte("B10")==0);
    assert(nu_str_tobyte("0O10")==8);
    assert(nu_str_tobyte("0f12")==0);
#endif // MINIMAL_VERSION
}

void test_nu_str_tobyte_default() {
    assert(nu_str_tobyte_default(NULL,0)==0);
    assert(nu_str_tobyte_default("",0)==0);
    assert(nu_str_tobyte_default("1",0)==1);
    assert(nu_str_tobyte_default("327",0)==0);
#ifndef MINIMAL_VERSION
    assert(nu_str_tobyte_default("0x10",0)==16);
    assert(nu_str_tobyte_default("x10",0)==0);
    assert(nu_str_tobyte_default("0b10",0)==2);
    assert(nu_str_tobyte_default("b10",0)==0);
    assert(nu_str_tobyte_default("0o10",0)==8);
    assert(nu_str_tobyte_default("o10",0)==0);
    assert(nu_str_tobyte_default("0X10",0)==16);
    assert(nu_str_tobyte_default("X10",0)==0);
    assert(nu_str_tobyte_default("0B10",0)==2);
    assert(nu_str_tobyte_default("B10",0)==0);
    assert(nu_str_tobyte_default("0o10",0)==8);
    assert(nu_str_tobyte_default("0f12",0)==0);
#endif // MINIMAL_VERSION
}

void test_nu_str_toshort() {
    assert(nu_str_toshort(NULL)==0);
    assert(nu_str_toshort("")==0);
    assert(nu_str_toshort("32767")==32767);
    assert(nu_str_toshort("32768")==0);
#ifndef MINIMAL_VERSION
    assert(nu_str_toshort("0x10")==16);
    assert(nu_str_toshort("x10")==0);
    assert(nu_str_toshort("0b10")==2);
    assert(nu_str_toshort("b10")==0);
    assert(nu_str_toshort("0o10")==2);
    assert(nu_str_toshort("o10")==0);
    assert(nu_str_toshort("0X10")==16);
    assert(nu_str_toshort("X10")==0);
    assert(nu_str_toshort("0B10")==2);
    assert(nu_str_toshort("B10")==0);
    assert(nu_str_toshort("0O10")==2);
    assert(nu_str_toshort("0f12")==0);
#endif // MINIMAL_VERSION
}

void test_nu_str_toshort_default() {
    assert(nu_str_toshort_default(NULL,0)==0);
    assert(nu_str_toshort_default("",0)==0);
    assert(nu_str_toshort_default("32767",0)==32767);
    assert(nu_str_toshort_default("32768",0)==0);
#ifndef MINIMAL_VERSION
    assert(nu_str_toshort_default("0x10",0)==16);
    assert(nu_str_toshort_default("x10",0)==0);
    assert(nu_str_toshort_default("0b10",0)==2);
    assert(nu_str_toshort_default("b10",0)==0);
    assert(nu_str_toshort_default("0o10",0)==8);
    assert(nu_str_toshort_default("o10",0)==0);
    assert(nu_str_toshort_default("0X10",0)==16);
    assert(nu_str_toshort_default("X10",0)==0);
    assert(nu_str_toshort_default("0B10",0)==2);
    assert(nu_str_toshort_default("B10",0)==0);
    assert(nu_str_toshort_default("0o10",0)==8);
    assert(nu_str_toshort_default("0f12",0)==0);
#endif // MINIMAL_VERSION
}

void test_nu_is_all_zeros() {
    assert(nu_is_all_zeros("")!=0);
    assert(nu_is_all_zeros(NULL)!=0);
    assert(nu_is_all_zeros("123")==0);
    assert(nu_is_all_zeros("0123")==0);
    assert(nu_is_all_zeros("0")!=0);
    assert(nu_is_all_zeros("00")!=0);
    assert(nu_is_all_zeros("0000")!=0);
}

void test_min() {
    char ca=0,cb=1;
    assert(nu_min(ca,cb)==ca);
    ca=-1;
    cb=1;
    assert(nu_min(ca,cb)==ca);

    short sa=100,sb=101;
    assert(nu_min(sa,sb)==sa);
    sa=-100;
    sb=101;
    assert(nu_min(sa,sb)==sa);

    int ia=1000,ib=1001;
    assert(nu_min(ia,ib)==ia);
    ia=-1000;
    ib=1001;
    assert(nu_min(ia,ib)==ia);

    long la=1000000,lb=1000001;
    assert(nu_min(la,lb)==la);
    la=-1000000;
    lb=1000001;
    assert(nu_min(la,lb)==la);

    long *pa =&la;
    long *pb = &lb;

    assert(nu_min(*pa,*pb)==*pa);
}

void test_max() {
    char ca=0,cb=1;
    assert(nu_max(ca,cb)==cb);
    ca=-1;
    cb=1;
    assert(nu_max(ca,cb)==cb);

    short sa=100, sb=101;
    assert(nu_max(sa,sb)==sb);
    sa=-100;
    sb=101;
    assert(nu_max(sa,sb)==sb);

    int ia=1000,ib=1001;
    assert(nu_max(ia,ib)==ib);
    ia=-1000;
    ib=1001;
    assert(nu_max(ia,ib)==ib);

    long la=1000000,lb=1000001;
    assert(nu_max(la,lb)==lb);
    la=-1000000;
    lb=1000001;
    assert(nu_max(la,lb)==lb);

    long *pa =&la;
    long *pb = &lb;

    assert(nu_max(pa,pb)==pb);
}

void test_is_digit() {
    assert(nu_is_digit(NULL)==0);
    assert(nu_is_digit("")==0);
    assert(nu_is_digit("01234")!=0);
    assert(nu_is_digit("01234v567")==0);
    assert(nu_is_digit("-012345")!=0);
    assert(nu_is_digit("01-234")==0);
}

void test_is_number() {
    assert(nu_is_number(NULL)==0);
    assert(nu_is_number("")==0);
    assert(nu_is_number("01234")!=0);
    assert(nu_is_number("01234.0")!=0);
    assert(nu_is_number("-01234")!=0);
    assert(nu_is_number("-01234.0")!=0);
    assert(nu_is_number("-01234-")==0);
    assert(nu_is_number("-012.3.4")==0);
}

int test_numutils()
{
    test_nu_str_toint();
    test_nu_str_toint_default();
    test_nu_str_tolong();
    test_nu_str_tolong_default();
    test_nu_str_tofloat();
    test_nu_str_tofloat_default();
    test_nu_str_tobyte();
    test_nu_str_tobyte_default();
    test_nu_str_toshort();
    test_nu_str_toshort_default();
    test_nu_is_all_zeros();
    test_min();
    test_max();
    test_is_digit();
    test_is_number();
}

int main(int argc, char **argv)
{
    printf("numutils testing started\n");
    int result = test_numutils();
    printf("numutils testing ended\n");
    return result;
}
