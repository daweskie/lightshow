#Source files for Motorola packet module
FLASH_CONFIG_SRC = $(EXTENSION)/flashconfig/src/configflash.c \
		$(EXTENSION)/flashconfig/src/Flash.c

#include directories
FLASH_CONFIG_INC = $(EXTENSION)/flashconfig/include

CSRC+=$(FLASH_CONFIG_SRC)
INCDIR+=$(FLASH_CONFIG_INC)