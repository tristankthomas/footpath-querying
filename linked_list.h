
#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_
#include "data.h"

// constant defnitions


// data definitions
typedef struct node node_t;
typedef struct footpaths footpaths_t;

// function prototypes
footpaths_t *make_empty_list(void);

int is_empty_list(footpaths_t *list);

void free_list(footpaths_t *list);

footpaths_t *insert_at_head(footpaths_t *list, footpath_t *footpath);

int get_head_id(footpaths_t *list);

// void print_addrs(footpaths_t *list);

footpath_t **linked_list_search(footpaths_t *fps, char *query, int *num_found);

void free_list(footpaths_t *fps);

void get_sorted_array(footpaths_t *list, node_t **arr);

int get_array(footpaths_t *list, node_t **arr);

//void node_swap(node_t )

int get_addr(node_t *node);

#endif
