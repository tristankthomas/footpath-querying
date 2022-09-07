/* ------------------------ 
Created by Tristan Thomas (tkthomas@student.unimelb.edu.au)
10/08/2022
*
* linked_list.h: Interface for the linked_list.c file
* 
 ------------------------ */

#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

#include "data.h"

/* --- Data declarations --- */
typedef struct node node_t;
typedef struct footpaths_ll footpathsll_t;


/* -- Function prototypes - */
/* Allocates and initialises an empty linked list */
footpathsll_t *make_empty_list();

/* Inserts an element at the head of the list */
footpathsll_t *insert_at_head(footpathsll_t *list, footpath_t *footpath);

/* Linearly searches the linked list and returns the footpath based on the query */
footpath_t **linked_list_search(footpathsll_t *fps, char *query, int *num_found);

/* Frees the linked list including the individual nodes and the footpath structs */
void free_list(footpathsll_t *fps);

/* Produces an array from the linked list and sorts it using insertion sort. */
void get_sorted_array(footpathsll_t *list, footpath_t **arr, int (*cmp) (footpath_t *, footpath_t *));

/* Prints the array of footpaths */
void print_array(FILE *f, footpath_t **arr, int num_found);

/* Produces array from linked list */
int get_array(footpathsll_t *list, footpath_t **arr);

/* Adds a footpaths linked list to the dynamic array */
footpathsll_t **add_footpaths(footpathsll_t **fps_list, footpathsll_t *fps, int num);

/* Finds the footpath with the grade1in value closest to the query using binary search 
 * of the sorted array */
footpath_t *binary_search(footpath_t **arr, double query, int num);

/* Returns the number of items a linked list has */
int get_num_items(footpathsll_t *fps);

/* Converts the list of linked lists to an array and sorts it */
footpath_t **to_array(footpathsll_t **fps_list, int num, int *total);

/* Clones a given linked list recursively */
node_t *clone_list(node_t* list);

/* Returns the head footpath of a footpath linked list */
footpath_t *get_footpath_head(footpathsll_t *fps);

/* Clones a given footpath linked list */
footpathsll_t *clone_fp(footpathsll_t *fps);

/* Checks the array for duplicate footpaths */
int id_check(footpath_t **arr, int index, int num_found);

/* Prints the array of footpaths ignoring duplicate footpaths */
void print_array_no_dup(FILE *f, footpath_t **arr, int num_found);

#endif
