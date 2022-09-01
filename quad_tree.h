/* ------------------------ 
Created by Tristan Thomas (tkthomas@student.unimelb.edu.au)
31/08/2022
*
* quad_tree.h: Interface for the quad_tree.c file
* 
 ------------------------ */

#ifndef _QUAD_TREE_H_
#define _QUAD_TREE_H_

#include "data.h"

/* -- Constant definitions - */
#define WHITE 0
#define GREY 1
#define BLACK 2

/* --- Data declarations --- */
typedef struct point_2D point_2D_t;
typedef struct rectangle_2D rectangle_2D_t;
typedef struct qt_node qt_node_t;


/* -- Function prototypes - */
qt_node_t *make_empty_tree(rectangle_2D_t *root_rect);
qt_node_t *create_new_node(footpath_t *footpath, rectangle_2D_t *rect);
rectangle_2D_t *get_root_rect(rectangle_2D_t *rectangle, char **arguments);


#endif
