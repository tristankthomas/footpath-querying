/* ------------------------ 
Created by Tristan Thomas (tkthomas@student.unimelb.edu.au)
31/08/2022
*
* quad_tree.c: Includes the functions and declarations related to the quad_tree
* 
*
 ------------------------ */

/* Quad tree definition */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "quad_tree.h"
#include "data.h"

struct point_2D {
    long double x;
    long double y;
};

struct rectangle_2D {
    point_2D_t bot_left;
    point_2D_t top_right;
};

struct qt_node {
    rectangle_2D_t *rectangle;
    qt_node_t *SW, *NW, *NE, *SE;
    footpath_t *footpath;
    int colour;
};

/* =========================== Quad tree functions ========================== */
/* Creates an empty quadtree by allocating memory for root node */

qt_node_t *create_new_node(footpath_t *footpath, rectangle_2D_t *rect) {
    qt_node_t *new_node = NULL;
    new_node = (qt_node_t *) malloc(sizeof(*new_node));
    assert(new_node);
    new_node->footpath = footpath;
    new_node->rectangle = rect;

    new_node->SW = new_node->NW = new_node->NE = new_node->SE = NULL;

    return new_node;
}

qt_node_t *make_empty_tree(rectangle_2D_t *root_rect) {
    qt_node_t *root = create_new_node(NULL, root_rect);
    return root;
}

rectangle_2D_t *get_root_rect(rectangle_2D_t *rectangle, char **arguments) {
    rectangle = (rectangle_2D_t *) malloc(sizeof(*rectangle));
    rectangle->bot_left.x = strtold(arguments[4], NULL);
    rectangle->bot_left.y = strtold(arguments[5], NULL);
    rectangle->top_right.x = strtold(arguments[6], NULL);
    rectangle->top_right.y = strtold(arguments[7], NULL);
    return rectangle;
}


