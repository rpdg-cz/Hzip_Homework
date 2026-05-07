#ifndef HEAP_H

#define HEAP_H

typedef struct Heap {
	void **H;
	int HeapSize;
	int max_size;
	int (*cmp)(const void *a, const void *b);
} Heap;

void Heap_init(Heap *ctex, int n, int (*cmp)(const void *a, const void *b));
void Heap_delete(Heap *ctex);
void up(int x, Heap *ctex);
void down(int x, Heap *ctex);
void push(void *val, Heap *ctex);
void pop(Heap *ctex);
void *top(Heap *ctex);
int empty(Heap *ctex);
int Size(Heap *ctex);



#endif