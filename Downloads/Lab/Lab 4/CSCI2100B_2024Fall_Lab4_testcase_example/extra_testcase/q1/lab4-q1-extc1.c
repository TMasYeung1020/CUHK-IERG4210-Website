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
	
	/* check the function, hash_init()*/
	T = NULL;
	hash_init(&T,21);
	
	/*if T is not initialized appropriately*/
	if (T == NULL) {
		isCorrect = 0;
	} else if (T->size != 21) {
		isCorrect = 0;
	}
	for (i = 0; i < 21; i++) {
		if (T->slots[i] != NULL) isCorrect = 0;
	}
	
	/* output "Correct" if the hash_init() is correctly implemented*/
	if (isCorrect == 1) {
		fprintf(fout, "Correct\n");
	} else {
		fprintf(fout, "Wrong Answer\n");
	}
	fclose(fout);

	return 0;
}

