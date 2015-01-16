# Global makefile

TEMPLATE_DIR=templates
THIS=$(shell pwd)

all:
	$(MAKE) -C str all
	$(MAKE) -C config all
	$(MAKE) -C checksum all
	$(MAKE) -C serial all
	$(MAKE) -C socket all

clean:
	$(MAKE) -C str clean
	$(MAKE) -C config clean
	$(MAKE) -C checksum clean
	$(MAKE) -C serial clean
	$(MAKE) -C socket clean

doc:
	$(MAKE) -C str doc
	$(MAKE) -C config doc
	$(MAKE) -C checksum doc
	$(MAKE) -C serial doc
	$(MAKE) -C socket doc

commit: clean
	git add --all
	git commit -a

help:
	@echo available targets:
	@echo "all    -compile all modules"
	@echo "clean  -clean all modules"
	@echo "doc    -create doc fro all modules"
	@echo "commit -commit to git"
	@echo

#create_lib:
#	mkdir $(LIB_NAME)
#	mkdir $(LIB_NAME)/src
#	mkdir $(LIB_NAME)/include
#	mkdir $(LIB_NAME)/doc
#	cp $(TEMPLATE_DIR)/.gitignore $(LIB_NAME)

#	awk ' { gsub("PROJECT_NAME +=","PROJECT_NAME=$(LIB_NAME)"); print; } ' $(TEMPLATE_DIR)/doc/doxyfile > $(LIB_NAME)/doc/doxyfile
#	awk ' { gsub("#!#LIB_NAME#!#","$(LIB_NAME)"); print; } ' $(TEMPLATE_DIR)/lib.cbp > $(LIB_NAME)/$(LIB_NAME).cbp
#	awk ' { gsub("#!#LIB_NAME#!#","$(LIB_NAME)"); print; } ' $(TEMPLATE_DIR)/Makefile.lib > $(LIB_NAME)/Makefile
#	mkdir $(LIB_NAME)/test
#	awk ' { gsub("#!#LIB_NAME#!#","$(LIB_NAME)"); print; } ' $(TEMPLATE_DIR)/test/test.cbp > $(LIB_NAME)/test/test.cbp
#	awk ' { gsub("#!#LIB_NAME#!#","$(LIB_NAME)"); gsub("#!#ZLIB#!#","$(THIS)");  print; } ' $(TEMPLATE_DIR)/test/Makefile > $(LIB_NAME)/test/Makefile
#	awk ' { gsub("#!#LIB_NAME#!#","$(LIB_NAME)"); print; } ' $(TEMPLATE_DIR)/test/main.c > $(LIB_NAME)/test/main.c

#$(LIB_NAME):
#	$(error No libname, please call make LIB_NAME=<libname>)

.PHONY:clean commit doc all

