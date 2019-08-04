#ifndef P_VECTOR_CLASSNAME_H
#define P_VECTOR_CLASSNAME_H

#ifndef bool
#define bool	_T
#define true	1
#define false	0
#endif

#define EMPTY_T 0

struct p_vector_CLASSNAME{
	_T *buffer;
	size_t last_offset;
	size_t c_size;
	size_t count;
};

bool p_vector_CLASSNAME_new(struct p_vector_CLASSNAME *v, size_t c_size);
_T p_vector_CLASSNAME_pop(struct p_vector_CLASSNAME *v);
bool p_vector_CLASSNAME_add(struct p_vector_CLASSNAME *v, _T val);
bool p_vector_CLASSNAME_add_index(struct p_vector_CLASSNAME *v, size_t index, _T val);
void p_vector_CLASSNAME_free(struct p_vector_CLASSNAME *v);
bool p_vector_CLASSNAME_replace(struct p_vector_CLASSNAME *v, size_t index, _T val);

#endif
