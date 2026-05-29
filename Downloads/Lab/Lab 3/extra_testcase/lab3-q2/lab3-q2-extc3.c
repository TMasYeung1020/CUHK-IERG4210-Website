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
	int s1, s2;

	fout = fopen(argv[1], "w");

	/* default, your program is correct*/
	isCorrect = 1;

	/* check the function, stack_size()*/
	s = NULL;
	s = malloc(sizeof(Stack));
	s->next = NULL;

	s1 = stack_size(s); /* s1 = 0*/

	for (i = 0; i < 5; i++) {
		ptr = NULL;
		ptr = malloc(sizeof(Stack));
		ptr->data = i;
		ptr->next = s->next;
		s->next = ptr;
	}

	s2 = stack_size(s); /* s2 = 5*/


	/* if s1, s2 are not correct	*/


	if (s1 != 0 || s2 != 5) {
		isCorrect = 0;
	}

	/* output "Correct" if the stack_size() is correctly implemented*/
	if (isCorrect == 1) {
		fprintf(fout, "Stack: Correct\n");
	} else {
		fprintf(fout, "Stack: Wrong Answer\n");
	}
	fclose(fout);

	return 0;
}
