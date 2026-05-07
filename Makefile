# 编译器与标志
CC = gcc
CFLAGS = -Wall -Wextra -O2 -I.
TARGET = hzip
OBJS = hzip.o huffman_impl.o Heap.o

# 默认目标
all: $(TARGET)

# 链接
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# 编译源文件
hzip.o: hzip.c Huffman.h
	$(CC) $(CFLAGS) -c $< -o $@

huffman_impl.o: huffman_impl.c Huffman.h Heap.h
	$(CC) $(CFLAGS) -c $< -o $@

Heap.o: Heap.c Heap.h
	$(CC) $(CFLAGS) -c $< -o $@

# 清理中间文件和可执行文件
clean:
	rm -f $(OBJS) $(TARGET)

# 彻底清理（同clean）
distclean: clean

# 声明伪目标
.PHONY: all clean distclean