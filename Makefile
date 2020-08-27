CC = gcc
CFLAGS = -c -Wall -g -O3
LD = $(CC)
LDFLAGS =

TARGET = MyProject

OBJECTS = $(patsubst src/utils/%.c, %.o, $(shell find ./src/utils -name "*.c"))

all: bin/s1/c1 bin/s1/c2 bin/s1/c3 bin/s1/c4 bin/s1/c5 bin/s1/c6

bin/s1/c1: $(OBJECTS) src/challenges/set1/challenge1.c
				mkdir -p bin/s1/
				$(LD) -o $@ $^ $(LDFLAGS)

bin/s1/c2: $(OBJECTS) src/challenges/set1/challenge2.c
				mkdir -p bin/s1/
				$(LD) -o $@ $^ $(LDFLAGS)

bin/s1/c3: $(OBJECTS) src/challenges/set1/challenge3.c
				mkdir -p bin/s1/
				$(LD) -o $@ $^ $(LDFLAGS)

bin/s1/c4: $(OBJECTS) src/challenges/set1/challenge4.c
				mkdir -p bin/s1/
				$(LD) -o $@ $^ $(LDFLAGS)

bin/s1/c5: $(OBJECTS) src/challenges/set1/challenge5.c
				mkdir -p bin/s1/
				$(LD) -o $@ $^ $(LDFLAGS)

bin/s1/c6: $(OBJECTS) src/challenges/set1/challenge6.c
				mkdir -p bin/s1/
				$(LD) -o $@ $^ $(LDFLAGS)

%.o: %.c
				$(CC) $(CFLAGS) $^ -o $@
