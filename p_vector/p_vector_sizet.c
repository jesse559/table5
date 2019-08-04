#include <stdlib.h>
#include "p_vector_sizet.h"

#ifdef DIAGNOSTICS
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#endif

/*
 * sizet
 * size_t
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

bool p_vector_sizet_new(struct p_vector_sizet *v, size_t c_size)
{
	v->buffer = (size_t*) malloc(sizeof(size_t) * c_size);
	if(!v->buffer)
		return false;
	v->c_size = c_size;
	v->count = 0;
	v->last_offset = 0;
	return true;
}

size_t p_vector_sizet_pop(struct p_vector_sizet *v)
{
	if (v->count == 0)
		return EMPTYsize_t;
	v->count--;
	v->last_offset--;
	return v->buffer[v->last_offset + 1];
}

static bool p_vector_sizet_double(struct p_vector_sizet *v)
{	
	size_t *new_buffer = (size_t*) malloc(sizeof(size_t) * v->c_size*2);

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

static bool p_vector_sizet_nble(struct p_vector_sizet *v, size_t multiple)
{
	size_t *new_buffer = (size_t*) malloc(sizeof(size_t) * v->c_size*multiple);

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

bool p_vector_sizet_add(struct p_vector_sizet *v, size_t val)
{
	if (v->last_offset == v->c_size-1){
		if(!p_vector_sizet_double(v))
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

bool p_vector_sizet_add_index(struct p_vector_sizet *v, size_t index, size_t val)
{
	if (index >= v->c_size-1){
		int k = 1;
		while (v->c_size * mypow(2, k) <= index)
			k++;
		if(!p_vector_sizet_nble(v, mypow(2, k)))
			return false;
	}

	if (v->last_offset == v->c_size-1){
		if(!p_vector_sizet_double(v))
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
