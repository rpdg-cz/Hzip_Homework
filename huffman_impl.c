#include "Huffman.h"

tnode* new_node() {
	tnode *p = (tnode *)malloc(sizeof(tnode));
	p->left = p->right = NULL;
	p->weight = 0;
	p->c = 0;
	p->is_leaf = 0;
	return p;
}

void init(Huffman_h *ctex) {
	for(int i = 0; i < ASCII_SIZE; i ++)
		ctex->Ccount[i] = 0;
	for(int i = 0; i < 128; i ++)
		for(int j = 0; j < MAXSIZE; j ++) {
			ctex->HCode[i][j] = 0;
		}
	ctex->vis_cnt = 0;
	ctex->Root = NULL;
	ctex->Src = NULL;
	ctex->Obj = NULL;
}

void statCount(Huffman_h *ctex) {
	char c;
	while((c = fgetc(ctex->Src)) != EOF) {
		// if(c == '\n') continue;
		ctex->Ccount[(int)c] ++;
	}
	ctex->Ccount[0] = 1;
	fclose(ctex->Src);
}

int cmp(const void *a, const void *b) {
	struct tnode **p = (struct tnode **)a, **q = (struct tnode **)b;
	if((*p) -> weight == (*q)->weight) {
		if((*p)->c < (*q)->c) return -1;
		else if((*p)->c > (*q)->c) return 1;
		else return 0;
	}
	else if((*p)->weight > (*q)->weight) return 1;
	else return -1;
}
void createHTree(Huffman_h *ctex) {
	struct tnode *F[ASCII_SIZE * 2];
	for(int i = 0; i < ASCII_SIZE * 2; i ++) F[i] = NULL;
	int hd = 0, tl = 0;
	for(int i = 0; i < 128; i ++) {
		struct tnode *p = (struct tnode *)malloc(sizeof(tnode));
		if(ctex->Ccount[i]) {
			p->c = (char) i;
			p->weight = ctex->Ccount[i];
			p->left = p->right = NULL;
			F[tl] = p;
			tl ++;
			ctex->vis_cnt ++;
		}
	}
	qsort(F, tl, sizeof(tnode *), cmp);
	while(hd < tl - 1) {
		tnode *t0 = F[hd], *t1 = F[hd + 1];
		hd += 2;
		tnode *q = (tnode *)malloc(sizeof(tnode));
		q->weight = t0->weight + t1->weight;
		q->left = t0;
		q->right = t1;
		int pos = hd - 2;
		for(int i = hd; i < tl; i ++) {
			if(F[i]->weight > q->weight) {
				pos = i - 1;
				break;
			}
		}
		if(pos == hd - 1) {
			hd --;
			F[hd] = q;
		}
		else if(pos == hd - 2) {
			F[tl] = q;
			tl ++;
		}
		else {
			for(int i = tl; i > pos + 1; i --) {
				F[i] = F[i - 1];
			}
			F[pos + 1] = q;
			tl ++;
		}
	}
	ctex->Root = F[hd];
}

void dfs1(struct tnode *x, char path[], int cnt, Huffman_h *ctex) {
	if(!x->left && !x->right) {
		path[cnt] = '\0';
		strcpy(ctex->HCode[x->c], path);
		return;
	}
	if(x->left) {
		path[cnt] = '0';
		dfs1(x->left, path, cnt + 1, ctex);
	}
	if(x->right) {
		path[cnt] = '1';
		dfs1(x->right, path, cnt + 1, ctex);
	}
}

void makeHCode(Huffman_h *ctex) {
	char path[ASCII_SIZE * 2];
	path[0] = '\0';
	dfs1(ctex->Root, path, 0, ctex);
}

void atoHZIP(Huffman_h *ctex) {
	char c;
	unsigned char hc = 0;
	int i = 0;
	while((c = fgetc(ctex->Src)) != EOF) {
		for(int k = 0; k < strlen(ctex->HCode[c]); k++,i++) {
			hc = (hc << 1)|(ctex->HCode[c][k] - '0');
			if((i + 1) % 8 == 0) {
				fputc(hc, ctex->Obj);
			}
		}
	}
	for(int k = 0; k < strlen(ctex->HCode[0]); k++,i++) {
		hc = (hc << 1)|(ctex->HCode[0][k] - '0');
		if((i + 1) % 8 == 0) {
			fputc(hc, ctex->Obj);
		}
	}
	int flg = 0;
	while(i%8 != 0) {
		hc = hc << 1;
		i++;
		flg = 1;
	}
	if(flg) fputc(hc, ctex->Obj);
}

void print_tab(Huffman_h *ctex) {
	char c;
	char hc = 0;
	fputc((char)ctex->vis_cnt, ctex->Obj);
	for(int i = 0; i < 128; i ++) {
		if(ctex->Ccount[i]) {
			fputc((char)i, ctex->Obj);
			fputc((char)strlen(ctex->HCode[i]), ctex->Obj);
			int r = 0;
			for(int k = 0; k < strlen(ctex->HCode[i]); k++,r++) {
				hc = (hc << 1)|(ctex->HCode[i][k] - '0');
				if((r + 1) % 8 == 0) {
					fputc(hc, ctex->Obj);
					// printf("%x", hc);
				}
			}
			int flg = 0;
			// printf("%d %d %d\n", i, strlen(HCode[i]), r);
			while(r%8 != 0) {
				hc = hc << 1;
				r++;
				flg = 1;
			}
			if(flg) fputc(hc, ctex->Obj);
		}
	}
}

int vis[ASCII_SIZE + 2];

void get_code(Huffman_h *ctex) {
	char c;
	c = fgetc(ctex->Src);
	for(int i = 0; i < (int)c; i ++) {
		char rec = fgetc(ctex->Src);
		char len = fgetc(ctex->Src);
		int lenc = 0;
		for(int j = 1; j <= len/8; j ++) {
			char code = fgetc(ctex->Src);
			for(int k = 7; k >= 0; k --) {
				ctex->HCode[rec][lenc++] = (char)((code >> k) & 1) + '0';
			}
		}
		if(len % 8 != 0) {
			char code = fgetc(ctex->Src);
			for(int k = 7; k >= 8 - (len%8); k --) {
				ctex->HCode[rec][lenc++] = (char)((code >> k) & 1) + '0';
			}
		}
		ctex->HCode[rec][lenc] = '\0';
		vis[rec] = 1;
	}
}

void pushup(tnode *p) {
	if(p->left) {
		pushup(p->left);
		p->weight += p->left->weight;
	}
	if(p->right) {
		pushup(p->right);
		p->weight += p->right->weight;
	}
	return;
}

void insert(char str[], char c, Huffman_h *ctex) {
	tnode *cur = ctex->Root;
	if(ctex->Root == NULL) {
		ctex->Root = new_node();
		cur = ctex->Root;
	}
	for(int i = 0; i < strlen(str); i ++) {
		int pos = str[i] - '0';
		if(pos == 0) {
			if(!cur->left) {
				cur->left = new_node();    
			}
			cur = cur->left;
		}
		else {
			if(!cur->right) {
				cur->right = new_node();    
			}
			cur = cur->right;
		}
	}
	cur->weight = cur->c = c;
	cur->is_leaf = 1;
	pushup(ctex->Root);
}

void rebuild_HTree(Huffman_h *ctex) {
	for(int i = 0; i < 128; i ++) {
		if(vis[i]) {
			insert(ctex->HCode[i], (char)i, ctex);
		}
	}
}

void HZIP_to_a(Huffman_h *ctex) {
	tnode *cur = ctex->Root;
	int ch;
	while((ch = fgetc(ctex->Src)) != EOF) {
		unsigned char c = (unsigned char) ch;
		for(int i = 7; i >= 0; i --) {
			int v = (int)((c >> i) & 1);
			if(cur->is_leaf) {
				if(cur->c == 0) {
					break;
				}
				fprintf(ctex->Obj, "%c", (char)cur->c);
				fflush(ctex->Obj);
				cur = ctex->Root;
			}
			if(v == 0) {
				cur = cur->left;
			}
			if(v == 1) {
				cur = cur->right;
			}
		}
	}
}

void print4(Huffman_h *ctex) {
	long int in_size, out_size;
	
	fseek(ctex->Src,0,SEEK_END);
	fseek(ctex->Obj,0,SEEK_END);
	in_size = ftell(ctex->Src);
	out_size = ftell(ctex->Obj);
	
	printf("\nOriginal file size: %ldB\n",in_size);
	printf("hzip file size: %ldB\n",out_size);
	printf("compression ratio: %.2f%%\n",(float)(in_size-out_size)*100/in_size);
}
void check(void *p) {
	if(p == NULL) {
		exit(-1);
	}
}
char **get_file_name_and_extension(char str[]) {
	char *file_name, *extension;
	int file_len = 0;
	for(int i = 0; str[i] != '\0'; i++) {
		if(str[i] == '.') {
			break;
		}
		file_len ++;
	}
	file_name = (char *)malloc((file_len + 2) * sizeof(char));
	check(file_name);
	extension = (char *)malloc((strlen(str) - file_len + 2) * sizeof(char));
	check(extension);
	for(int i = 0; i < file_len; i ++) {
		file_name[i] = str[i];
	}
	file_name[file_len] = '\0';
	for(int i = file_len + 1; i < strlen(str); i++) {
		extension[i - file_len - 1] = str[i];
	}
	extension[strlen(str) - file_len - 1] = '\0';
	char **pair;
	pair = (char **)malloc(2 * sizeof(char *));
	check(pair);
	pair[0] = file_name;
	pair[1] = extension;
	return pair;
}
char *change_extension(char file_name[], char ex[]) {
	char *output;
	output = (char *)malloc((strlen(file_name) + strlen(ex) + 5)*sizeof(char));
	int len = strlen(file_name);
	for(int i = 0; i < len; i ++) {
		output[i] = file_name[i];
	}
	output[len] = '.';
	int ex_len = strlen(ex);
	for(int i = len + 1; i < len + ex_len + 1; i ++) {
		output[i] = ex[i - len - 1];
	}
	output[len + ex_len + 1] = '\0';
	return output;
}

int encode(int argc, char *argv[], Huffman_h *ctex) {
	char **pair = get_file_name_and_extension(argv[1]);
	char *file_name = pair[0], *extension = pair[1];
	int file_len = strlen(file_name);
	ctex->Src = fopen(argv[1], "r");
	if(ctex->Src == NULL) {
		perror("File can't be found!!!");
		return -1;
	}
	char *output = change_extension(file_name, "hzip");
	ctex->Obj = fopen(output, "w");
	if(ctex->Obj == NULL) {
		perror("File can't be found!!!");
		return -1;
	}
	statCount(ctex);
	createHTree(ctex);
	makeHCode(ctex);
	ctex->Src = fopen(argv[1], "r");
	print_tab(ctex);
	atoHZIP(ctex);
	print4(ctex);
	fclose(ctex->Src);
	fclose(ctex->Obj);
	free(output);
	free(file_name);
	free(extension);
	free(pair);
	return 0;
}

int decode(int argc, char *argv[], Huffman_h *ctex) {
	if(argv[1][0] != '-' || (argv[1][0] == '-' && argv[1][1] != 'u')) {
		perror("Usage: hzip.exe [-u] <filename>");
		return -1;
	}
	char **pair = get_file_name_and_extension(argv[2]);
	int file_len = 0;
	char *file_name = pair[0], *extension = pair[1];
	
	if(strcmp(extension, "hzip") != 0) {
		perror("File extension error!");
		return -1;
	}
	ctex->Src = fopen(argv[2], "rb");
	if(ctex->Src == NULL) {
		perror("File can't be found!!!");
		return -1;
	}
	char *output = change_extension(file_name, "txt");
	ctex->Obj = fopen(output, "w");
	if(ctex->Obj == NULL) {
		perror("File can't be found!!!");
		return -1;
	}
	get_code(ctex);
	rebuild_HTree(ctex);
	HZIP_to_a(ctex);
	fclose(ctex->Src);
	fclose(ctex->Obj);
	free(output);
	free(file_name);
	free(extension);
	free(pair);
	return 0;
}