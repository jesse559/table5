#include "p_strings.h"

#ifdef TESTING
#include <stdio.h>
#endif
/*
 *  Initialize p_stream *s.
 *  b_size is in bytes not including the null-terminator
 *  that will be added to the end of the stream.
 * 
 *  return true on success.
 */
bool p_stream_init(struct p_stream *s, size_t b_size, bool grow)
{
    s->buffer = (char*) malloc(sizeof(char) * (b_size + 1));
    if (!s->buffer){
        return false;
    }

    s->buffer[b_size] = '\0';
    s->b_size = b_size;
    s->pos = 0;
	s->grow = grow;
    return true;
}

static bool p_stream_double(struct p_stream *s)
{
	size_t n_size = (s->b_size * 2) + 1;
	char *n_buffer = (char*) malloc(sizeof(char) * n_size);
	if (!s->buffer)
		return false;

	// copy
	for (int i=0; i < s->pos; i++){
		n_buffer[i] = s->buffer[i];
	}

	free(s->buffer);
	s->buffer = n_buffer;
	s->b_size = n_size;
	s->buffer[s->b_size] = '\0';
	return true;
}

bool p_stream_printc(struct p_stream *s, char c)
{
    /* Buffer overflow */
	if (s->pos >= s->b_size){
		if(!p_stream_double(s))
			return false;
	}

    s->buffer[s->pos] = c;
	s->pos++;
    return true;
}

/*
 * str is a null-terminated string.
 */
bool p_stream_prints(struct p_stream *s, const char *str)
{
	size_t len = strlen(str);

    /* Buffer overflow */
	if (s->pos >= s->b_size){
		if(!p_stream_double(s))
			return false;
	}   
    char *dest = &s->buffer[s->pos];
	strcpy(dest, str); 
	s->pos += len;
    return true;
}

/*
 * Frees the buffer.
 * Does not free the *s pointer itself.
 */
void p_stream_free(struct p_stream *s) {
	free(s->buffer);
}

#ifdef TESTING

int main(int argc, char *argv[])
{
	struct p_stream s;
	p_stream_init(&s, 20);
	p_stream_printc(&s, 'A');
	p_stream_prints(&s, "hello");

	s.pos = 0;
	int total2 = s.b_size;
	for (int i=0; i < total2; i++){
		if(!p_stream_printc(&s, 'X'))
			printf("failed on p_stream_printfc\n");
	}

	printf("%s\n", s.buffer);
	/* print as bytes */
	int total = s.b_size + 1;
	for (int i=0; i < total; i++){
		printf("(%d,%c,%d),", i, s.buffer[i], s.buffer[i]);	
		if (i%9 == 0 && i !=0){
			printf("\n");
		}
	}
	printf("\n");
	p_stream_free(&s);
	return 0;
}

#endif
