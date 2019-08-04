#ifndef P_STRINGS_H
#define P_STRINGS_H

#include <stdlib.h>
#include <string.h>

#ifndef bool 
#define bool	int
#define true	1
#define false   0
#endif

struct p_stream{
    char *buffer;
    size_t b_size; /* not including the null-terminator */
    size_t pos; /* position of the char to be added */
	bool grow;
};


bool p_stream_init(struct p_stream *s, size_t b_size, bool grow);
bool p_stream_printc(struct p_stream *s, char c);
bool p_stream_prints(struct p_stream *s, const char *str);
void p_stream_free(struct p_stream *s);

#endif
