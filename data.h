/* ------------------------ 
Created by Tristan Thomas (tkthomas@student.unimelb.edu.au)
10/08/2022
*
* data.h: Interface for the data.c file
* 
 ------------------------ */

#ifndef _DATA_H_
#define _DATA_H_

#include <stdio.h>

/* -- Constant definitions - */
#define MAX_STR_LEN 128
#define INIT_SIZE 2

#define START_LON 1
#define START_LAT 2
#define END_LON 3
#define END_LAT 4

/* --- Data declarations --- */
typedef struct footpath footpath_t;

/* -- Function prototypes - */
/* Reads individual footpath data. */
footpath_t *footpath_read(FILE *f);

/* Skips the first line of the input data file. */
void skip_header_line(FILE *f);

/* Reads the string attributes of the footpath */
int read_string(FILE *f, char *str);

/* Returns a specified footpath variable */
double get_fp_coord(footpath_t *fp, int type);

int get_id(footpath_t *fp);

double get_grade1in(footpath_t *fp);

/* Compares the query to the footpath address */
int cmp_addr(footpath_t *fp, char *query);

/* Adds a footpath to the dynamic array */
footpath_t **add_footpath(footpath_t **fps, footpath_t *fp, int num);

/* Prints an array of footpaths */
void footpaths_print(FILE *f, footpath_t **fps, int num_found);

/* Prints a single footpath */
void footpath_print(FILE *f, footpath_t *fp);

/* Frees the allocated footpath memory including strings */
void free_footpath(footpath_t *fp);

/* Compares grade1in of two footpaths */
int cmp_grade(footpath_t *fp1, footpath_t *fp2);

int cmp_id(footpath_t *fp1, footpath_t *fp2);

footpath_t *fp_dup(footpath_t *fp);

/* Returns the closest footpath based on grade1in value */
footpath_t *get_closest(footpath_t *fp1, footpath_t *fp2, double value);

#endif
