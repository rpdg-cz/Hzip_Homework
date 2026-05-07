/**
 * @file hzip.c
 * @brief Huffman 压缩/解压缩工具
 * @author Lycroff
 * @date 2025-05-06
 * 
 * 用法: 
 *   压缩: hzip.exe input.txt
 *   解压: hzip.exe -u input.hzip
 */

#include<stdio.h>
#include<math.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include "Huffman.h"

int main(int argc, char *argv[]) {
	Huffman_h ctex;
	init(&ctex);
	if(argc < 2 || argc >= 4) {
		perror("Usage: hzip.exe [-u] <filename>");
		return -1;
	}
	else if(argc == 2) {
		return encode(argc, argv, &ctex);
	}
	else if(argc == 3) {
		return decode(argc, argv, &ctex);
	}
}