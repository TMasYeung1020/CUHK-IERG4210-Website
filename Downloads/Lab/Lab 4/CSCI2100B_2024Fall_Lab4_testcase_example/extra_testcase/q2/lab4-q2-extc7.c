#include"lab4-q2.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

int h1(int key, int size) { return key % size; }
int h2(int key, int size) { return key % (size - 2) + 1; }

int main(int argc, char *argv[]){

	FILE *fout;
	Hash_OA *T;
	int isCorrect, i;
	int v1, v2, v3, v4;
	
	fout = fopen(argv[1], "w");
	
	/* default, your program is correct*/
	isCorrect = 1;
	
	/* check the function, hash_find()*/
	T = malloc(sizeof(Hash_OA));
	T->size = 11;
	T->hash_function = h1;
	T->hash_function2 = h2;
	T->slots = malloc(sizeof(Node)*11);
	for (i = 0; i < T->size; i++) {
		T->slots[i].key = HASH_NULL_KEY;
	}
	
	T->slots[8].key = 41; T->slots[8].value = 2;
	T->slots[4].key = 33; T->slots[4].value = -1;
	T->slots[2].key = 24; T->slots[2].value = 17;
	
	
	v1 = hash_find(T, 41);
	v2 = hash_find(T, 33);
	v3 = hash_find(T, 24);
	v4 = hash_find(T, 66);
	
	/*if T is not implemented appropriately*/
	if (v1 != 2 || v3 != 17) {
		isCorrect = 0;
	} else if (v4 != INT_MAX || v2 != INT_MAX) {
		isCorrect = 0;
	}
	
	
	/* output "Correct" if the hash_find() is correctly implemented*/
	if (isCorrect == 1) {
		fprintf(fout, "Hash: Correct\n");
	} else {
		fprintf(fout, "Hash: Wrong Answer\n");
	}
	fclose(fout);
	
	return 0;
}
