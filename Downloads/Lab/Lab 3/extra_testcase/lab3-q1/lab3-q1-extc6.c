#include"lab3-q1.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

int main(int argc, char *argv[]){

	FILE *fout;
	Queue *q;
	int isCorrect;
	int i;
	int s1, s2, s3;
	
	fout = fopen(argv[1], "w");
	
	/* default, your program is correct*/
	isCorrect = 1;
	
	/* check the function, queue_enqueue()*/
	q = NULL;
	q = malloc(sizeof(Queue));
	
	q->size = 0;
	q->front = MAX_SIZE - 2;
	q->rear = MAX_SIZE - 2;
	
	for (i = 16; i < 20; i++) {
		queue_enqueue(q, i);
	}
	
	
	
	/* if check position of front, rear and elements at front, rear, MAX_SIZE-1, 0*/
	
	if (q->front != MAX_SIZE - 2 || q->rear != 2 || q->size != 4) {
		isCorrect = 0;
	}
	if (q->data[q->front] != 16) isCorrect = 0;
	//if (q->data[q->rear] != 0) isCorrect = 0;
	if (q->data[MAX_SIZE-1] != 17) isCorrect = 0;
	if (q->data[0] != 18) isCorrect = 0;
	
	/* output "Correct" if the queue_enqueue() is correctly implemented*/
	if (isCorrect == 1) {
		fprintf(fout, "Correct\n");
	} else {
		fprintf(fout, "Wrong Answer\n");
	}
	fclose(fout);

	return 0;
}

