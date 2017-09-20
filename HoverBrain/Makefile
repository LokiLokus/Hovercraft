CC = gcc
LD = gcc
CFLAGS = -std=c99
LDFLAGS =

BIN = bin
RUN = $(BIN)/ptrs

OBJECTS += $(BIN)/dbuffer.o
OBJECTS += $(BIN)/events.o
OBJECTS += $(BIN)/main.o

EXTERN_LIBS =

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

$(RUN): $(BIN) $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) $(EXTERN_LIBS)

$(BIN):
	mkdir $(BIN)

$(BIN)/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@
