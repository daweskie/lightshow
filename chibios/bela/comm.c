/*
 * Copyright (C)  2011 Zoltan Zidarics (Zamek)
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/** \file
    \brief
*/

#include <sys/queue.h>
#include <chtypes.h>

#include "ch.h"
#include "hal.h"
#include "bela.h"
#include "comm.h"
#include <macros.h>
#include <checksum.h>
#include <modbus.h>
#include <endian.h>

int q=0;

static SerialConfig ser_cfg={
    BELA_COMM_BAUD,
    0,
    0,
    0
};


static ModbusDriver MODD1;

static uint16_t modbusRegisters[BELA_MODBUS_MAX_REGISTERS];

void cmd_serial_test(BaseSequentialStream *chp, int argc, char *argv[]) {
    (void) chp;
    (void) argc;
    (void) argv;
}

void commInit(void) {

    int i=0;
    for (;i<BELA_MODBUS_MAX_REGISTERS;i++)
        modbusRegisters[i] = htobe16(42-i);

    modbusObjectInit(&MODD1, "mod0", &SD2, BELA_COMM_MAX_PACKETS,
                       BELA_MODBUS_ADDRESS,
                       modbusRegisters, BELA_MODBUS_MAX_REGISTERS, MODBUS_REGISTER_BASE);

    spStart(&MODD1, &ser_cfg);

    palSetPadMode(GPIOA, 2, PAL_MODE_ALTERNATE(7));
    palSetPadMode(GPIOA, 3, PAL_MODE_ALTERNATE(7));
}

