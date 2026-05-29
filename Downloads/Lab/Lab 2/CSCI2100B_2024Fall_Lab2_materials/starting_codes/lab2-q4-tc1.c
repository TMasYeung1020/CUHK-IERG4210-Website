/* This file is named as lab2-q4-tc1.c */
#include"lab2-q4.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<ctype.h>

int comparefunction(char *A, char *B) {
	return (*B) - (*A);
}

int main(int argc, char **argv) {
	
	FILE *fout = fopen(argv[1], "w");
	char word[10] = "AaBbCc";
	char ans[10] = "ABCabc";
	int dtsize = sizeof(char);
	int arraylength = 6;
	
	/* Assume your program is correct */
	int isCorrect = 1;
	
	/* calling genericsort() */
	genericsort(word, dtsize, arraylength, comparefunction);
	
	/* Test your answer */
	if (strcmp(word, ans) != 0) isCorrect = 0;
	
	/* Output test result */
	if (isCorrect) {
		fprintf(fout, "Sort: Correct\n");
	} else {
		fprintf(fout, "Sort: Wrong Answer\n");
	}
	
	return 0;
}
