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
	char *s0, *s1, *s3, *s4;
	int s2;
	char ans0[10] = "(top)";
	char ans1[100] = "(top) C A B A";
	char ans3[100] = "(top) A B A";


	fout = fopen(argv[1], "w");

	/* default, your program is correct*/
	isCorrect = 1;

	stack_init(&s);

	s0 = stack_print(s);

	stack_push(s, 'A');
	stack_push(s, 'B');
	stack_push(s, 'A');
	stack_push(s, 'C');

	s1 = stack_print(s);

	stack_pop(s);

	s2 = stack_top(s);

	s3 = stack_print(s);


	//stack_free(&s);


	/* if the four string match, s2 is correct*/

	if (strlen(s0) != strlen(ans0) || strncmp(s0, ans0, strlen(s0)) != 0) {
		isCorrect = 0;
	}
	if (strlen(s1) != strlen(ans1) || strncmp(s1, ans1, strlen(s1)) != 0) {
		isCorrect = 0;
	}
	if (strlen(s3) != strlen(ans3) || strncmp(s3, ans3, strlen(s3)) != 0) {
		isCorrect = 0;
	}
	if (s2 != 65) {
		isCorrect = 0;
	}


	/* output "Correct" if the library is correctly implemented*/
	if (isCorrect == 1) {
		fprintf(fout, "Stack: Correct\n");
	} else {
		fprintf(fout, "Stack: Wrong Answer\n");
	}
	fclose(fout);

	return 0;
}
