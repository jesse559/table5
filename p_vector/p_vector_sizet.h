#ifndef P_VECTOR_sizet_H
#define P_VECTOR_sizet_H

#ifndef bool
#define bool	int
#define true	1
#define false	0
#endif

#define EMPTYsize_t 0

struct p_vector_sizet{
	size_t *buffer;
	size_t last_offset;
	size_t c_size;
	size_t count;
};

bool p_vector_sizet_new(struct p_vector_sizet *v, size_t c_size);
size_t p_vector_sizet_pop(struct p_vector_sizet *v);
bool p_vector_sizet_add(struct p_vector_sizet *v, size_t val);
bool p_vector_sizet_add_index(struct p_vector_sizet *v, size_t index, size_t val);


#endif
