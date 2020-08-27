CC = gcc
CFLAGS = -c -Wall -g -O3
LD = $(CC)
LDFLAGS = -lcrypto -lssl

OBJECTS = $(patsubst src/utils/%.c, %.o, $(shell find ./src/utils -name "*.c"))
TARGETS = $(patsubst ./src/challenges/%.c, bin/%, $(shell find ./src/challenges -name "*.c"))

all: $(TARGETS)

bin/%: $(OBJECTS) src/challenges/%.c
	mkdir -p $(dir $@)
	$(LD) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $^ -o $@
