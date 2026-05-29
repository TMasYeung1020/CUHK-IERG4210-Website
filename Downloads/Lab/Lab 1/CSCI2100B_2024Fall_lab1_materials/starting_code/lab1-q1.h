// lab1-q1.h
// Cannot modify this file
typedef struct _node {
	double value;			/* value stored in the node */
	struct _node * next;	      /* pointer to the next node */
} Node;

/* Initialize the linked list as an empty list #1 */
void list_init_empty(Node **list);

/* Initialize the linked list with an array of n elements #2 */
/* The last element of the arr is the first element in the linked list*/
void list_init(Node **list, double *arr, int n);

/* Insert value to the front of the linked list #3 #4 */
/* Return the head of the linked list */
Node* list_insert(Node *list, double value);

/* Delete value from the linked list #5 #6 #7 */
/* If value is in the list, then delete it */
/* Note: there can be multiple value, you need to delete all of them */
/* If value is not in the list, then do nothing */
/* Return the head of the linked list */
Node* list_delete(Node *list, double value);

/* Check if value exists in the list or not #8 */
/* If value is in the list, return 1 */
/* If value is not in the list, return 0 */
int list_contain(Node *list, double value);

/* Free the list, if list is not NULL #9 */
/* Assign NULL to the *list */
void list_free(Node **list);

/* The print function print the list in an output string #10 */
/* Example: "14.00 15.00 17.50 " */
/* You can assume the number of nodes is not more than 100 */
/* You can assume the largest value is capped by 100 */
char* list_print(Node *list);
