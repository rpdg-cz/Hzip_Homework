CC = gcc
CFLAGS = -Wall -Wextra -Werror -O2
TARGET = hzip
OBJS = hzip.o huffman_impl.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c Huffman.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean