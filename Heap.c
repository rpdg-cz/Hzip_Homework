#include<stdio.h>
#include<math.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include "Heap.h"

void swap(void **a, void **b) {
	void *c = *a;
	*a = *b;
	*b = c;
}

void Heap_init(Heap *ctex, int n, int (*cmp)(const void *a,const void *b)) {
	ctex->cmp = cmp;
	ctex->max_size = n + 3;
	ctex->H = (void **)malloc(ctex->max_size * sizeof(void *));
	if (ctex->H == NULL) {
		fprintf(stderr, "Memory allocation failed in Heap_init\n");
		exit(1);
	}
	ctex->HeapSize = 0;
	for(int i = 0; i < ctex->max_size; i ++) {
		ctex->H[i] = NULL;
	}
}

void Heap_delete(Heap *ctex) {
	free(ctex->H);
	ctex ->HeapSize = ctex->max_size = 0;
}

void up(int x, Heap *ctex) {
	while(x > 1 && ctex->cmp(&(ctex->H[x]), &(ctex->H[x / 2])) >= 0) {
		swap(&(ctex->H[x]), &(ctex->H[x / 2]));
		x /= 2;
	}
}

void down(int x, Heap *ctex) {
	while(x * 2 <= ctex->HeapSize) {
		int t = x * 2;
		if(t + 1 <= ctex->HeapSize && ctex->cmp(&(ctex->H[t + 1]), &(ctex->H[t])) >= 0) {
			t = t + 1;
		}
		if(ctex->cmp(&(ctex->H[t]), &(ctex->H[x])) < 0) break;
		swap(&(ctex->H[x]), &(ctex->H[t]));
		x = t;
	}
}

void push(void *val, Heap *ctex) {
	if (ctex->HeapSize + 1 >= ctex->max_size) {
		fprintf(stderr, "Heap overflow in push\n");
		exit(1);
	}
	ctex->H[++ctex->HeapSize] = val;
	up(ctex->HeapSize, ctex);
}

void pop(Heap *ctex) {
	if (ctex->HeapSize == 0) {
		fprintf(stderr, "Heap underflow in pop\n");
		exit(1);
	}
	swap(&(ctex->H[ctex->HeapSize]), &(ctex->H[1]));
	ctex->HeapSize --;
	down(1, ctex);
}

void *top(Heap *ctex) {
	if (ctex->HeapSize == 0) {
		fprintf(stderr, "Heap is empty in top\n");
		return NULL;
	}
	return ctex->H[1];
}

int empty(Heap *ctex) {
	return !ctex->HeapSize;
}

int Size(Heap *ctex) {
	return ctex->HeapSize;
}