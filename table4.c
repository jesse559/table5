#include <stdio.h>
#include <stdlib.h>
#include "p_strings.h"
#include "p_vector/p_vector_sizet.h"

void repeat_char(char c, int n)
{
	for (int i=0; i < n; i++){
		printf("%c", c);
	}
}

void print_cell(struct p_stream *s, char *cell, int max_col_width, int cell_width)
{
	int extra = (max_col_width - cell_width) + 2;
	if (cell){
		p_stream_printc(s, ' ');
		p_stream_printc(s, ' ');
		p_stream_prints(s, cell);
		for (int k=0; k < extra; k++)
			p_stream_printc(s, ' ');
		p_stream_printc(s, '|');
	}else{
		for (int i=0; i < max_col_width+4; i++)
			p_stream_printc(s, ' ');
		p_stream_printc(s, '|');
		
	}
}


bool print_table(char *f_buffer, size_t f_size)
{
	size_t init_max_rows = f_size/2;
	size_t init_max_cols = f_size/2;
	struct p_vector_sizet cell_width;
	struct p_vector_sizet cell_offset;
	struct p_vector_sizet max_col_width;
	size_t col_count = 0;
	size_t row_count = 0;

	size_t max_cols = init_max_cols;
	for (int i=0; i < max_col_width.c_size; i++)
		max_col_width.buffer[i] = 0;

	p_vector_sizet_new(&cell_width, init_max_cols * init_max_rows);
	p_vector_sizet_new(&cell_offset, init_max_cols * init_max_rows);
	p_vector_sizet_new(&max_col_width, init_max_cols);

	int c=0, row=0, col=0;
	for (int i=0; i < f_size; i++){
		c++;
		if (f_buffer[i] == '|' || f_buffer[i] == '\n'){
			p_vector_sizet_add_index(&cell_width, max_cols*row+col, c-1);
			if(max_col_width.buffer[col] < (c-1))
				max_col_width.buffer[col] = c-1;
			col++;
			if (col > col_count)
				col_count = col;
			c = 0;
			if(f_buffer[i] == '\n' || i == f_size-1){
				col = 0;
				row++;
			}
			p_vector_sizet_add_index(&cell_offset, max_cols*row+col, i+1);
		}
	}
	row_count = row;

	
	printf("lastchar:%c\n", f_buffer[f_size-1]);
	printf("Row Size:%ld\nCol Size:%ld\n", row_count, col_count);
	
	printf("Max Col Size:\n");
	for (int i=0; i < col_count; i++){
		printf("%ld,", max_col_width.buffer[i]);
	}
	printf("\n\n");

	// printing cell width
	int w=0;
	for (int i=0; i < row_count; i++){
		for (int j=0; j < col_count; j++){
			w = cell_width.buffer[max_cols*i+j];
			//if (w){
				printf("%d,", w);
			//}
		}
		printf("\n");
	}

	// printing cell offset
 	printf("cell offset:\n");
	for (int i=0; i < row_count; i++){
		for (int j=0; j < col_count; j++){
			int w = cell_offset.buffer[max_cols*i + j];
			printf("%d,", w);
		}
		printf("\n");
	}
	

	struct p_stream s;
	p_stream_init(&s, f_size*f_size);
	char *cell = NULL;
	char old;
	for (int i=0; i < row_count; i++){
		for (int j=0; j < col_count; j++){
			if(cell_width.buffer[max_cols*i + j] != 0){
				cell = &f_buffer[cell_offset.buffer[max_cols*i + j]];
				old = f_buffer[cell_offset.buffer[max_cols*i + j]+cell_width.buffer[max_cols*i + j]];
				f_buffer[cell_offset.buffer[max_cols*i + j]+cell_width.buffer[max_cols*i + j]] = '\0';
			}else{
				cell = NULL;
			}

			print_cell(&s, cell, max_col_width.buffer[j], cell_width.buffer[max_cols*i + j]); 

			f_buffer[cell_offset.buffer[max_cols*i + j]+cell_width.buffer[max_cols*i + j]] = old;
		
		}
		//printf("\n");
		p_stream_printc(&s, '\n');
	}

	printf("%s", s.buffer);
	p_stream_free(&s);

	return true;
}

int main(int argc, char *argv[])
{
	char *fpath = "file.txt";
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

	print_table(f_buffer, f_size);

	fclose(file);
	free(f_buffer);
	return 0;
}
