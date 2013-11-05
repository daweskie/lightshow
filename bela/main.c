#include <stdlib.h>



#include <ch.h>
#include <hal.h>
#include <shell.h>
#include <chprintf.h>

#include "bela.h"
#include "usbcfg.h"
#include "misc.h"
#include "adconv.h"
#include "Flash.h"
#include "configflash.h"
#include "belapwm.h"
#include "comm.h"
#include <serialpacket.h>
#include <modbus.h>

SerialUSBDriver SDU1;

static const ShellCommand commands[] = {
    {"mem", cmd_mem},
    {"threads", cmd_threads},
    {"showcfg",cmd_show_config},
    {"savecfg",cmd_save_config},
    {"erasecfg",cmd_erase_config_flash},
    {"blinkspeed", cmd_blinkspeed},
    {"dump",cmd_dump},
    {"bs", cmd_blinkspeed},
    {"ad", cmd_ad},
    {"showadparams", cmd_showChannelParams},
    {"pwm", cmd_pwm},
    _base_serial_packet_commands
    _modbus_commands
    {NULL,NULL}
};

#define SHELL_WA_SIZE THD_WA_SIZE(2048)

extern SerialUSBDriver SDU1;

static const ShellConfig shell_cfg1 = {
    (BaseSequentialStream *) &SDU1,
    commands
};

void connectConsole(void) {
    shellInit();
    sduObjectInit(&SDU1);
    sduStart(&SDU1, &serusbcfg);
    usbDisconnectBus(serusbcfg.usbp);
    chThdSleepMilliseconds(1000);
    usbStart(serusbcfg.usbp, &usbcfg);
    usbConnectBus(serusbcfg.usbp);
}

/**
    Application entry point
*/
int main(void)
{
    Thread *shelltp=NULL;

    halInit();
    chSysInit();
    connectConsole();
    cfgInit(BELA_FLASH_CONFIG_SECTOR);
    miscInit();


    adConvInit();
    pwmInitilaize();
    commInit();

    while(TRUE) {
        if (!shelltp) {
            if(SDU1.config->usbp->state==USB_ACTIVE)
                shelltp = shellCreate(&shell_cfg1, SHELL_WA_SIZE, NORMALPRIO);
        }
        else {
            if(chThdTerminated(shelltp)) {
                chThdRelease(shelltp);
                shelltp=NULL;
            }
        }
        chThdSleepMilliseconds(500);
    }
    return 0;
}
