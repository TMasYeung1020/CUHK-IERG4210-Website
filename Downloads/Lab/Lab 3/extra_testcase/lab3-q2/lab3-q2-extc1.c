#include"lab3-q2.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

int main(int argc, char *argv[]){

	FILE *fout;
	Stack *s;
	int isCorrect;

	fout = fopen(argv[1], "w");

	/* default, your program is correct*/
	isCorrect = 1;

	/* check the function, stack_init()*/
	s = NULL;
	stack_init(&s);

	/*if s is not initialized appropriately*/
	if (s == NULL) {
		isCorrect = 0;
	} else if (s->next != NULL || s->data != CHAR_MAX) {
		isCorrect = 0;
	}

	/* output "Correct" if the stack_init() is correctly implemented*/
	if (isCorrect == 1) {
		fprintf(fout, "Stack: Correct\n");
	} else {
		fprintf(fout, "Stack: Wrong Answer\n");
	}
	fclose(fout);

	return 0;
}
