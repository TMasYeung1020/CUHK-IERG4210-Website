// This file is named as lab1-q1-tc1.c
#include"lab1-q1.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

int main(int argc, char *argv[]){

	FILE *fout;
	Node *list;
	int isCorrect, i;	
	fout = fopen(argv[1], "w");
	
	/* default, your program is correct */
	isCorrect = 1;
	
	/* check the function, list_init_empty() */
	list_init_empty(&list);
	
	/* if list_init_empty() is not implemented appropriately */
	if (list != NULL) {
		isCorrect = 0;
	} 
		
	/* output "Correct" if test case is passed */
	if (isCorrect == 1) {
		fprintf(fout, "List: Correct\n");
	} else {
		fprintf(fout, "List: Wrong Answer\n");
	}
	fclose(fout);

	return 0;
}
