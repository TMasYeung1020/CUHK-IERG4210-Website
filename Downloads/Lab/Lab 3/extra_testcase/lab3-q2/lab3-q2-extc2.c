#include"lab3-q2.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

int main(int argc, char *argv[]){

	FILE *fout;
	Stack *s, *ptr;
	int i;
	int isCorrect;

	fout = fopen(argv[1], "w");

	/* default, your program is correct*/
	isCorrect = 1;

	/* check the function, stack_make_empty()*/
	s = NULL;
	s = malloc(sizeof(Stack));
	s->next = NULL;

	for (i = 0; i < 3; i++) {
		ptr = NULL;
		ptr = malloc(sizeof(Stack));
		ptr->data = i;
		ptr->next = s->next;
		s->next = ptr;
	}

	/* if s is not empty*/
	stack_make_empty(s);
	if (s == NULL) {
		isCorrect = 0;
	} else if (s->next != NULL) {
		isCorrect = 0;
	}

	/* output "Correct" if the stack_make_empty() is correctly implemented*/
	if (isCorrect == 1) {
		fprintf(fout, "Stack: Correct\n");
	} else {
		fprintf(fout, "Stack: Wrong Answer\n");
	}
	fclose(fout);

	return 0;
}
