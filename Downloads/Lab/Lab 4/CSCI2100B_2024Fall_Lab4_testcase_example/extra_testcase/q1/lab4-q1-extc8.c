#include"lab4-q1.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

int main(int argc, char *argv[]){

	FILE *fout;
	Hash_SC *T;
	int isCorrect, i;
	List *tmp;
	int v1, v2, v3, v4;
	
	fout = fopen(argv[1], "w");
	
	/* default, your program is correct*/
	isCorrect = 1;
	
	/* check the function, hash_free()*/
	T = malloc(sizeof(Hash_SC));
	T->size = 11;
	T->slots = malloc(sizeof(List*)*11);
	for (i = 0; i < T->size; i++) {
		T->slots[i] = NULL;
	}
	
	for (i = 0; i < 8; i++) {
		tmp = malloc(sizeof(List));
		tmp->key = i*(T->size) + 0;
		tmp->value = i*2;
		tmp->next = T->slots[0];
		T->slots[0] = tmp;
	}
	for (i = 2; i < 4; i++) {
		tmp = malloc(sizeof(List));
		tmp->key = i*(T->size) + 10;
		tmp->value = i-3;
		tmp->next = T->slots[10];
		T->slots[10] = tmp;
	}
	
	hash_free(&T);
	
	
	if(T!=NULL) isCorrect = 0;
	
	
	
	/* output "Correct" if the hash_free() is correctly implemented*/
	if (isCorrect == 1) {
		fprintf(fout, "Correct\n");
	} else {
		fprintf(fout, "Wrong Answer\n");
	}
	fclose(fout);
	
	return 0;
}