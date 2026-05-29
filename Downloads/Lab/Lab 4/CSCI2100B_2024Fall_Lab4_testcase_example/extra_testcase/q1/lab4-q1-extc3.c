#include"lab4-q1.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

char *print_list(List* L) {
	int n, len;
	char *output;
	List *ptr;
	
	ptr = L;
	n = 0;
	while (ptr != NULL) {
		ptr = ptr->next;
		n++;
	}
	
	len = n*24+3;
	output = malloc(sizeof(char)*len);
	memset(output, 0, sizeof(output));
	
	ptr = L;
	while (ptr != NULL) {
		sprintf(output, "%s%d-%d ", output, ptr->key, ptr->value);
		ptr = ptr->next;
	}
	
	output[strlen(output)-1] = 0;
	return output;
	
}

int main(int argc, char *argv[]){

	FILE *fout;
	Hash_SC *T;
	int isCorrect, i;
	List *tmp, *anslist;
	char *str1, *str2;
	
	fout = fopen(argv[1], "w");
	
	/* default, your program is correct*/
	isCorrect = 1;
	
	/* check the function, hash_insert()*/
	T = malloc(sizeof(Hash_SC));
	T->size = 10;
	T->slots = malloc(sizeof(List*)*10);
	for (i = 0; i < T->size; i++) {
		T->slots[i] = NULL;
	}
	
	anslist = NULL;
	for (i = 0; i < 4; i++) {
		tmp = malloc(sizeof(List));
		tmp->key = i*10 + 1;
		tmp->value = i;
		tmp->next = T->slots[1];
		T->slots[1] = tmp;
		
		tmp = malloc(sizeof(List));
		tmp->key = i*10 + 1;
		tmp->value = i;
		tmp->next = anslist;
		anslist = tmp;
	}
		
	hash_insert(T, 51, 10);
	hash_insert(T, 61, 11);
	hash_insert(T, 91, 11);
	
	tmp = malloc(sizeof(List));
	tmp->key = 51;
	tmp->value = 10;
	tmp->next = anslist;
	anslist = tmp;
	
	tmp = malloc(sizeof(List));
	tmp->key = 61;
	tmp->value = 11;
	tmp->next = anslist;
	anslist = tmp;
	
	tmp = malloc(sizeof(List));
	tmp->key = 91;
	tmp->value = 11;
	tmp->next = anslist;
	anslist = tmp;
	
	str1 = print_list(T->slots[1]);
	str2 = print_list(anslist);
	
	/*if T is not implemented appropriately*/
	if (strlen(str1) != strlen(str2)) {
		isCorrect = 0;
	} else if (strncmp(str1, str2, strlen(str1)) != 0) {
		isCorrect = 0;
	}
	
	
	/* output "Correct" if the hash_insert() is correctly implemented*/
	if (isCorrect == 1) {
		fprintf(fout, "Correct\n");
	} else {
		fprintf(fout, "Wrong Answer\n");
	}
	fclose(fout);
	
	return 0;
}