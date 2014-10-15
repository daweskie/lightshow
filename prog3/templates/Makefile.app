# Make for C compiling

ZLIB=../

DEBUG=y

ARCH=x86


USE_UTIL=n
USE_STR=n

SRC=src

include $(ZLIB)/settings.mk

OBJS=$(patsubst $(SRC)/%.c,$(OBJ_ROOT)/$(ARCH)/%.o,$(wildcard $(SRC)/*.c))
BIN_NAME=$(BIN_ROOT)/$(ARCH)/#!#APP_NAME#!#


$(info ==============================================)
$(info arch:$(ARCH))
$(info objs:$(OBJS))
$(info bin_root:$(BIN_ROOT))
$(info obj_root:$(OBJ_ROOT))
$(info bin_name:$(BIN_NAME))
$(info include:$(INCLUDE_DIR))
$(info libpath:$(LIB_PATH))
$(info lib: $(LIB))
$(info ==============================================)

$(BIN_NAME): $(OBJ_ROOT)/$(ARCH) $(BIN_ROOT)/$(ARCH) $(OBJS)
	$(CC) -o$(BIN_NAME) $(OBJS) $(LIB_PATH) $(LIB) $(CFLAGS)

include $(ZLIB)/targets.mk

.PHONY:run
