#include"lab4-q1.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

int main(int argc, char *argv[]){

	FILE *fout;
	int isCorrect, v1, v2, v3, v4;
	
	fout = fopen(argv[1], "w");
	
	/* default, your program is correct*/
	isCorrect = 1;
	
	/* check the function, hash_function()*/
	v1 = hash_function(1213, 3);
	v2 = hash_function(0, 19);
	v3 = hash_function(1234567890, 119);
	v4 = hash_function(-11234567, 17);
	
	
	/*if hash_function() not implemented appropriately*/
	if (v1 != 1 || v2 != 0) {
		isCorrect = 0;
	} else if (v3 != 10 || v4 != 2) {
		isCorrect = 0;
	}
	
	/* output "Correct" if the hash_function() is correctly implemented*/
	if (isCorrect == 1) {
		fprintf(fout, "Correct\n");
	} else {
		fprintf(fout, "Wrong Answer\n");
	}
	fclose(fout);

	return 0;
}

