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

# Enables the use of FPU on Cortex-M4.
# Enable this if you really want to use the STM FWLib.
ifeq ($(USE_FPU),)
  USE_FPU = no
endif

# Enable this if you really want to use the STM FWLib.
ifeq ($(USE_FWLIB),)
  USE_FWLIB = no
endif

#
# Architecture or project specific options
##############################################################################

##############################################################################
# Project, sources and paths
#
# Imported source files and paths
CHIBIOS = /usr/src/chibios

EXTENSION=../Extension

include $(CHIBIOS)/boards/ST_STM32F4_DISCOVERY/board.mk
include $(CHIBIOS)/os/hal/platforms/STM32F4xx/platform.mk
include $(CHIBIOS)/os/hal/hal.mk
include $(CHIBIOS)/os/ports/GCC/ARMCMx/STM32F4xx/port.mk
include $(CHIBIOS)/os/kernel/kernel.mk

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

# Define linker script file here
LDSCRIPT= $(PORTLD)/STM32F407xG.ld
#LDSCRIPT= $(PORTLD)/STM32F407xG_CCM.ld

CSRC = $(PORTSRC) \
       $(KERNSRC) \
       $(HALSRC) \
       $(PLATFORMSRC) \
       $(BOARDSRC) \
       $(CHIBIOS)/os/various/shell.c \
       $(CHIBIOS)/os/various/chprintf.c \
       $(PROJECTSRC)

ifeq ($(USE_UTIL),y)
CSRC+=$(EXTENSION_UTIL_SRC)
endif

ifeq ($(USE_CONFIG),y)
CSRC+=$(FLASH_CONFIG_SRC)
endif

ifeq ($(USE_MODBUS),y)
CSRC+=$(SERIAL_PACKET_SRC) $(MODBUS_SRC)
endif

# List ASM source files here
ASMSRC = $(PORTASM)

INCDIR = $(PORTINC) $(KERNINC) $(TESTINC) \
         $(HALINC) $(PLATFORMINC) $(BOARDINC) \
         $(CHIBIOS)/os/various

ifeq ($(USE_UTIL),y)
INCDIR+= $(EXTENSION_UTIL_INC)
endif

ifeq ($(USE_CONFIG),y)
INCDIR+= $(FLASH_CONFIG_INC)
endif

ifeq ($(USE_MODBUS),y)
INCDIR+= $(SERIAL_PACKET_INC) $(MODBUS_INC)
endif

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
LD   = $(TRGT)gcc
#LD   = $(TRGT)g++
CP   = $(TRGT)objcopy
AS   = $(TRGT)gcc -x assembler-with-cpp
OD   = $(TRGT)objdump
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

#
# End of user defines
##############################################################################

ifeq ($(USE_FPU),yes)
  USE_OPT += -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -fsingle-precision-constant
  DDEFS += -DCORTEX_USE_FPU=TRUE
else
  DDEFS += -DCORTEX_USE_FPU=FALSE
endif

ifeq ($(USE_FWLIB),yes)
  include $(CHIBIOS)/ext/stm32lib/stm32lib.mk
  CSRC += $(STM32SRC)
  INCDIR += $(STM32INC)
  USE_OPT += -DUSE_STDPERIPH_DRIVER
endif

include $(CHIBIOS)/os/ports/GCC/ARMCMx/rules.mk
