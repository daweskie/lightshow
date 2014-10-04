# make setting for ChibiOS


##############################################################################
# Build global options
# NOTE: Can be overridden externally.
#

# Compiler options here.
ifeq ($(USE_OPT),)
  USE_OPT = -O2 -ggdb -fomit-frame-pointer -falign-functions=16
endif

# C specific options here (added to USE_OPT).
ifeq ($(USE_COPT),)
  USE_COPT =
endif

# C++ specific options here (added to USE_OPT).
ifeq ($(USE_CPPOPT),)
  USE_CPPOPT = -fno-rtti
endif

# Enable this if you want the linker to remove unused code and data
ifeq ($(USE_LINK_GC),)
  USE_LINK_GC = yes
endif

# Linker extra options here.
ifeq ($(USE_LDOPT),)
  USE_LDOPT =
endif

# Enable this if you want link time optimizations (LTO)
ifeq ($(USE_LTO),)
  USE_LTO = no
endif

# If enabled, this option allows to compile the application in THUMB mode.
ifeq ($(USE_THUMB),)
  USE_THUMB = yes
endif

# Enable this if you want to see the full log while compiling.
ifeq ($(USE_VERBOSE_COMPILE),)
  USE_VERBOSE_COMPILE = no
endif

#
# Build global options
##############################################################################

##############################################################################
# Architecture or project specific options
#

# Stack size to be allocated to the Cortex-M process stack. This stack is
# the stack used by the main() thread.
ifeq ($(USE_PROCESS_STACKSIZE),)
  USE_PROCESS_STACKSIZE = 0x400
endif

# Stack size to the allocated to the Cortex-M main/exceptions stack. This
# stack is used for processing interrupts and exceptions.
ifeq ($(USE_EXCEPTIONS_STACKSIZE),)
  USE_EXCEPTIONS_STACKSIZE = 0x400
endif

# Enables the use of FPU on Cortex-M4.
# Enable this if you really want to use the STM FWLib.
ifeq ($(USE_FPU),)
  USE_FPU = no
endif

#
# Architecture or project specific options
##############################################################################

##############################################################################
# Project, sources and paths
#
# Imported source files and paths
CHIBIOS = /usr/src/chibios

EXTENSION_PREFIX ?= ..
EXTENSION =$(EXTENSION_PREFIX)/Extension

BOARD ?= ST_STM32F4_DISCOVERY

include $(CHIBIOS)/os/hal/hal.mk
include $(CHIBIOS)/os/hal/boards/$(BOARD)/board.mk
include $(CHIBIOS)/os/hal/ports/STM32/STM32F4xx/platform.mk
include $(CHIBIOS)/os/hal/osal/rt/osal.mk
include $(CHIBIOS)/os/rt/rt.mk
include $(CHIBIOS)/os/rt/ports/ARMCMx/compilers/GCC/mk/port_stm32f4xx.mk
include $(CHIBIOS)/test/rt/test.mk


# Define linker script file here
LDSCRIPT= $(PORTLD)/STM32F407xG.ld
#LDSCRIPT= $(PORTLD)/STM32F407xG_CCM.ld

CSRC = $(PORTSRC) \
       $(KERNSRC) \
       $(TESTSRC) \
       $(HALSRC) \
       $(OSALSRC) \
       $(PLATFORMSRC) \
       $(BOARDSRC) \
       $(CHIBIOS)/os/various/shell.c \
       $(CHIBIOS)/os/various/chprintf.c \
       $(PROJECTSRC)

INCDIR = $(PORTINC) $(KERNINC) $(TESTINC) \
         $(HALINC) $(OSALINC) $(PLATFORMINC) $(BOARDINC) \
         $(CHIBIOS)/os/various

ifeq ($(USE_MOTOROLA_PACKET),y)
USE_UTIL=y
USE_CONFIG=y
USE_SERIAL_PACKET=y
include $(EXTENSION)/motorolapacket/motorolapacket.mk
endif

ifeq ($(USE_MODBUS),y)
USE_UTIL=y
USE_CONFIG=y
USE_SERIAL_PACKET=y
include $(EXTENSION)/modbus/modbus.mk
endif

ifeq ($(USE_SERIAL_PACKET),y)
include $(EXTENSION)/serialpacket/serialpacket.mk
endif

ifeq ($(USE_CONFIG),y)
USE_UTIL=y
include $(EXTENSION)/flashconfig/flashconfig.mk
endif

ifeq ($(USE_UTIL),y)
include $(EXTENSION)/util/util.mk
endif

ifeq ($(USE_ACCEL),y)
CSRC+=$(CHIBIOS)/os/various/devices_lib/accel/lis302dl.c
INCDIR+=$(CHIBIOS)/os/various/devices_lib/accel
endif

#ifeq ($(USE_MODBUS),y)
#CSRC+=$(SERIAL_PACKET_SRC) $(MODBUS_SRC)
#endif

# C++ sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CPPSRC =

# C sources to be compiled in ARM mode regardless of the global setting.
# NOTE: Mixing ARM and THUMB mode enables the -mthumb-interwork compiler
#       option that results in lower performance and larger code size.
ACSRC =

# C++ sources to be compiled in ARM mode regardless of the global setting.
# NOTE: Mixing ARM and THUMB mode enables the -mthumb-interwork compiler
#       option that results in lower performance and larger code size.
ACPPSRC =

# C sources to be compiled in THUMB mode regardless of the global setting.
# NOTE: Mixing ARM and THUMB mode enables the -mthumb-interwork compiler
#       option that results in lower performance and larger code size.
TCSRC =

# C sources to be compiled in THUMB mode regardless of the global setting.
# NOTE: Mixing ARM and THUMB mode enables the -mthumb-interwork compiler
#       option that results in lower performance and larger code size.
TCPPSRC =

# List ASM source files here
ASMSRC = $(PORTASM)

#
# Project, sources and paths
##############################################################################

##############################################################################
# Compiler settings
#

MCU  ?= cortex-m4
TRGT ?= arm-none-eabi-

CC   = $(TRGT)gcc
CPPC = $(TRGT)g++
# Enable loading with g++ only if you need C++ runtime support.
# NOTE: You can use C++ even without C++ support if you are careful. C++
#       runtime support makes code size explode.
#LD   = $(TRGT)gcc
LD   = $(TRGT)g++

CP   = $(TRGT)objcopy
AS   = $(TRGT)gcc -x assembler-with-cpp
AR   = $(TRGT)ar
OD   = $(TRGT)objdump
SZ   = $(TRGT)size
HEX  = $(CP) -O ihex
BIN  = $(CP) -O binary

# ARM-specific options here
AOPT =

# THUMB-specific options here
TOPT = -mthumb -DTHUMB

# Define C warning options here
CWARN = -Wall -Wextra -Wstrict-prototypes
# Define C++ warning options here
CPPWARN = -Wall -Wextra


# Compiler settings
##############################################################################

##############################################################################
# Start of user section
#

# List all user C define here, like -D_DEBUG=1
UDEFS =

# Define ASM defines here
UADEFS =

# List all user directories here
UINCDIR =

# List the user directory to look for the libraries here
ULIBDIR =

# List all user libraries here
ULIBS =
#
# End of user defines
##############################################################################

RULESPATH = $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC
include $(RULESPATH)/rules.mk

