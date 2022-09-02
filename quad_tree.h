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

/* - Constant definitions - */
#define WHITE 0
#define GREY 1
#define BLACK 2

/* --- Data declarations --- */
typedef struct point_2D point_2D_t;
typedef struct rectangle_2D rectangle_2D_t;
typedef struct qt_node qt_node_t;

typedef enum quadrant quadrant_t;

/* -- Function prototypes - */
qt_node_t *make_empty_tree(rectangle_2D_t *root_rect);
qt_node_t *create_new_node(rectangle_2D_t *rect);
rectangle_2D_t *get_root_rect(rectangle_2D_t *rectangle, char **arguments);
qt_node_t *insert_data(qt_node_t *node, footpath_t *footpath, point_2D_t *coords);
quadrant_t determine_quadrant(point_2D_t *point, rectangle_2D_t *rect);
int in_rectangle(point_2D_t *point, rectangle_2D_t *rect);
point_2D_t *get_coord(footpath_t *fp, int point_type);
rectangle_2D_t *make_rect(point_2D_t bot_left, point_2D_t top_right);
point_2D_t make_point(long double x, long double y);
qt_node_t *subdivide(qt_node_t *node, footpath_t *fp_old, footpath_t *fp_new, point_2D_t *new_coords, point_2D_t *old_coords);

#endif
