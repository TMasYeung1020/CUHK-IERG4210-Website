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
	int s1, s2, s3;

	fout = fopen(argv[1], "w");

	/* default, your program is correct*/
	isCorrect = 1;

	/* check the function, stack_push()*/
	s = NULL;
	s = malloc(sizeof(Stack));
	s->next = NULL;


	for (i = 1; i < 3; i++) {
		stack_push(s, i);
	}


	/* if the stack holds the data in correct order and structure*/
	ptr = s->next;
	i--;
	while (ptr != NULL) {
		if (ptr->data != i) {
			isCorrect = 0;
			break;
		}
		i--;
		ptr = ptr->next;
	}
	if (i != 0) isCorrect = 0;


	/* output "Correct" if the stack_push() is correctly implemented*/
	if (isCorrect == 1) {
		fprintf(fout, "Stack: Correct\n");
	} else {
		fprintf(fout, "Stack: Wrong Answer\n");
	}
	fclose(fout);

	return 0;
}
