#include<stdio.h>
#include<stdlib.h>
#include<stdio.h>
#include<math.h>

// You can implement some functions

int main(int argc, char *argv[]) {
	
	FILE *fin, *fout;
	int n, k;
	
	
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	
	fscanf(fin, "%d%d", &n, &k);
	
	// Your code here
	
	
	fclose(fin);
	fclose(fout);
	
	return 0;
}

