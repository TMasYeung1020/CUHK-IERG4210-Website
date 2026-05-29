// This file is named as lab1-q2-tc1.c
#include"lab1-q2.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

int main(int argc, char *argv[]){

	FILE *fout;
	ArrayList *list;
	int isCorrect, i;	
	fout = fopen(argv[1], "w");
	
	/* default, your program is correct */
	isCorrect = 1;
	
	/* check the function, arraylist_init_empty */
	arraylist_init_empty(&list);
	
	/* if arraylist_init_empty isn't implemented appropriately */
	if (list == NULL) {
		isCorrect = 0;
	} else if (list->first != 0 || list->last != 0) {
		isCorrect = 0;
	} else if (list->word[0][0] != 0 || list->word[99][50] != 0) {
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
