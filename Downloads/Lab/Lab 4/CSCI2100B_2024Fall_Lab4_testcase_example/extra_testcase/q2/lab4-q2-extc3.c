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
	
	fout = fopen(argv[1], "w");
	
	/* default, your program is correct*/
	isCorrect = 1;
	
	/* check the function, hash_make_empty()*/
	T = malloc(sizeof(Hash_OA));
	T->size = 10;
	T->hash_function = h1;
	T->hash_function2 = h2;
	T->slots = malloc(sizeof(Node)*10);
	for (i = 0; i < T->size; i++) {
		T->slots[i].key = i;
	}
	hash_make_empty(T);
	
	/* if T is not initialized appropriately */
	if (T == NULL) {
		isCorrect = 0;
	} else if (T->size != 10) {
		isCorrect = 0;
	}
	for (i = 0; i <10; i++) {
		if (T->slots[i].key != HASH_NULL_KEY) isCorrect = 0;
	}
	
	/* output "Correct" if the hash_make_empty() is correctly implemented*/
	if (isCorrect == 1) {
		fprintf(fout, "Hash: Correct\n");
	} else {
		fprintf(fout, "Hash: Wrong Answer\n");
	}
	fclose(fout);

	return 0;
}

