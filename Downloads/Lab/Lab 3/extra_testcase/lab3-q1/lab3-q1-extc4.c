#include"lab3-q1.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

int main(int argc, char *argv[]){

	FILE *fout;
	Queue *q;
	int isCorrect;
	int s1, s2, s3;
	
	fout = fopen(argv[1], "w");
	
	/* default, your program is correct*/
	isCorrect = 1;
	
	/* check the function, queue_is_empty()*/
	q = NULL;
	q = malloc(sizeof(Queue));

	q->front = 0;
	q->rear = 11;	
	q->size = 12;
	s1 = queue_is_empty(q);
	
	q->front = 0;
	q->rear = 0;
	q->size = 0;
	s2 = queue_is_empty(q);
	
	q->front = 1;
	q->rear = 13;
	q->size = 13;
	s3 = queue_is_empty(q);
	
	
	
	/* if s1 s2 s3 are correct*/
	if (s1 != 0 || s2 != 1 || s3 != 0) {
		isCorrect = 0;
	}	
	
	/* output "Correct" if the queue_is_empty() is correctly implemented*/
	if (isCorrect == 1) {
		fprintf(fout, "Correct\n");
	} else {
		fprintf(fout, "Wrong Answer\n");
	}
	fclose(fout);

	return 0;
}

