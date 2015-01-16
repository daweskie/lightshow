#include <stdlib.h>



#include "ch.h"
#include "hal.h"
#include <shell.h>
#include <chprintf.h>


#include "usbcfg.h"
#include "misc.h"
#include "config.h"
#include "shellmenu.h"



/*
 * Shell configuration
 */

#define SHELL_WA_SIZE	 THD_WA_SIZE(2048)




/**
    Application entry point
*/
int main(void)
{
    //
    /*
     * Shell thread
     */
    Thread *shelltp = NULL;

    /*
     * System initializations.
     * - HAL initialization, this also initializes the configured device drivers
     *	 and performs the board-specific initializations.
     * - Kernel initialization, the main() function becomes a thread and the
     *	 RTOS is active.
     */
    halInit();
    chSysInit();

    /*
     * Initializes a serial-over-USB CDC driver.
     */
    sduObjectInit(&SDU1);
    sduStart(&SDU1, &serusbcfg);

    /*
     * Activates the USB driver and then the USB bus pull-up on D+.
     * Note, a delay is inserted in order to not have to disconnect the cable
     * after a reset.
     */
    usbDisconnectBus(serusbcfg.usbp);
    chThdSleepMilliseconds(2000);
    usbStart(serusbcfg.usbp, &usbcfg);
    usbConnectBus(serusbcfg.usbp);

    initPWM();
    buttonInit();
    startPWMThread();


    /*
     * Main loop, does nothing except
     */

    while (TRUE)
    {

        /* spawn a shell when the old one was terminated */
        if (!shelltp)
        {
            if (SDU1.config->usbp->state == USB_ACTIVE)
            {
                /* Spawns a new shell.*/
                shelltp = shellCreate(&shell_cfg1, SHELL_WA_SIZE, NORMALPRIO-10);
            }
        }
        else
        {
            /* If the previous shell exited.*/
            if (chThdTerminated(shelltp))
            {
                /* Recovers memory of the previous shell.*/
                chThdRelease(shelltp);
                shelltp = NULL;
            }
        }

        chThdSleepMilliseconds(50);

    }
}
