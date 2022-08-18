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

/* --- Data definitions --- */
typedef struct node node_t;
typedef struct footpaths footpaths_t;


/* -- Function prototypes - */
/* Allocates and initialises an empty linked list */
footpaths_t *make_empty_list(void);

/* Inserts an element at the head of the list */
footpaths_t *insert_at_head(footpaths_t *list, footpath_t *footpath);

/* Linearly searches the linked list and returns the footpath based on the query */
footpath_t **linked_list_search(footpaths_t *fps, char *query, int *num_found);

/* Frees the linked list including the individual nodes and the footpath structs */
void free_list(footpaths_t *fps);

/* Produces an array from the linked list and sorts it using insertion sort. */
void get_sorted_array(footpaths_t *list, node_t **arr);

/* Produces array from linked list */
int get_array(footpaths_t *list, node_t **arr);

/* Finds the footpath with the grade1in value closest to the query using binary search 
 *   of the sorted array */
footpath_t *binary_search(node_t **arr, double query, int num);

#endif
