#include"lab4-q1.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

int main(int argc, char *argv[]){

	FILE *fout;
	Hash_SC *T;
	int isCorrect, i;
	
	fout = fopen(argv[1], "w");
	
	/* default, your program is correct*/
	isCorrect = 1;
	
	/* check the function, hash_make_empty()*/
	T = malloc(sizeof(Hash_SC));
	T->size = 18;
	T->slots = malloc(sizeof(List*)*18);
	for (i = 0; i < T->size; i++) {
		T->slots[i] = malloc(sizeof(List));
		T->slots[i]->key = i;
		T->slots[i]->next = NULL;
	}
	hash_make_empty(T);
	
	/*if T is not initialized appropriately*/
	if (T == NULL) {
		isCorrect = 0;
	} else if (T->size != 18) {
		isCorrect = 0;
	}
	for (i = 0; i < 18; i++) {
		if (T->slots[i] != NULL) isCorrect = 0;
	}
	
	/* output "Correct" if the hash_make_empty() is correctly implemented*/
	if (isCorrect == 1) {
		fprintf(fout, "Correct\n");
	} else {
		fprintf(fout, "Wrong Answer\n");
	}
	fclose(fout);
	
	return 0;
}