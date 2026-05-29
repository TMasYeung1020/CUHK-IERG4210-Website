// lab4-q1.h 
// Cannot modify this file 

typedef struct node {
	int key;
	int value;
	struct node *next;
} List;

typedef struct hash_sc {
	int size;		// hash table size 
	List ** slots;	// array of linked lists 
} Hash_SC;

// Linked List Part 
// Insert (key, value) to the front of the linked list L #10 
// If key does not exist in T, then add (key, value) to the front of the list 
// If key already exist in T, then update the value 
// Return list head 
List* list_insert(List *L, int key, int value);

// Delete key from the linked list L #10 
// If key does not exist in T, then do nothing 
// Return the head of the list 
List* list_delete(List *L, int key);

// Find the value based on the key in the L #10 
// If key in L, return the value 
// If key does not exist in L, return INT_MAX 
int list_find(List *L, int key);


// Hash Table Separate Chaining Part 
// Initialize the hash table to the given size #1 
// Create a new empty hash table with the given size
void hash_init(Hash_SC ** T, int size);

// Make the hash table T empty #2 
// All slots are pointing to NULL 
void hash_make_empty(Hash_SC *T);

// Insert (key, value) to the hash table T 
// If key does not exist in T, then add (key, value) to the front of the list #3 
// If key already exist in T, then update the value #4 
void hash_insert(Hash_SC *T, int key, int value);

// Delete key from the hash table T #5 
// If key does not exist in T, then do nothing 
void hash_delete(Hash_SC *T, int key);

// Check if key exist in the hash table T or not #6 
// If key in T, return 1 
// If key not in T, return 0 
int hash_contain(Hash_SC *T, int key);

// Find the value based on the key in the hash table T #7 
// If key in T, return the value 
// If key does not exist in T, return INT_MAX 
int hash_find(Hash_SC *T, int key);

// Free the hash table T, if T is not NULL #8 
// Assign NULL to T 
void hash_free(Hash_SC **T);

// The hash function calculates the hash value #9 
// hash_value = key mod size 
// key can be negative, but hash_value must be range between 0 and size - 1 
// e.g. if key = -1, then hash_value will be size - 1
int hash_function(int key, int size);


