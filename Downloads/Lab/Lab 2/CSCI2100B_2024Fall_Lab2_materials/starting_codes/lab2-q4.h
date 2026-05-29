// lab2-q4.h 
// Cannot modify this file 

// sort the array according to the order specified by comparefunc
// positive return value from comparefunc means swapping is needed
// datatypesize is the size of the data
// e.g. datatypesize = 4 when the input array is an int array
// arraylength is the number of elements to be sorted in array
// after calling the function, the input array becomes sorted
// you can use any sorting algorithm for this problem 
void genericsort(void *array, int datatypesize, int arraylength, int (*comparefunc)(void*,void*));
