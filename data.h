
#ifndef _DATA_H_
#define _DATA_H_

#include <stdio.h>
// constant defnitions
#define MAX_STR_LEN 128
#define INIT_SIZE 2

// data definitions
typedef struct footpath footpath_t;


// function prototypes
footpath_t *footpath_read(FILE *f);

void skip_header_line(FILE *f);

int read_string(FILE *f, char *str);

int get_id(footpath_t *footpath);

double get_grade1in(footpath_t *fp);

int cmp_addr(footpath_t *fp, char *query);

footpath_t **add_footpath(footpath_t **fps, footpath_t *fp, int num);

void footpaths_print(FILE *f, footpath_t **fps, int num_found);

void footpath_print(FILE *f, footpath_t *fp);

void free_footpath(footpath_t *fp);

int cmp_grade(footpath_t *fp1, footpath_t *fp2);

footpath_t *get_closest(footpath_t *fp1, footpath_t *fp2, double value);

#endif
