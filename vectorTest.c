#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

#define DATA_SIZE 100

static void for_del_integer(void* integer)
{
	free(integer);
}

static void for_print_integer(void* data)
{
	int* tmp = (int*)data;

	printf("%d ", *tmp);
}

static void init_intArray(int** intArray, int size)
{
	int i;
	for (i = 0; i < size; i++) {
		int* num = (int*)calloc(1, sizeof(int));
		if (num) *num = i + 1;
		intArray[i] = num;
	}
}

void run_vector_test(void)
{
	int i;
	Vector* vector = vector_new(0);
	int** intArray = (int**)calloc(DATA_SIZE, sizeof(int *));

	if (!intArray) return;
	init_intArray(intArray, DATA_SIZE);
	
	/* APPEND */
	for (i = 0; i < DATA_SIZE; i++) {
		vector_append_val(vector, intArray[i]);
	}
	print_vector(vector, for_print_integer);
	printf("\n");

	/* REMOVE INDEX */
	printf("Remove index test : 11 should be deleted\n");
	vector = vector_remove_index_with_sub(vector, 10, for_del_integer);
	print_vector(vector, for_print_integer);
	printf("\n");

	/* FREE WITH SUB */
	vector = vector_free_with_sub(vector, for_del_integer);
	print_vector(vector, for_print_integer);

	free(intArray);
}
