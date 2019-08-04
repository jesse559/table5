#include <stdlib.h>
#include "p_vector_int.h"

#ifdef DIAGNOSTICS
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#endif

/*
 * int 
 * int
 */

static int mypow(int base, int exp)
{
	int sum = 1;
	for (int i=0; i < exp; i++){
		sum = sum * base;
	}
	return sum;
}

#ifdef DIAGNOSTICS
static double get_time()
{
    struct timeval t;
    struct timezone tzp;
    gettimeofday(&t, &tzp);
    return t.tv_sec + t.tv_usec*1e-6;
}
#endif

bool p_vector_int_new(struct p_vector_int *v, size_t c_size)
{
	v->buffer = (int*) malloc(sizeof(int) * c_size);
	if(!v->buffer)
		return false;
	v->c_size = c_size;
	v->count = 0;
	v->last_offset = 0;
	return true;
}

int p_vector_int_pop(struct p_vector_int *v)
{
	if (v->count == 0)
		return EMPTYint;
	v->count--;
	v->last_offset--;
	return v->buffer[v->last_offset + 1];
}

static bool p_vector_int_double(struct p_vector_int *v)
{	
	int *new_buffer = (int*) malloc(sizeof(int) * v->c_size*2);

	if(!new_buffer){
		return false;
	}

	for (int i=0; i < v->c_size; i++){
		new_buffer[i] = v->buffer[i];
	}
	
	free(v->buffer);
	v->buffer = new_buffer;
	v->c_size = v->c_size*2;
	return true;
}

static bool p_vector_int_nble(struct p_vector_int *v, size_t multiple)
{
	int *new_buffer = (int*) malloc(sizeof(int) * v->c_size*multiple);

	if(!new_buffer){
		return false;
	}

	for (int i=0; i < v->c_size; i++){
		new_buffer[i] = v->buffer[i];
	}
	
	free(v->buffer);
	v->buffer = new_buffer;
	v->c_size = v->c_size*multiple;
	return true;
}

bool p_vector_int_add(struct p_vector_int *v, int val)
{
	if (v->last_offset == v->c_size-1){
		if(!p_vector_int_double(v))
			return false;
	}

	if (v->count == 0){
		v->buffer[0] = val;
		v->count++;
#ifdef DIAGNOSTICS
		printf("lo: %ld, count: %ld\n", v->last_offset, v->count);
#endif
	}else{
		v->buffer[++v->last_offset] = val;
		v->count++;
#ifdef DIAGNOSTICS
		printf("lo: %ld, count: %ld\n", v->last_offset, v->count);
#endif
	}
	return true;
}

bool p_vector_int_add_index(struct p_vector_int *v, size_t index, int val)
{
	if (index >= v->c_size-1){
		int k = 1;
		while (v->c_size * mypow(2, k) <= index)
			k++;
		if(!p_vector_int_nble(v, mypow(2, k)))
			return false;
	}

	if (v->last_offset == v->c_size-1){
		if(!p_vector_int_double(v))
			return false;
	}

	if (index > v->last_offset){
		v->buffer[index] = val;
		v->last_offset = index;
		v->count = index + 1;
	}else{
		if (v->count == 0){
			v->buffer[0] = val;
			v->count++;
		}else{
			size_t i = v->last_offset;
			do {
				v->buffer[i+1] = v->buffer[i];
				i--;
			}while(i > index);

			v->buffer[index] = val;
			v->last_offset++;
			v->count++;
		}
	}
	return true;
}

bool p_vector_int_replace(struct p_vector_int *v, size_t index, int val)
{
	if (index >= v->c_size-1){
		int k = 1;
		while (v->c_size * mypow(2, k) <= index)
			k++;
		if(!p_vector_int_nble(v, mypow(2, k)))
			return false;
	}

	if (v->last_offset == v->c_size-1){
		if(!p_vector_int_double(v))
			return false;
	}

	v->buffer[index] = val;
	if (index > v->last_offset){
		v->last_offset = index;
		v->count = index+1;
	}

	return true;
}

void p_vector_int_free(struct p_vector_int *v)
{
	free(v->buffer);
}
