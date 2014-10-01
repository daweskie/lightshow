#Source files for Motorola packet module
EXTENSION_UTIL_SRC = $(EXTENSION)/util/src/checksum.c \
    $(EXTENSION)/util/src/cmdutil.c \
    $(EXTENSION)/util/src/snprintf.c \
    $(EXTENSION)/util/src/stringbuilder.c


#include directories
EXTENSION_UTIL_INC = $(EXTENSION)/util/include

CSRC+=$(EXTENSION_UTIL_SRC)
INCDIR+=$(EXTENSION_UTIL_INC)


