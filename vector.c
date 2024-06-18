/* SYSTEM HEADERS */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
/* END OF SYSTEM HEADERS */

/* PROJECT HEADERS */
#include "vector.h"
/* END OF PROJECT HEADERS */

/* MACROS */
#define CONST_INITIAL_CAPACITY 10
#define CONST_GROWTH_FACTOR 2
/* END OF MACROS */

struct Vector {
	void** data;
	unsigned int capacity;
	int length;
	unsigned int zero_terminated;
	unsigned int is_sorted;
};

Vector* vector_set_unsorted(Vector* vector)
{
	vector->is_sorted = 0;

	return vector;
}

Vector* vector_set_sorted(Vector* vector)
{
	vector->is_sorted = 1;

	return vector;
}

Vector* vector_sized_new(int zero_terminated, unsigned int reserved_size)
{
	Vector* vector = NULL;

	vector = (Vector*) malloc(sizeof(Vector));
	if (vector) {
		vector->length = 0;
		vector->zero_terminated = zero_terminated;
		vector->capacity = reserved_size;
		vector_set_unsorted(vector);
		vector->data = (void**)malloc(vector->capacity * sizeof(void*));
		if (vector->data) {
			if (vector->zero_terminated) {
				vector->data[0] = NULL;
			}
		}
	}

	return vector;
}

Vector* vector_new(int zero_terminated)
{
	return vector_sized_new(zero_terminated, CONST_INITIAL_CAPACITY + zero_terminated);
}

static void rightShiftVector(Vector* vector, int index)
{
	assert(0 <= index && index < vector->length);

	memmove(&(vector->data[index + 1]), &(vector->data[index]), (vector->length - 1 - index) * sizeof(void*));
}

static int checkAdjacentElements(Vector* vector, int index)
{
	int left = index - 1, right = index + 1;

	if (index == 0) {
		left++;
	} else if (index == vector->length - 1) {
		right--;
	}

	if (vector->data[index] >= vector->data[left] && vector->data[index] <= vector->data[right]) {
		return 1;
	}

	return 0;
}

Vector* vector_insert_val(Vector* vector, int index, void* data)
{
	int data_capacity = vector->capacity - vector->zero_terminated;
	assert(0 <= index && index <= vector->length);
	vector->length++;

	if (vector->length > data_capacity) {
		void** tmp = (void**)realloc(vector->data, CONST_GROWTH_FACTOR * vector->capacity * sizeof(void*));
		if (tmp == NULL) {
			free(vector->data);
		}
		vector->data = tmp;
		vector->capacity *= CONST_GROWTH_FACTOR;
	}
	rightShiftVector(vector, index);
	if (vector->data) {
		vector->data[index] = data;
	}
	if (vector->length != 1) {
		if (checkAdjacentElements(vector, index)) {
			vector_set_unsorted(vector);
		}
	}

	return vector;
}

Vector* vector_append_val(Vector* vector, void* data)
{
	vector_insert_val(vector, vector->length, data);

	return vector;
}

Vector* vector_prepend_val(Vector* vector, void* data)
{
	vector_insert_val(vector, 0, data);

	return vector;
}

static void leftShiftVector(Vector* vector, int index)
{
	assert(0 <= index && index < vector->length);

	memmove(&(vector->data[index]), &(vector->data[index + 1]), (vector->length - index) * sizeof(void*));
}

Vector* vector_remove_index(Vector* vector, int index)
{
	assert(0 <= index && index < vector->length);

	leftShiftVector(vector, index);
	vector->length--;
	if (vector->zero_terminated) {
		vector->data[vector->length] = NULL;
	}

	return vector;
}

Vector* vector_remove_index_with_sub(Vector* vector, int index, void (*free_sub)(void*))
{
	assert(0 <= index && index < vector->length);

	if (free_sub != NULL) {
		(*free_sub)(vector->data[index]);
	}

	leftShiftVector(vector, index);
	vector->length--;
	if (vector->zero_terminated) {
		vector->data[vector->length] = NULL;
	}

	return vector;
}

Vector* vector_free_dum(Vector* vector)
{
	if (vector != NULL) {
		free(vector->data);
		free(vector);
	}

	return NULL;
}

Vector* vector_clear(Vector* vector, void (*free_fun)(void*))
{
	if (free_fun != NULL) {
		int i;

		for (i = 0; i < vector->length; i++) {
			(*free_fun)(vector->data[i]);
		}
	}
	vector->length = 0;
	vector->is_sorted = 0;

	return vector;
}

Vector* vector_free_with_sub(Vector* vector, void (*free_fun)(void*))
{
	vector_clear(vector, free_fun);
	vector_free_dum(vector);

	return NULL;
}

Vector* vector_replace_val(Vector* vector, int i, void* data)
{
	assert(0 <= i && i < vector->length);

	vector->data[i] = data;

	return vector;
}

Vector* vector_swap(Vector* vector, int i, int j)
{
	void* temp;
	assert(0 <= j && j < vector->length);
	assert(0 <= j && j < vector->length);
	
	temp = vector->data[i];
	vector->data[i] = vector->data[j];
	vector->data[j] = temp;
	vector_set_unsorted(vector);

	return vector;
}

Vector* vector_create_copy(Vector* vector)
{
	int i;
	Vector* vectorCopy = vector_sized_new(vector->zero_terminated, vector->capacity);
	vectorCopy->length = vector->length;
	vectorCopy->capacity = vector->capacity;
	vectorCopy->is_sorted = vector->is_sorted;

	for (i = 0; i < vector->length; i++) {
		vectorCopy->data[i] = vector->data[i];
	}

	return vectorCopy;
}

Vector* vector_sort(Vector* vector)
{
	int i, j;

	if (!vector->is_sorted) {
		for (i = 0; i < vector->length; i++) {
			for (j = 0; j < vector->length; j++) {
				if (vector->data[j] > vector->data[j + 1]) {
					vector = vector_swap(vector, j, j + 1);
				}
			}
		}
		vector_set_sorted(vector);
	}

	return vector;
}

static int binarySearch(Vector* vector, void* cand)
{
	int left = 0;
	int right = vector->length - 1;

	while (left <= right) {
		int middle = (left + right) / 2;

		if (vector->data[middle] == cand) {
			return middle;
		} else if (vector->data[middle] < cand) {
			left = middle + 1;
		} else {
			right = middle - 1;
		}
	}

	return -1;
}

int vector_find_index(Vector* vector, void* data)
{
	if (!vector->is_sorted) {
		vector_sort(vector);
	}

	return binarySearch(vector, data);
}

int vector_find_index_seqs(Vector* vector, void* data)
{
	int i;
	
	for (i = 0; i < vector->length; i++) {
		if (vector->data[i] == data) {
			return i;
		}
	}

	return -1;
}

int vector_unique_without_sorting(Vector* vector)
{
	int i;

	for (i = 0; i < vector->length; i++) {
		if (vector->data[i] == vector->data[i + 1]) {
			vector_remove_index(vector, i + 1);
			i--;
		}
	}

	return vector->length;
}

int vector_unique(Vector* vector)
{
	if (!vector->is_sorted) {
		vector_sort(vector);
	}

	vector_unique_without_sorting(vector);

	return vector->length;
}

int vector_get_size(Vector* vector)
{
	return vector->length;
}

void** vector_get_data(Vector* vector)
{
	return vector->data;
}

int vector_is_sorted(Vector* vector)
{
	return vector->is_sorted;
}

void print_vector(Vector* vector, void (*fun)(void*))
{
	int i;

	if (!fun || !vector) return;

	printf("Vector size = %d\n", vector->length);
	printf("Vector capacity = %d\n", vector->capacity);
	printf("Vector is sorted (0 or 1) : %d\n", vector->is_sorted);
	printf("Vector data :\n");
	for (i = 0; i < vector->length; i++) {
		(*fun)(vector->data[i]);
	}

	printf("\n");
}
