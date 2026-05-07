#ifndef HUFFMAN_H

#define HUFFMAN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXSIZE 32
#define MAX_SIZE2 10002
#define ASCII_SIZE 128

typedef struct tnode {            //Huffman树结构
	char c;    
	int weight;
	struct tnode *left,*right;
	int is_leaf;
	int rank;
} tnode;

typedef struct Huffman_h {
	int Ccount[ASCII_SIZE];        //存放每个字符的出现次数，如Ccount[i]表示ASCII值为i的字符出现次数 
	struct tnode *Root;     //Huffman树的根节点
	char HCode[ASCII_SIZE][MAXSIZE]; //字符的Huffman编码，如HCode['a']为字符a的Huffman编码（字符串形式） 
	int vis_cnt;
	FILE *Src, *Obj;
	int vis_for_decode[ASCII_SIZE * 2];
} Huffman_h;

void init(Huffman_h *ctx);
void statCount(Huffman_h *ctx);
void createHTree(Huffman_h *ctx);
void makeHCode(Huffman_h *ctx);
void atoHZIP(Huffman_h *ctx);
void print_tab(Huffman_h *ctx);
void get_code(Huffman_h *ctx);
void rebuild_HTree(Huffman_h *ctx);
void HZIP_to_a(Huffman_h *ctx);
void print4(Huffman_h *ctx);
int encode(int argc, char *argv[], Huffman_h *ctx);
int decode(int argc, char *argv[], Huffman_h *ctx);

#endif