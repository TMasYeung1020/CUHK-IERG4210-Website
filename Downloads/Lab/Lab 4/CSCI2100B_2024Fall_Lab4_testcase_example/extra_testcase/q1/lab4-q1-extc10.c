#include"lab4-q1.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

char *print_list(List* L) {
	int n, len;
	char *output;
	List *ptr;
	
	ptr = L;
	n = 0;
	while (ptr != NULL) {
		ptr = ptr->next;
		n++;
	}
	
	len = n*24+3;
	output = malloc(sizeof(char)*len);
	memset(output, 0, sizeof(output));
	
	ptr = L;
	while (ptr != NULL) {
		sprintf(output, "%s%d-%d ", output, ptr->key, ptr->value);
		ptr = ptr->next;
	}
	
	output[strlen(output)-1] = 0;
	return output;
	
}

int main(int argc, char *argv[]){
    
	FILE *fout;
	Hash_SC *T;
	int isCorrect, i;
	List *tmp;
	char *str1, *str2, *str3, *str4, *str5;
	int v1, v2, v3, v4;
	char ans1[100] = "";
	char ans2[100] = "258-444 8-222";
	char ans3[100] = "99-555 149-333 9-111";
	char ans4[100] = "149-333";
	char ans5[100] = "58-888 258-444 8-222";
	
	fout = fopen(argv[1], "w");
	
	/* default, your program is correct*/
	isCorrect = 1;

	/* modified example in the lecture note */
        T = NULL;
	hash_init(&T,10);
	
	hash_insert(T, 9, 111);
	hash_insert(T, 8, 222);
	hash_insert(T, 149, 333);
	hash_insert(T, 258, 444);
	hash_insert(T, 691, 555);
	hash_insert(T, 99, 555);
	
	str1 = print_list(T->slots[4]);
	str2 = print_list(T->slots[8]);
	str3 = print_list(T->slots[9]);

    
	hash_delete(T, 99);
	hash_delete(T, 99);
	hash_delete(T, 9);
	
    
	str4 = print_list(T->slots[9]);
	
	hash_insert(T, 58, 666);
	hash_insert(T, 58, 777);
	hash_insert(T, 58, 888);
	hash_insert(T, 84, 888);
	
	str5 = print_list(T->slots[8]);
	
	v1 = hash_find(T, 49);
	v2 = hash_contain(T, 49);
	v3 = hash_find(T, 58);
	v4 = hash_contain(T, 84);
	
    
	hash_free(&T);
    
	/* if T is not implemented appropriately */
	if (v1 != INT_MAX || v2 != 0) {
		isCorrect = 0;
	} else if (v3 != 888 || v4 != 1) {
		isCorrect = 0;
	} else if (strlen(str1) != strlen(ans1) || strncmp(str1, ans1, strlen(ans1)) != 0) {
		isCorrect = 0;
	} else if (strlen(str2) != strlen(ans2) || strncmp(str2, ans2, strlen(ans2)) != 0) {
		isCorrect = 0;
	} else if (strlen(str3) != strlen(ans3) || strncmp(str3, ans3, strlen(ans3)) != 0) {
		isCorrect = 0;
	} else if (strlen(str4) != strlen(ans4) || strncmp(str4, ans4, strlen(ans4)) != 0) {
		isCorrect = 0;
	} else if (strlen(str5) != strlen(ans5) || strncmp(str5, ans5, strlen(ans5)) != 0) {
		isCorrect = 0;
	}
	
	
	/* output "Correct" if the hash_find() is correctly implemented*/
	if (isCorrect == 1) {
		fprintf(fout, "Correct\n");
	} else {
		fprintf(fout, "Wrong Answer\n");
	}
	fclose(fout);
	
	return 0;
}
