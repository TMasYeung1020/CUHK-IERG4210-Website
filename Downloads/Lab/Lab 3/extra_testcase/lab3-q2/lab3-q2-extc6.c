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
	int s1, s2, s3, s4;

	fout = fopen(argv[1], "w");

	/* default, your program is correct*/
	isCorrect = 1;

	/* check the function, stack_top()*/
	s = NULL;
	s = malloc(sizeof(Stack));
	s->next = NULL;

	s1 = stack_top(s); /* s1 = 0*/

	for (i = 1; i < 5; i+=2) {
		ptr = NULL;
		ptr = malloc(sizeof(Stack));
		ptr->data = i;
		ptr->next = s->next;
		s->next = ptr;
	}

	s2 = stack_top(s); /* s2 = 3*/

	for (i = 1; i < 2; i++) {
		ptr = NULL;
		ptr = malloc(sizeof(Stack));
		ptr->data = i*5;
		ptr->next = s->next;
		s->next = ptr;
	}

	s3 = stack_top(s); /* s3 = 5*/
	s4 = stack_top(s); /* s4 = 5*/

	/* if s1, s2, s3, s4 are not correct	*/

	if (s1 != 0 || s2 != 3 || s3 != 5 || s4 != 5) {
		isCorrect = 0;
	}

	/* output "Correct" if the stack_top() is correctly implemented*/
	if (isCorrect == 1) {
		fprintf(fout, "Stack: Correct\n");
	} else {
		fprintf(fout, "Stack: Wrong Answer\n");
	}
	fclose(fout);

	return 0;
}
