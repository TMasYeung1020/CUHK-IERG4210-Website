#include"lab4-q2.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

int h1(int key, int size) { return ((key % size) + size) % size; }
int h2(int key, int size) { int v = size - 2; return v - ((key % v + v)  % v); }

int main(int argc, char *argv[]){

	FILE *fout;
	Hash_OA *T;
	int isCorrect, i;
	char *str1, *str2;
	int v1, v2, v3, v4, v5, v6, v7;
	char ans1[500] = "0:\n1:\n2:(12)111\n3:\n4:\n5:\n6:\n7:(17)222\n8:\n9:(19)333\n";
	char ans2[500] = "0:\n1:\n2:(12)111\n3:\n4:(4)888\n5:\n6:\n7:(17)222\n8:(18)777\n9:(19)333\n";
	
	fout = fopen(argv[1], "w");
	
	/* default, your program is correct*/
	isCorrect = 1;

	/* modified example in the lecture note */
	T = NULL;
         hash_init(&T, 10, h1, h2);
	
	hash_insert(T, 12, 111);
	hash_insert(T, 17, 222);
	hash_insert(T, 19, 333);


	str1 = hash_print(T);
	
	v1 = hash_insert(T, 18, 666);
	v2 = hash_insert(T, 18, 777);
	v3 = hash_insert(T, 4, 888);
	

	str2 = hash_print(T);

	
	v4 = hash_find(T, 9);
	v5 = hash_contain(T, 9);
	v6 = hash_find(T, 18);
	v7 = hash_contain(T, 4);
	
	hash_free(&T);

	/* if T is not implemented appropriately */
	if (v1 != 1 || v2 != 2 || v3 != 1) {
		isCorrect = 0;
	} else if (v4 != INT_MAX || v5 != 0) {
		isCorrect = 0;
	} else if (v6 != 777 || v7 != 1) {
		isCorrect = 0;
	} else if (strlen(str1) != strlen(ans1) || strncmp(str1, ans1, strlen(ans1)) != 0) {
		isCorrect = 0;
	} else if (strlen(str2) != strlen(ans2) || strncmp(str2, ans2, strlen(ans2)) != 0) {
		isCorrect = 0;
	} 
	
	/* output "Correct" if the hash_function() is correctly implemented*/
	if (isCorrect == 1) {
		fprintf(fout, "Hash: Correct\n");
	} else {
		fprintf(fout, "Hash: Wrong Answer\n");
	}
	fclose(fout);

	return 0;
}

	