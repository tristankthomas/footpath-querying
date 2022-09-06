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
void get_sorted_array(footpathsll_t *list, node_t **arr);
// GENERALISE COMP FUNCTION
void get_sorted_array2(footpathsll_t *list, footpath_t **arr);

void footpath_printarr(FILE *f, footpath_t **arr, int num_found);

/* Produces array from linked list */
int get_array(footpathsll_t *list, node_t **arr);

int get_array2(footpathsll_t *list, footpath_t **arr);



footpathsll_t *ll_append(footpathsll_t list, footpathsll_t item);

/* Finds the footpath with the grade1in value closest to the query using binary search 
 *   of the sorted array */
footpath_t *binary_search(node_t **arr, double query, int num);

int get_num_items(footpathsll_t *fps);

node_t *clone(node_t* list);

footpathsll_t *clone_fp(footpathsll_t *fps);


//int get_is_freed(footpathsll_t *fps);

#endif
