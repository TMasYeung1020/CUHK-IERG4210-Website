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
	char *s1, *s2, *s3;
	char ans1[10] = "(top)";
	char ans2[400] = "(top) C B A";


	fout = fopen(argv[1], "w");

	/* default, your program is correct*/
	isCorrect = 1;

	/* check the function, stack_print()*/
	s = NULL;
	s = malloc(sizeof(Stack));
	s->next = NULL;

	s1 = stack_print(s); /* s1 = ans1*/

	for (i = 65; i <= 67; i++) {
		ptr = NULL;
		ptr = malloc(sizeof(Stack));
		ptr->data = i;
		ptr->next = s->next;
		s->next = ptr;
	}

	s2 = stack_print(s); /* s2 = ans2*/




	/* if the three strings match*/

	if (strlen(s1) != strlen(ans1) || strncmp(s1, ans1, strlen(s1)) != 0) {
		isCorrect = 0;
	}
	if (strlen(s2) != strlen(ans2) || strncmp(s2, ans2, strlen(s2)) != 0) {
		isCorrect = 0;
	}


	/* output "Correct" if the stack_print() is correctly implemented*/
	if (isCorrect == 1) {
		fprintf(fout, "Stack: Correct\n");
	} else {
		fprintf(fout, "Stack: Wrong Answer\n");
	}
	fclose(fout);

	return 0;
}
