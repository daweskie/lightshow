##############################################################################
# Build global options
# NOTE: Can be overridden externally.
#

# Architecture or project specific options
##############################################################################
USE_UTIL=y
USE_CONFIG=y
USE_MODBUS=n
USE_SERIAL_PACKET=n
USE_MOTOROLA_PACKET=n
USE_MODBUS=n

##############################################################################
# Project, sources and paths
#

# Define project name here
PROJECT = bela
PROJECTSRC=$(patsubst %.c,%.o,$(wildcard *.c))

# List all default C defines here, like -D_DEBUG=1
DDEFS =

# List all default ASM defines here, like -D_DEBUG=1
DADEFS =

# List all default directories to look for include files here
DINCDIR =

# List the default directory to look for the libraries here
DLIBDIR =

# List all default libraries here
DLIBS =

#
# End of default section
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

include ../settings.mk

$(info ======================================================)
$(info  Include dir:$(INCDIR))
$(info ======================================================)
$(info  Asm src:$(ASMSRC))
$(info ======================================================)
$(info C src:$(CSRC))
$(info ======================================================)
$(info Project src:$(PROJECTSRC))
$(info ======================================================)

include ../targets.mk
include ../help.mk

