#ifndef VECTOR_H
#define VECTOR_H

typedef struct Vector Vector;

Vector* vector_new(int zero_terminated);
Vector* vector_sized_new(int zero_terminated, unsigned int reserved_size);
Vector* vector_append_val(Vector* vector, void* data);
Vector* vector_prepend_val(Vector* vector, void* data);
Vector* vector_insert_val(Vector* vector, int index, void* data);
Vector* vector_remove_index(Vector* vector, int index);
Vector* vector_remove_index_with_sub(Vector* vector, int index, void (*free_sub)(void*));
Vector* vector_free_dum(Vector* vector);
Vector* vector_clear(Vector* vector, void (*free_fun)(void*));
Vector* vector_free_with_sub(Vector* vector, void (*free_fun)(void*));
Vector* vector_replace_val(Vector* vector, int index, void* data);
Vector* vector_create_copy(Vector* vector);
Vector* vector_sort(Vector* vector);
int vector_unique(Vector* vector);
int vector_unique_without_sorting(Vector* vector);
int vector_find_index(Vector* vector, void* data);
int vector_find_index_seqs(Vector* vector, void* data);
Vector* vector_swap(Vector* vector, int i, int j);
Vector* vector_set_unsorted(Vector* vector);
Vector* vector_set_sorted(Vector* vector);
int vector_is_sorted(Vector* vector);
void** vector_get_data(Vector* vector);
int vector_get_size(Vector* vector);
void print_vector(Vector* vector, void (*fun)(void*));
#endif