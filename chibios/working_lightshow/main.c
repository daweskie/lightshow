/*
		ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
								 2011,2012 Giovanni Di Sirio.

		This file is part of ChibiOS/RT.

		ChibiOS/RT is free software; you can redistribute it and/or modify
		it under the terms of the GNU General Public License as published by
		the Free Software Foundation; either version 3 of the License, or
		(at your option) any later version.

		ChibiOS/RT is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
		GNU General Public License for more details.

		You should have received a copy of the GNU General Public License
		along with this program.	If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ch.h"
#include "hal.h"

#include "myMisc.h"
#include "buttonpush.h"
#include "chprintf.h"
#include "usbcfg.h"

#include "menu.h"
#include "config.h"
#include <chsem.h>



/*
 * Shell configuration
 */

#define SHELL_WA_SIZE	 THD_WA_SIZE(2048)





/*
 * Application entry point.
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

    chBSemInit(&my_bsem, FALSE); //Init semaphore

    startBlinker();
    startButton();

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
