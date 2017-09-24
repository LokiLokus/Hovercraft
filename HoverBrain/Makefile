CC = gcc
LD = gcc
CFLAGS = -D_GNU_SOURCE -std=c99 -Ilibwebsock/src -Ipigpio
LDFLAGS = -Llibwebsock/src/.libs -Lpigpio

LIBWEBSOCK = libwebsock/src/.libs/libwebsock.so
LIBPIGPIO = pigpio/libpigpio.so

BIN = bin
RUN = $(BIN)/brain

OBJECTS += $(BIN)/dbuffer.o
OBJECTS += $(BIN)/events.o
OBJECTS += $(BIN)/util.o
OBJECTS += $(BIN)/log.o
#OBJECTS += $(BIN)/websocket.o
OBJECTS += $(BIN)/main.o
OBJECTS += $(BIN)/hw_access.o

EXTERN_LIBS = -lpigpio -pthread

all: CFLAGS += -O2 -g
all: $(RUN)

debug: CFLAGS += -g
debug: $(RUN)

release: CFLAGS += -O2
release: $(RUN)

install: release
	cp $(RUN) /usr/local/bin/

remove:
	rm /usr/local/bin/ptrs

clean:
	if [ -d $(BIN) ]; then rm -r $(BIN); fi

$(RUN): $(LIBWEBSOCK) $(BIN) $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) $(EXTERN_LIBS) -o $(RUN)

$(LIBWEBSOCK):
	cd libwebsock && ./autogen.sh
	cd libwebsock && ./configure
	$(MAKE) -C libwebsock

$(LIBPIGPIO):
	$(MAKE) -C pigpio

$(BIN):
	mkdir $(BIN)

$(BIN)/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@
