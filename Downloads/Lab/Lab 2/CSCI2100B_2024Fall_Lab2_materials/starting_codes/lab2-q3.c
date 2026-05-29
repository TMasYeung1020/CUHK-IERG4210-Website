#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<ctype.h>

int compare(char *A, char *B) {
	// Your code here
	
	
}

void swap(char **a, int x, int y) {
	// Your code here
	
	
}

int median3(char** a, int l, int r){
    int c = (l + r) / 2;
    // Your code here
	
	
    return l; 
}


void my_qsort(char**a, int left, int right, int n, FILE *fout){
    int i, j, p; 
    if (left >= right) return;
    if (left + 1 == right){
        if (compare(/* What is missing here? */) > 0) {
			/* What is missing here? */
		}
		
        return;
    } 
    p = median3(/* What is missing here? */); 
    i = left; 
    j = right;
    while (1){
        while (compare(/* What is missing here? */) <= 0) i++;
        while (compare(/* What is missing here? */) >= 0) j--;
        if (i < j) {
			/* What is missing here? */
		}
        else break;
    } 
    swap(a, j, left);
	
	for (i = 0; i < n-1; i++)
		fprintf(fout, "%s ", a[i]);
	fprintf(fout, "%s\n", a[n-1]);
		
	
    my_qsort(/* What is missing here? */); 
    my_qsort(/* What is missing here? */);
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
	
	my_qsort(data, 0, n-1, n, fout);
	
	for (i = 0; i < n; i++) {
		fprintf(fout, "%s", data[i]);
		if (i != n-1) fprintf(fout, " ");
		else fprintf(fout, "\n");
		free(data[i]);
	}
	free(data);
	
	
	fclose(fin);
	fclose(fout);
	
	return 0;
}

