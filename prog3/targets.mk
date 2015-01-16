#targets for ZLIB

$(BIN_ROOT)/$(ARCH):$(BIN_ROOT)
	mkdir $(BIN_ROOT)/$(ARCH)

$(BIN_ROOT):
	mkdir $(BIN_ROOT)

$(OBJ_ROOT)/$(ARCH):$(OBJ_ROOT)
	mkdir $(OBJ_ROOT)/$(ARCH)

$(OBJ_ROOT):
	mkdir $(OBJ_ROOT)

$(OBJ_ROOT)/$(ARCH)/%.o:$(SRC)/%.c
	$(CC) $(CFLAGS) $(INCLUDE_DIR) -c $< -o $@

Debug:$(BIN_NAME)

Release:$(BIN_NAME)

clean:
	$(RM) $(BIN_NAME)
	$(RM) $(OBJ_ROOT)/$(ARCH)/*.o
	$(MAKE) -C doc clean
	$(MAKE) -C test clean

cleanDebug:clean

cleanRelease:clean

doc:
	$(MAKE) -C doc doc

.PHONY: clean cleanDebug cleanRelease doc


