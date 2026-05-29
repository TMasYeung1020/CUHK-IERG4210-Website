#include"lab3-q1.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

int main(int argc, char *argv[]){

	FILE *fout;
	Queue *q;
	int isCorrect;
	
	fout = fopen(argv[1], "w");
	
	/* default, your program is correct*/
	isCorrect = 1;
	
	/* check the function, queue_free()*/
    q = NULL;
	q = malloc(sizeof(Queue));
	
	q->size = 8;
	q->front = MAX_SIZE - 4;
	q->rear = 4;
	
	for (int i = 0; i < MAX_SIZE; i++) {
		q->data[i] = i+100;
	}

    queue_free(&q);

    if (q != NULL) isCorrect = 0;
	
	/* output "Correct" if the queue_init() is correctly implemented */
	if (isCorrect == 1) {
		fprintf(fout, "Correct\n");
	} else {
		fprintf(fout, "Wrong Answer\n");
	}
	fclose(fout);

	return 0;
}

