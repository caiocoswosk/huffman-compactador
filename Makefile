CC      := gcc
CFLAGS  := -Wall -Wextra -std=c11 -O2 -g
LDFLAGS :=

EXEC    := huffman
SRCS    := heap.c Huffman.c main.c
OBJS    := $(SRCS:.c=.o)

.PHONY: all run clean

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

heap.o:    heap.c heap.h
Huffman.o: Huffman.c Huffman.h heap.h
main.o:    main.c Huffman.h heap.h

run: $(EXEC)
	./$(EXEC)

clean:
	rm -f $(OBJS) $(EXEC)
