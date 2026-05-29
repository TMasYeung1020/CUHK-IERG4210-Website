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
	char *str1;
	char str2[] = "0:\n1:(1)-12345\n2:(2)-11\n3:(3)1212\n4:\n5:(5)-1010\n6:(6)-1\n";
	
	
	fout = fopen(argv[1], "w");
	
	/* default, your program is correct*/
	isCorrect = 1;
	
	/* check the function, hash_print()*/
	T = malloc(sizeof(Hash_OA));
	T->size = 7;
	T->hash_function = h1;
	T->hash_function2 = h2;
	T->slots = malloc(sizeof(Node)*11);
	for (i = 0; i < T->size; i++) {
		T->slots[i].key = HASH_NULL_KEY;
	}
	
	T->slots[1].key = 1; T->slots[1].value = -12345;
	T->slots[2].key = 2; T->slots[2].value = -11;
	T->slots[3].key = 3; T->slots[3].value = 1212;
	T->slots[5].key = 5; T->slots[5].value = -1010;
	T->slots[6].key = 6; T->slots[6].value = -1;
	
	str1 = hash_print(T);

	
	/* if hash_print() not implemented appropriately */
	if (strlen(str1) != strlen(str2)) {
		isCorrect = 0;
	} else if (strncmp(str1, str2, strlen(str2)) != 0) {
		isCorrect = 0;
	}
	
	/* output "Correct" if the hash_print() is correctly implemented*/
	if (isCorrect == 1) {
		fprintf(fout, "Hash: Correct\n");
	} else {
		fprintf(fout, "Hash: Wrong Answer\n");
	}
	fclose(fout);

	return 0;
	
}
