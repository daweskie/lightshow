#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int test_#!#LIB_NAME#!#()
{
}

int main(int argc, char **argv)
{
    printf("#!#LIB_NAME#!# testing started\n");
    int result = test_#!#LIB_NAME#!#(argc, argv);
    printf("#!#LIB_NAME#!# testing ended\n");
    return result;
}
