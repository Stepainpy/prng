CC = cc
CFLAGS += -std=c11 -O2
CFLAGS += -Wall -Wextra -pedantic

SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,%.o,$(SRC))

all: $(OBJ)
	$(CC) -o tp $(OBJ) $(CFLAGS)

%.o: %.c prng.h
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	del /Q $(OBJ) $(wildcard *.exe) 2>nul