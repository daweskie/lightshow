#make c forditashoz v 1.1
SHELL=/bin/sh
USERNAME := $(shell whoami)
CFLAGS= -Wall -g
APPNAME=
OBJS=$(patsubst %.c,%.o,$(wildcard *.c)) \
	$(patsubst src/%.c,src/%.o,$(wildcard src/*.c))

BIN=main
CC=gcc

INCLUDE=-I include -I ./

.PHONY:bin
$(BIN):$(OBJS)
	$(CC) -o$(BIN) $(OBJS)

%.o:%.c
	$(CC) -c $< -o $(patsubst %.c,%.o,$<) $(CFLAGS) $(INCLUDE)

.PHONY: clean
clean:
	$(RM) $(OBJS)
	$(RM) $(BIN)


.PHONY: newapp
newapp:
	./mkproject

pull:
	git pull

commit:
	git add --all
	git commit -a


srcdeploy:
	zip ${SRC_ZIP_NAME} *.c *.h *.cbp *.txt *.depend *.layout

Debug:$(BIN)

cleanDebug:clean
