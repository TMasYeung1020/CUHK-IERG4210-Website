#include"lab3-q1.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

int main(int argc, char *argv[]){

	FILE *fout;
	Queue *q;
	int i;
	int isCorrect;
	char *s1, *s2;
	char ans1[30] = "(front) (rear)";
	char ans2[100] = "(front) 2019 2020 2021 2022 2023 2024 (rear)";
	
	
	fout = fopen(argv[1], "w");
	
	/* default, your program is correct*/
	isCorrect = 1;
	
	/* check the function, queue_print()*/
	q = NULL;
	q = malloc(sizeof(Queue));
	
	q->front = 0;
	q->size = 0;
	q->rear = 0;
	s1 = queue_print(q); /* s1 = ans1*/
	
	
	for (i = 0; i < MAX_SIZE; i++) {
		q->data[i] = 2019 + i;
	}
	
	q->front = 0;
	q->size = 6;
	q->rear = 6;

	s2 = queue_print(q); /* s2 = ans2*/
	
	
	
	
	/* if the two string match*/
	
	if (strlen(s1) != strlen(ans1) || strncmp(s1, ans1, strlen(s1)) != 0) {
		isCorrect = 0;
	}
	if (strlen(s2) != strlen(ans2) || strncmp(s2, ans2, strlen(s2)) != 0) {
		isCorrect = 0;
	}
	
	
	
	/* output "Correct" if the queue_print() is correctly implemented*/
	if (isCorrect == 1) {
		fprintf(fout, "Correct\n");
	} else {
		fprintf(fout, "Wrong Answer\n");
	}
	fclose(fout);

	return 0;
}

