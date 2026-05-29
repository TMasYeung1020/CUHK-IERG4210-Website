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
	int v1, v2, v3, v4, v5;
	
	fout = fopen(argv[1], "w");
	
	/* default, your program is correct*/
	isCorrect = 1;
	
	/* check the function, hash_free()*/
	T = malloc(sizeof(Hash_OA));
	T->size = 12;
	T->hash_function = h1;
	T->hash_function2 = h2;
	T->slots = malloc(sizeof(Node)*12);
	for (i = 0; i < T->size; i++) {
		T->slots[i].key = HASH_NULL_KEY;
	}
	
	T->slots[0].key = 20; T->slots[0].value = 3;
	T->slots[1].key = 21; T->slots[1].value = 3;
	T->slots[2].key = 22; T->slots[2].value = 3;
	T->slots[3].key = 23; T->slots[3].value = 3;
	T->slots[4].key = 24; T->slots[4].value = 3;
	T->slots[5].key = 25; T->slots[5].value = 3;
	T->slots[6].key = 26; T->slots[6].value = 3;
	T->slots[7].key = 27; T->slots[7].value = 3;
	T->slots[8].key = 28; T->slots[8].value = 3;
	T->slots[9].key = 29; T->slots[9].value = 3;
	T->slots[10].key = 30; T->slots[9].value = 3;
	T->slots[11].key = 31; T->slots[9].value = 3;
	
	hash_free(&T);
	
	/*if T is not implemented appropriately*/
	if(T!=NULL) isCorrect = 0;
	
	
	
	/* output "Correct" if the hash_insert() is correctly implemented*/
	if (isCorrect == 1) {
		fprintf(fout, "Hash: Correct\n");
	} else {
		fprintf(fout, "Hash: Wrong Answer\n");
	}
	fclose(fout);
	
	return 0;
}
