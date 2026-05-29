#include"lab3-q1.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

int main(int argc, char *argv[]){

	FILE *fout;
	Queue *s;
	int i;
	int isCorrect;
	char *s0, *s1, *s3, *s4;
	int s2;
	char ans0[100] = "(front) (rear)";
	char ans1[100] = "(front) 1 5 1 4 (rear)";
	char ans3[100] = "(front) 1 4 (rear)";
	
	
	fout = fopen(argv[1], "w");
	
	/* default, your program is correct*/
	isCorrect = 1;
	
    s = NULL;
	queue_init(&s);
	
	s0 = queue_print(s);
	
	queue_enqueue(s, 1);
	queue_enqueue(s, 5);
	queue_enqueue(s, 1);
	queue_enqueue(s, 4);
	
	s1 = queue_print(s);
	
	queue_dequeue(s);
	
	s2 = queue_dequeue(s); /* s2 = 5*/
	
	s3 = queue_print(s);
	
	queue_dequeue(s);
	
	queue_dequeue(s);
	
	s4 = queue_print(s);
	
	queue_free(&s);
	
	
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
	if (strlen(s4) != strlen(ans0) || strncmp(s4, ans0, strlen(s4)) != 0) {
		isCorrect = 0;
	}
	if (s2 != 5) {
		isCorrect = 0;
	}
	
	
	/* output "Correct" if the library is correctly implemented*/
	if (isCorrect == 1) {
		fprintf(fout, "Correct\n");
	} else {
		fprintf(fout, "Wrong Answer\n");
	}
	fclose(fout);

	return 0;
}

