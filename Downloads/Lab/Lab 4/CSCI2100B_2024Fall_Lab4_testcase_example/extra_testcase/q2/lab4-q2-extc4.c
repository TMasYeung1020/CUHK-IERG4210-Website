#include"lab4-q2.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

int h1(int key, int size) { return key % size; }
int h2(int key, int size) { return key % (size - 1) + 1; }

int main(int argc, char *argv[]){

	FILE *fout;
	Hash_OA *T;
	int isCorrect, i;
	int v1, v2, v3, v4, v5;
	
	fout = fopen(argv[1], "w");
	
	/* default, your program is correct*/
	isCorrect = 1;
	
	/* check the function, hash_insert()*/
	T = malloc(sizeof(Hash_OA));
	T->size = 10;
	T->hash_function = h1;
	T->hash_function2 = h2;
	T->slots = malloc(sizeof(Node)*10);
	for (i = 0; i < T->size; i++) {
		T->slots[i].key = HASH_NULL_KEY;
	}
	
	T->slots[0].key = 0;
	T->slots[1].key = 2;
	T->slots[2].key = 4;
	T->slots[3].key = 6;
	T->slots[4].key = 8;
	v1 = hash_insert(T, 25, -10);
	v2 = hash_insert(T, 26, -10);
	v3 = hash_insert(T, 27, -10);
	v4 = hash_insert(T, 28, -10);
	v5 = hash_insert(T, 29, -10);
	
	/*if T is not implemented appropriately*/
	for (i = 5; i < 10; i++) {
		if (T->slots[i].key != 20+i) isCorrect = 0;
		if (T->slots[i].value != -10) isCorrect = 0;
	}
	if (v1 != 1 || v2 != 1 || v3 != 1 || v4 != 1 || v5 != 1) isCorrect = 0;
	for (i = 0; i < 5; i++) {
		if (T->slots[i].key != i*2) isCorrect = 0;
	}

	/* output "Correct" if the hash_insert() is correctly implemented*/
	if (isCorrect == 1) {
		fprintf(fout, "Hash: Correct\n");
	} else {
		fprintf(fout, "Hash: Wrong Answer\n");
	}
	fclose(fout);
	
	return 0;
}
