#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

// You may edit and reuse some code in lab1-q1 or lab1-q2
// We only accept a single c file, no multiple files for 1 question

int main(int argc, char *argv[]) {
	
	FILE *fin, *fout;
	char cmd[300];
	char op[10], song1[101], song2[101];
	int d, t;
		
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	
	// Your code here, some variable declarations and initializations
	
	
	memset(cmd, 0, sizeof(cmd));
	while (fgets(cmd, 300, fin) > 0) {
		memset(op, 0, sizeof(op));
		memset(song1, 0, sizeof(song1));
		memset(song2, 0, sizeof(song2));
		sscanf(cmd, "%s", op);
		
		
		
		if (strcmp(op, "Add\0") == 0) {
			
			sscanf(cmd, "%s%s%d%d", op, song1, &d, &t);
			// Your code here
			
		} else if (strcmp(op, "Insert\0") == 0) {
			
			sscanf(cmd, "%s%s%s%d%d", op, song1, song2, &d, &t);
			// Your code here
			
		} else if (strcmp(op, "Delete\0") == 0) {
			
			sscanf(cmd, "%s%s%d", op, song1, &t);
			// Your code here
			
		} else if (strcmp(op, "Query\0") == 0) {
			
			sscanf(cmd, "%s%d", op, &t);
			// Your code here
			
		} else if (strcmp(op, "Print\0") == 0) {
			
			sscanf(cmd, "%s%d", op, &t);
			// Your code here
			
		} 
		
		memset(cmd, 0, sizeof(cmd));
	}
	
	fclose(fin);
	fclose(fout);
	
	return 0;
}
