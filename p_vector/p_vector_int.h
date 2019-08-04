#ifndef P_VECTOR_int_H
#define P_VECTOR_int_H

#ifndef bool
#define bool	int
#define true	1
#define false	0
#endif

#define EMPTYint 0

struct p_vector_int{
	int *buffer;
	size_t last_offset;
	size_t c_size;
	size_t count;
};

bool p_vector_int_new(struct p_vector_int *v, size_t c_size);
int p_vector_int_pop(struct p_vector_int *v);
bool p_vector_int_add(struct p_vector_int *v, int val);
bool p_vector_int_add_index(struct p_vector_int *v, size_t index, int val);
void p_vector_int_free(struct p_vector_int *v);
bool p_vector_int_replace(struct p_vector_int *v, size_t index, int val);

#endif
