#ifndef _DATA_H_
#define _DATA_H_

#include <stdio.h>
// constant defnitions
#define MAX_STR_LEN 128

// data definitions
typedef struct footpath footpath_t;


// function prototypes
footpath_t *footpath_read(FILE *f);

void skip_header_line(FILE *f);

int read_string(FILE *f, char *str);

int get_id(footpath_t *footpath);

#endif
