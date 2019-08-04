#include <stdio.h>
#include <stdlib.h>
#include "p_strings.h"
#include "p_vector/p_vector_int.h"

#define NEW_ROW -11

#define PO_ALIGN_LEFT		1
#define PO_ALIGN_RIGHT		2
#define PO_ALIGN_CENTER		3

struct print_options{
	bool top_bar;
	bool bottom_bar;
	bool seperate_rows;
	char c_empty;
	char c_seperator;
	char c_edge;
	char c_bar;
	uint margin_left;
	uint margin_right;
	int align_style;
};

void print_options_setdefaults(struct print_options *po)
{
	po->top_bar = true;
	po->bottom_bar = true;
	po->seperate_rows = false;
	po->c_empty = ' ';
	po->c_seperator = '|';
	po->c_edge = '+';
	po->c_bar = '-';
	po->margin_left = 2;
	po->margin_right = 2;
	po->align_style = PO_ALIGN_LEFT;
}

static void p_stream_repeat_char(struct p_stream *s, char c, int n)
{
	for (int i=0; i < n; i++)
		p_stream_printc(s, c);
}

static void print_row_bar(struct p_stream *s, const struct print_options *po, const struct p_vector_int *max_col_width, int col_count)
{
	p_stream_printc(s, po->c_edge);
	for (int i=0; i < col_count; i++){
		p_stream_repeat_char(s, po->c_bar, max_col_width->buffer[i] + po->margin_left + po->margin_right);
		p_stream_printc(s, po->c_edge);
	}
	p_stream_printc(s, '\n');
}

static void print_cell(struct p_stream *s, const struct print_options *po, const char *cell, int max_col_width, int cell_width)
{
	int extra = max_col_width - cell_width;

	p_stream_printc(s, po->c_seperator);

	if (cell){
		switch(po->align_style){
			case PO_ALIGN_RIGHT:
				p_stream_repeat_char(s, po->c_empty, po->margin_left);
				for (int k=0; k < extra; k++)
					p_stream_printc(s, po->c_empty);
				p_stream_prints(s, cell);
				p_stream_repeat_char(s, po->c_empty, po->margin_right);
				break;
			case PO_ALIGN_CENTER:
				p_stream_repeat_char(s, po->c_empty, po->margin_left);
				for (int k=0; k < extra/2; k++)
					p_stream_printc(s, po->c_empty);
				p_stream_prints(s, cell);
				for (int k=0; k < extra/2; k++){
					p_stream_printc(s, po->c_empty);
				}
				if (extra%2)
					p_stream_printc(s, po->c_empty);
				p_stream_repeat_char(s, po->c_empty, po->margin_right);
				break;
			default:
				p_stream_repeat_char(s, po->c_empty, po->margin_left);
				p_stream_prints(s, cell);
				for (int k=0; k < extra; k++)
					p_stream_printc(s, po->c_empty);
				p_stream_repeat_char(s, po->c_empty, po->margin_right);
				break;
		}
	}else{
		p_stream_repeat_char(s, po->c_empty, max_col_width + po->margin_left + po->margin_right);
	}
	
}


bool print_table(char *f_buffer, size_t f_size, const struct print_options *po)
{	
	size_t max_cell = f_size/2;

	struct p_vector_int cell_width;
	struct p_vector_int cell_offset;
	struct p_vector_int max_col_width;

	size_t col_count = 0;
	size_t row_count = 0;

	p_vector_int_new(&cell_width, max_cell);
	p_vector_int_new(&cell_offset, max_cell);
	p_vector_int_new(&max_col_width, max_cell);

	
	p_vector_int_add(&cell_offset, 0);

	int c=0, col=0;
	for (int i=0; i < f_size; i++){
		c++;
		if (f_buffer[i] == '|' || f_buffer[i] == '\n'){
			p_vector_int_add(&cell_width, c-1);
			if (max_col_width.buffer[col] < (c-1))
				p_vector_int_replace(&max_col_width, col, c-1);
			col++;
			if (col > col_count)
				col_count = col;
			c=0;
			if (f_buffer[i] == '\n'){
				col = 0;
				p_vector_int_add(&cell_offset, NEW_ROW);
				row_count++;
			}
			p_vector_int_add(&cell_offset, i+1);
		}
	}	

	// remove empty rows at the end
	int last = f_size-1;
	while (f_buffer[last] == '\n'){
		p_vector_int_pop(&cell_offset);
		last--;
	}

#ifdef DIAGNOSTICS
	// Diagnostic Information
	printf("Cell Offset:\n");
	for (int i=0; i < cell_offset.count; i++){
		if (cell_offset.buffer[i] == NEW_ROW)
			printf("NEWROW, ");
		else
			printf("%d, ", cell_offset.buffer[i]);
		if ((i-1) % 10 == 0 && (i-1) != 0)
			printf("\n");
	}
	printf("\n");

	printf("\n");
	printf("Cell Width:\n");
	for (int i=0; i < cell_width.count; i++){
		if (cell_width.buffer[i] == NEW_ROW)
			printf("NEWROW, ");
		else
			printf("%d, ", cell_width.buffer[i]);
		if ((i-1) % 10 == 0 && (i-1) != 0)
			printf("\n");
	}
	printf("\n");

	printf("\n");
	printf("Max Column Width:\n");
	for (int i=0; i < max_col_width.count; i++){
		if (max_col_width.buffer[i] == NEW_ROW)
			printf("NEWROW, ");
		else
			printf("%d, ", max_col_width.buffer[i]);
		if ((i-1) % 10 == 0 && (i-1) != 0)
			printf("\n");
	}
	printf("\n");

	printf("\n");
	printf("Column Count: %ld\n", col_count);
	printf("Row Count: %ld\n", row_count);
	printf("\n");
#endif
	// Diagnostic Information End
	
	struct p_stream s;
	p_stream_init(&s, f_size*4, true);

	char *cell;
	char old;
	int w=0;
	col=0;

	if (po->top_bar)
		print_row_bar(&s, po, &max_col_width, col_count);

	int row=0;
	for (int i=0; i < cell_offset.count; i++){
		if (cell_offset.buffer[i] == NEW_ROW){
			for (int j=0; j < col_count-col; j++){
				print_cell(&s, po, NULL, max_col_width.buffer[col+j], cell_width.buffer[w]);
			}
			p_stream_printc(&s, po->c_seperator);
			p_stream_printc(&s, '\n');
			if (po->seperate_rows){
				print_row_bar(&s, po, &max_col_width, col_count);
			}else if(++row == 1 && !po->seperate_rows)
				print_row_bar(&s, po, &max_col_width, col_count);
			col = 0;
		}else{
			cell = &f_buffer[cell_offset.buffer[i]];
			old = f_buffer[cell_offset.buffer[i] + cell_width.buffer[w]];
			f_buffer[cell_offset.buffer[i] + cell_width.buffer[w]] = '\0';

			// print the f#@*$g cell
			print_cell(&s, po, cell, max_col_width.buffer[col], cell_width.buffer[w]);

			f_buffer[cell_offset.buffer[i] + cell_width.buffer[w]] = old;
			w++;
			col++;
		}
	}
	
	if(po->bottom_bar && !po->seperate_rows)
		print_row_bar(&s, po, &max_col_width, col_count);

	s.buffer[s.pos] = '\0';
	printf("%s", s.buffer);
	p_vector_int_free(&cell_width);
	p_vector_int_free(&cell_offset);
	p_vector_int_free(&max_col_width);
	p_stream_free(&s);
	return true;
}

int main(int argc, char *argv[])
{
	char *fpath = argv[1];
	FILE *file = fopen(fpath, "r");
	if (!file){
		printf("Couldn't open file\n");
		return -1;
	}

	/* Get file size */
	fseek(file, 0, SEEK_END);
	long f_size = ftell(file);
	rewind(file);

	char *f_buffer = (char*) malloc(sizeof(char) * f_size);
	if (!f_buffer){
		printf("Unable to allocated memory\n");
	}

	size_t result = fread(f_buffer, sizeof(char), f_size, file);
	if (result != f_size){
		free(f_buffer);
		return -1;
	}

	//printing table
	struct print_options po;
	print_options_setdefaults(&po);
	print_table(f_buffer, f_size, &po);

	fclose(file);
	free(f_buffer);
	return 0;
}
