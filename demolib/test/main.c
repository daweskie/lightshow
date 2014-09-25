#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#include <demolib.h>
#include <config.h>

int test_demolib()
{
    printf("Demolib test starting...");
    assert(cfgInit("demolib.cfg")==EXIT_SUCCESS);
    struct DemoLib *demolib = dlCreateCfg("demolib");
    assert(demolib);

    assert(dlStart(demolib)==EXIT_SUCCESS);
    assert(!dlIsPaused(demolib));
    assert(!dlIsStopped(demolib));

    int i=0;
    for(;i<10;++i) {
        printf("%d. element is:%d\n",i,dlGetValue(demolib));
        sleep(1);
    }

    dlPause(demolib);

    dlStop(demolib);
    assert(dlDestroy(demolib)==EXIT_SUCCESS);
    assert(cfgClose()==EXIT_SUCCESS);
    printf("..passed");
    return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
    printf("demolib testing started\n");
    int result = test_demolib(argc, argv);
    printf("demolib testing ended\n");
    return result;
}
