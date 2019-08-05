#ifndef TABLE5_H
#define TABLE5_H

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

void print_options_set_defaults(struct print_options *po);
bool print_table(char *f_buffer, size_t f_size, const struct print_options *po);

#endif
