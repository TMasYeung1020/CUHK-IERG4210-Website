#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<ctype.h>

int compare(char *A, char *B) {
	// Your code here
	
	
	return 1;
}

void insertionsort(int n, char ** a, FILE *fout){     
    // Your code here 
	
	
}


int main(int argc, char *argv[]) {
	
	FILE *fin, *fout;
	int n;
	char **data;
	int i;

	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	
	fscanf(fin, "%d", &n);
	data = (char**) malloc(sizeof(char*)*n);
	printf("n = %d\n", n);
	for (i = 0; i < n; i++) {
		data[i] = (char*)malloc(sizeof(char)*100);
		memset(data[i], 0, sizeof(char)*100);
		fscanf(fin, "%s", data[i]);
	}
	
	insertionsort(n, data, fout);
	
	for (i = 0; i < n; i++) {
		free(data[i]);
	}
	free(data);
	
	
	fclose(fin);
	fclose(fout);
	
	return 0;
}

