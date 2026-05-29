#include"lab3-q2.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

int main(int argc, char *argv[]){

	FILE *fout;
	Stack *s, *ptr;
	int i, s1;
	int isCorrect;

	fout = fopen(argv[1], "w");

	/* default, your program is correct*/
	isCorrect = 1;

	/* check the function, stack_pop()*/
	s = NULL;
	s = malloc(sizeof(Stack));
	s->next = NULL;


	for (i = 1; i <= 4; i++) {
		ptr = NULL;
		ptr = malloc(sizeof(Stack));
		ptr->data = i;
		ptr->next = s->next;
		s->next = ptr;
	}



	/* if stack structure correct for each pop*/

	for (i = 0; i < 5; i++) {
		stack_pop(s);
		s1 = 0;
		ptr = s->next;
		while (ptr != NULL) {
			if (ptr->data + i + s1 != 3) isCorrect = 0;
			s1++;
			ptr = ptr->next;
		}

	}


	/* output "Correct" if the stack_pop() is correctly implemented*/
	if (isCorrect == 1) {
		fprintf(fout, "Stack: Correct\n");
	} else {
		fprintf(fout, "Stack: Wrong Answer\n");
	}
	fclose(fout);

	return 0;
}
