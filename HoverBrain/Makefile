CC = gcc
LD = gcc
CFLAGS = -std=c99 -Ilibwebsock/src
LDFLAGS = -Llibwebsock/src/.libs

LIBWEBSOCK = libwebsock/src/.libs/libwebsock.so

BIN = bin
RUN = $(BIN)/brain

OBJECTS += $(BIN)/dbuffer.o
OBJECTS += $(BIN)/events.o
OBJECTS += $(BIN)/websocket.o
OBJECTS += $(BIN)/main.o

EXTERN_LIBS = -lwebsock

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

$(BIN):
	mkdir $(BIN)

$(BIN)/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@
