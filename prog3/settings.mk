# make setting for ZLIB

# Default c compiler flag is warnings on
CFLAGS ?=-Wall

# the place of binary, defaults is bin
BIN_ROOT ?=bin

#the place of objects, default is obj
OBJ_ROOT ?=obj

#the place of sources, default is src
SRC ?=src

#default architecture is x86
ARCH ?=x86

#Frescale's imx6 based wandboard architecture
ifeq ($(ARCH),wand)
CFLAG +=-mthumb -O3 -march=armv7-a -mcpu=cortex-a9 -mtune=cortex-a9 -mfpu=neon -mvectorize-with-neon-quad -mfloat-abi=softfp
CC=arm-linux-gnueabi-gcc
endif

# if defined the debug add -g to c compiler flags
ifeq ($(DEBUG),y)
CFLAGS+=-g
endif

#if wants to use socket module
ifeq ($(USE_SOCKET_SERVER),y)
LIB_PATH+=-L $(ZLIB)/Socketserver/lib/$(ARCH)
LIB+=-l pthread -l Socketserver
INCLUDE_DIR+=-I $(ZLIB)/Socketserver/include
USE_CONFIG=y
USE_UTIL=y
endif

#if wants to use socket module
ifeq ($(USE_SOCKET_CLIENT),y)
LIB_PATH+=-L $(ZLIB)/socketclient/lib/$(ARCH)
LIB+=-l pthread -l socketclient
INCLUDE_DIR+=-I $(ZLIB)/socketclient/include
USE_CONFIG=y
USE_UTIL=y
endif


#if wants to use serial module
ifeq ($(USE_SERIAL),y)
LIB_PATH+=-L $(ZLIB)/serial/lib/$(ARCH)
LIB+=-l pthread -l serial
INCLUDE_DIR+=-I $(ZLIB)/serial/include
USE_CONFIG=y
USE_CRC=y
USE_UTIL=y
endif

#if wants to use serial module
ifeq ($(USE_SERIAL_PACKET),y)
LIB_PATH+=-L $(ZLIB)/serialPacket/lib/$(ARCH)
LIB+=-l pthread -l serialPacket
INCLUDE_DIR+=-I $(ZLIB)/serialPacket/include
USE_CONFIG=y
USE_UTIL=y
endif


#if wants to use task
ifeq ($(USE_TASK),y)
LIB_PATH+=-L $(ZLIB)/task/lib/$(ARCH)
LIB+=-l task -l pthread
INCLUDE_DIR+=-I $(ZLIB)/task/include
USE_UTIL=y
USE_CONFIG=y
endif

#if wants to use crc module
ifeq ($(USE_CRC),y)
LIB_PATH+=-L $(ZLIB)/checksum/lib/$(ARCH)
LIB+=-l checksum
INCLUDE_DIR+=-I $(ZLIB)/checksum/include
USE_UTIL=y
endif

#if wants to use config module
ifeq ($(USE_CONFIG),y)
LIB_PATH+=-L $(ZLIB)/config/lib/$(ARCH)
LIB+=-l config
INCLUDE_DIR+=-I $(ZLIB)/config/include
USE_STR=y
USE_UTIL=y
endif

#if wants to use str module
ifeq ($(USE_STR),y)
LIB_PATH+=-L $(ZLIB)/str/lib/$(ARCH)
LIB+=-l str
INCLUDE_DIR+=-I $(ZLIB)/str/include
USE_UTIL=y
endif

#if wants to use util module
ifeq ($(USE_UTIL),y)
LIB_PATH+=-L $(ZLIB)/util/lib/$(ARCH)
LIB+=-l util
INCLUDE_DIR+=-I $(ZLIB)/util/include
endif
