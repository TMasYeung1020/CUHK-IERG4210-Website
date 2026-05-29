// lab1-q2.h
// Cannot modify this file
typedef struct _list {
	char word[100][51];	/* The 2D array for words */
	int first, last;	/* Index for the first and last position */
} ArrayList;

/* Initialize the arraylist as an empty list #1 */
/* first and last are set to 0 */
void arraylist_init_empty(ArrayList **list);

/* Initialize the arraylist with an array of n words #2 */
/* The last element of the arr is the last element in the list */
/* Suppose n < 100 */
void arraylist_init(ArrayList **list, char arr[100][51], int n);

/* Insert value to the end of the list #3 #4 */
/* If list is full, then do nothing */
void arraylist_insert(ArrayList *list, char *word);

/* Delete the first word from the list #5 #6 */
/* If list is empty, then do nothing and return null pointer */
/* Return the deleted word for non-empty list */
char* arraylist_deletefirst(ArrayList *list);

/* Delete the last word from the list #7 #8 */
/* If list is empty, then do nothing and return null pointer */
/* Return the deleted word for non-empty list */
char* arraylist_deletelast(ArrayList *list);

/* Return the size of the list #9 */
/* Note: The return value ranges from 0 and 99 inclusively */
int arraylist_size(ArrayList *list);

/* Free the list, if list is not NULL #10 */
/* Assign NULL to the *list */
void arraylist_free(ArrayList **list);

/* The print function print the list in an output string #11 */
/* Example: "haha Hello Yes " */
/* You can assume the number of nodes is not more than 99 */
char* arraylist_print(ArrayList *list);
