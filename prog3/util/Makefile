# Make for C compiling

ZLIB=../

MODULE=util

DEBUG ?= y

BIN_ROOT=lib

include $(ZLIB)/settings.mk

INCLUDE_DIR+=-I ./include

OBJS=$(patsubst src/%.c,obj/$(ARCH)/%.o,$(wildcard src/*.c))

BIN_NAME=$(BIN_ROOT)/$(ARCH)/lib$(MODULE).a

$(info ==============================================)
$(info objs:$(OBJS))
$(info bin_root:$(BIN_ROOT))
$(info obj_root:$(OBJ_ROOT))
$(info bin_name:$(BIN_NAME))
$(info include:$(INCLUDE_DIR))
$(info ==============================================)

$(BIN_NAME):$(OBJ_ROOT)/$(ARCH) $(BIN_ROOT)/$(ARCH) $(OBJS)
	$(AR) rcs $(BIN_NAME) $(OBJS)

include $(ZLIB)/targets.mk


