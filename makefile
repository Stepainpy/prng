.PHONY: all clean
CC = cc
CFLAGS += -std=c11 -O2 -s
CFLAGS += -Wall -Wextra -pedantic
TARGET = tp
OBJD = bin

EXE = $(TARGET).exe
SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,$(OBJD)/%.o,$(SRC))

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

$(OBJD)/%.o: %.c | $(OBJD)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJD):
	mkdir $@

clean:
	del /Q $(EXE) $(subst /,\,$(OBJ)) 2>nul