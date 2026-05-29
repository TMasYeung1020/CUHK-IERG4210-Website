#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

void merge(int a[], int tmpa[], int l, int r, int rend, int maxlen, FILE *fout){
    int i, t = l, lend, n;
	
	int lend; // what is the value for lend?
	int n; // what is the value for n?
	if (n <= 1) return; /* Only need to merge one element */
    while (/* What is missing here? */)  /* main loop */
        if (a[l] >= a[r]) tmpa[t++] = a[l++];
        else tmpa[t++] = a[r++];
    while (/* What is missing here? */) /* copy rest of first half */
        tmpa[t++] = a[l++];
    while (/* What is missing here? */) /* copy rest of second half */
        tmpa[t++] = a[r++];
    for (i = 0; i < n; i++, rend--) /* copy tmpa back */
        a[rend] = tmpa[rend];
	
	for (i = 0; i < maxlen-1; i++) 	
		fprintf(fout, "%d ", a[i]);
	fprintf(fout, "%d\n", a[maxlen-1]);
}


void msort(int a[], int tmpa[], int left, int right, int maxlen, FILE *fout){
    int center;
    if (left < right){
        center = (left + right) / 2;
        msort(/* What is missing here? */);
        msort(/* What is missing here? */);
        merge(/* What is missing here? */);
    }
}


void mymergesort(int n, int a[], FILE *fout){     /* driver */
    int *tmpa = (int *)malloc(n * sizeof(int)); /* memory for merged list */

    msort(a, tmpa, 0, n - 1, n, fout);
    free(tmpa);
}


int main(int argc, char *argv[]) {
	
	FILE *fin, *fout;
	int n;
	int *data;
	int i;

	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	
	fscanf(fin, "%d", &n);
	data = (int*) malloc(sizeof(int)*n);
	for (i = 0; i < n; i++) fscanf(fin, "%d", data+i);
	
	mymergesort(n, data, fout);
	
	free(data);
	
	
	fclose(fin);
	fclose(fout);
	
	return 0;
}

