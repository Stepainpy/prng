.PHONY: all clean
CC = cc
CFLAGS += -std=c11 -O2
CFLAGS += -Wall -Wextra -pedantic
OBJD = bin
TARGET = tp

SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,$(OBJD)/%.o,$(SRC))

all: $(OBJ)
	$(CC) -o $(TARGET) $^

$(OBJD)/%.o: %.c | $(OBJD)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJD):
	mkdir $@

clean:
	del /Q $(TARGET).exe $(subst /,\,$(OBJ)) 2>nul