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
	
	/* check the function, queue_init()*/
	q = NULL;
	queue_init(&q);
	
	/* if queue_init() is not implemented appropriately */
	if (q == NULL) {
		isCorrect = 0;
	} else if (q->front != 0 || q->rear != 0 || q->size != 0) {
		isCorrect = 0;
	}	
	
	/* output "Correct" if the queue_init() is correctly implemented */
	if (isCorrect == 1) {
		fprintf(fout, "Correct\n");
	} else {
		fprintf(fout, "Wrong Answer\n");
	}
	fclose(fout);

	return 0;
}

