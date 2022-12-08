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
#include "linked_list.h"

/* - Constant definitions - */
/* empty leaf node */
#define WHITE 0
/* internal node */
#define GREY 1
/* leaf node with footpath */
#define BLACK 2

/* --- Data declarations --- */
typedef struct point_2D point_2D_t;
typedef struct pointd_2D pointd_2D_t;
typedef struct rectangle_2D rectangle_2D_t;
typedef struct qt_node qt_node_t;

typedef enum quadrant quadrant_t;

/* -- Function prototypes - */
/* Creates an empty quadtree by allocating memory for root node */
qt_node_t *create_new_node(qt_node_t *og_node, rectangle_2D_t *rect);

/* Converts the command line arguments to a rectangle for the root node */
rectangle_2D_t *get_root_rect(rectangle_2D_t *rectangle, char **arguments);

/* Inserts a footpath pointer into the correct quad tree node */
qt_node_t *insert_data(qt_node_t *node, footpathsll_t *footpaths, point_2D_t *coords);

/* Finds which quadrant of a rectangle region a point lies in */
quadrant_t determine_quadrant(point_2D_t *point, rectangle_2D_t *rect);

/* Checks that a given point lies within a rectangle region */
int in_rectangle(point_2D_t *point, rectangle_2D_t *rect);

/* Returns the type of coords (start or end) the fp is being inserted by */
point_2D_t *get_coord(footpath_t *fp, int point_type);

/* Creates a rectangle from two 2D points (non pointers) */
rectangle_2D_t *make_rect(point_2D_t bot_left, point_2D_t top_right);

/* Creates a 2D point with long doubles as arguments */
point_2D_t make_point(long double x, long double y);

/* Creates a 2D point from doubles returning as pointer */
point_2D_t *make_point_ptr(double x, double y);

/* Subdivides a quadtree node until the two footpath lists are in seperate nodes */
qt_node_t *subdivide(qt_node_t *node, footpathsll_t *fps_old, footpathsll_t *fps_new, point_2D_t *new_coords, point_2D_t *old_coords);

/* Searches the quadtree for a single point */
footpathsll_t *search_tree(qt_node_t *node, point_2D_t *query);

/* Frees a single quadtree node */
void free_node(qt_node_t *node);

/* Frees the whole quadtree */
void free_tree(qt_node_t *node);

/* Duplicates coordinates */
point_2D_t *coord_dup(point_2D_t *coord);

/* Checks if two rectangle regions overlap */
int rectangle_overlap(rectangle_2D_t *rect1, rectangle_2D_t *rect2);

/* Searches the quadtree for a region and returns all footpath linked lists in an array */
footpathsll_t **range_query(qt_node_t *node, rectangle_2D_t *query, footpathsll_t **fps_found, int *num_found);

/* Creates a rectangle from two 2D point pointers */
rectangle_2D_t *make_rect_ptrs(point_2D_t *bot_left, point_2D_t *top_right);

#endif
