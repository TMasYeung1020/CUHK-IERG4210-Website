#include"lab3-q1.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

int main(int argc, char *argv[]){

	FILE *fout;
	Queue *q;
	int isCorrect;
	int s1, s2;
	
	fout = fopen(argv[1], "w");
	
	/* default, your program is correct*/
	isCorrect = 1;
	
	/* check the function, queue_size()*/
	q = NULL;
	q = malloc(sizeof(Queue));
	
	q->size = 12;
	s1 = queue_size(q);
	
	q->size = 14;
	s2 = queue_size(q);
	
	
	
	/* if s1 s2 are correct*/
	if (s1 != 12 || s2 != 14) {
		isCorrect = 0;
	}	
	
	/* output "Correct" if the queue_size() is correctly implemented*/
	if (isCorrect == 1) {
		fprintf(fout, "Correct\n");
	} else {
		fprintf(fout, "Wrong Answer\n");
	}
	fclose(fout);

	return 0;
}

