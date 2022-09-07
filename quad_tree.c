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
#include "linked_list.h"

/* Quad tree structure */
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
    footpathsll_t *footpaths;
    point_2D_t *coords;
    int colour;
};

/* Enum containing all possible quadrants */
enum quadrant {
    SW, NW, NE, SE
};


/* =========================== Insertion functions ========================== */
/* Creates an empty quadtree by allocating memory for root node */
qt_node_t *create_new_node(qt_node_t *og_node, rectangle_2D_t *rect) {
    qt_node_t *new_node = NULL;
    if (!og_node) {
        new_node = (qt_node_t *) malloc(sizeof(*new_node));
        assert(new_node);
        new_node->footpaths = NULL;
    } else {
        new_node = og_node;
    }
    
    new_node->rectangle = rect;

    new_node->SW = new_node->NW = new_node->NE = new_node->SE = NULL;
    new_node->colour = WHITE;
    new_node->coords = NULL;

    return new_node;
}


/* ========================================================================== */
/* Inserts a footpath pointer into the correct quad tree node */
qt_node_t *insert_data(qt_node_t *node, footpathsll_t *footpaths, point_2D_t *coords) {

    /* checks to make sure coordinates are in rectangle */
    assert(in_rectangle(coords, node->rectangle));

    /* footpaths can be inserted here */
    if (node->colour == WHITE) {

        qt_node_t *newnode = create_new_node(node, node->rectangle);
        newnode->footpaths = footpaths;

        newnode->coords = coords;
        newnode->colour = BLACK;
        return newnode;

    } 

    /* node will either be subdivided or footpath preprend to existing linked list */
    if (node->colour == BLACK) {

        /* duplicates footpaths and coords to enable freeing of these structs */
        footpathsll_t *new = clone_fp(footpaths);
        footpathsll_t *old = clone_fp(node->footpaths);
        point_2D_t *new_coords = coord_dup(coords);
        point_2D_t *old_coords = coord_dup(node->coords);
        
        /* prepends fp if inserted coords are the same as the node coords and subdivides if not */
        if (new_coords->x == old_coords->x && new_coords->y == old_coords->y) {

            /* frees everything thats been duplicated or no longer needed */
            old = insert_at_head(old, fp_dup(get_footpath_head(new)));
            if (node->footpaths)
                free_list(node->footpaths);
            node->footpaths = NULL;

            free(new_coords);
            free_list(footpaths);
            new_coords = NULL;
            free(old_coords);
            old_coords = NULL;
            free_list(new);
            free(coords);

            /* replaces footpaths at node with one creates above */
            node->footpaths = old;
            
            
        }
        else {
            if (node->footpaths)
                free_list(footpaths);
            free(coords);
            /* subdivides the node creating 4 children nodes */
            node = subdivide(node, old, new, new_coords, old_coords);
        }
        return node;
    }

    /* keeps traversing tree until a leaf node is found */
    if (node->colour == GREY) {

        if (determine_quadrant(coords, node->rectangle) == NW) {
            node->NW = insert_data(node->NW, footpaths, coords);

        } else if (determine_quadrant(coords, node->rectangle) == NE) {
            node->NE = insert_data(node->NE, footpaths, coords);

        } else if (determine_quadrant(coords, node->rectangle) == SW) {
            node->SW = insert_data(node->SW, footpaths, coords);

        } else if (determine_quadrant(coords, node->rectangle) == SE) {
            node->SE = insert_data(node->SE, footpaths, coords);

        }
    }
    /* will eventually return root node */
    return node;

}


/* ========================================================================== */
/* Subdivides a quadtree node until the two footpath lists are in seperate nodes */
qt_node_t *subdivide(qt_node_t *node, footpathsll_t *fps_old, footpathsll_t *fps_new, 
        point_2D_t *new_coords, point_2D_t *old_coords) {

    /* centre point of rectangle */
    long double centre_x = node->rectangle->bot_left.x + 
        ((node->rectangle->top_right.x - node->rectangle->bot_left.x) / 2);
    long double centre_y = node->rectangle->bot_left.y + 
        ((node->rectangle->top_right.y - node->rectangle->bot_left.y) / 2);

    rectangle_2D_t *sw, *se, *nw, *ne;

    /* creates the rectangle boundaries for all children nodes */
    sw = make_rect(node->rectangle->bot_left, make_point(centre_x, centre_y));
    se = make_rect(make_point(centre_x, node->rectangle->bot_left.y), 
        make_point(node->rectangle->top_right.x, centre_y));
    nw = make_rect(make_point(node->rectangle->bot_left.x, centre_y), 
        make_point(centre_x, node->rectangle->top_right.y));
    ne = make_rect(make_point(centre_x, centre_y), node->rectangle->top_right);

    /* creates all children nodes only if not already created */
    node->NE = create_new_node(node->NE, ne);
    node->NW = create_new_node(node->NW, nw);
    node->SE = create_new_node(node->SE, se);
    node->SW = create_new_node(node->SW, sw);

    /* black node is now an internal node */
    node->colour = GREY;

    /* frees the node footpaths and coords as will no longer be needed as an internal node */
    if (node->footpaths)
        free_list(node->footpaths);
    node->footpaths = NULL;

    free(node->coords);
    node->coords = NULL;

    /* checks if the two fps can be inserted or whether another subdivision can occur */
    if (determine_quadrant(old_coords, node->rectangle) == determine_quadrant(new_coords, node->rectangle)) {

        quadrant_t quadrant = determine_quadrant(old_coords, node->rectangle);
        /* subdivides again depending on coords quadrant */
        switch(quadrant) {
            case NE:
                subdivide(node->NE, fps_old, fps_new, new_coords, old_coords);
                break;
            case NW:
                subdivide(node->NW, fps_old, fps_new, new_coords, old_coords);
                break;
            case SE:
                subdivide(node->SE, fps_old, fps_new, new_coords, old_coords);
                break;
            case SW:
                subdivide(node->SW, fps_old, fps_new, new_coords, old_coords);
                break;   
        }
            
            
    } else {
        /* inserts originally inserted footpath in appropriate node */
        switch(determine_quadrant(new_coords, node->rectangle)) {
            case NE:
                node->NE = insert_data(node->NE, fps_new, new_coords);
                break;
            case NW:
                node->NW = insert_data(node->NW, fps_new, new_coords);
                break;
            case SE:
                node->SE = insert_data(node->SE, fps_new, new_coords);
                break;
            case SW:
                node->SW = insert_data(node->SW, fps_new, new_coords);
                break;   
        }
        /* inserts footpath that was originally in a black node to appropriate node */
        switch(determine_quadrant(old_coords, node->rectangle)) {
            case NW:
                node->NW = insert_data(node->NW, fps_old, old_coords);
                break;
            case NE:
                node->NE = insert_data(node->NE, fps_old, old_coords);
                break;
            case SE:
                node->SE = insert_data(node->SE, fps_old, old_coords);
                break;
            case SW:
                node->SW = insert_data(node->SW, fps_old, old_coords);
                break;   
        }
        
        
    }

    return node;

}


/* ========================== Point query functions ========================= */
/* Searches the quadtree for a single point */
footpathsll_t *search_tree(qt_node_t *node, point_2D_t *query) {

    /* checks to see if query is within bounds and is in quadtree */
    assert(in_rectangle(query, node->rectangle));

    assert(node->colour != WHITE);

    /* footpath in tree */
    if (node->colour == BLACK)
        return node->footpaths;

    /* traverses tree until black node found */
    if (node->colour == GREY) {
        if (determine_quadrant(query, node->rectangle) == NW) {
            printf(" NW");
            return search_tree(node->NW, query);

        } else if (determine_quadrant(query, node->rectangle) == NE) {
            printf(" NE");
            return search_tree(node->NE, query);

        } else if (determine_quadrant(query, node->rectangle) == SW) {
            printf(" SW");
            return search_tree(node->SW, query);

        } else if (determine_quadrant(query, node->rectangle) == SE) {
            printf(" SE");
            return search_tree(node->SE, query);

        }
    }

    return NULL;

}


/* ========================================================================== */
/* Checks that a given point lies within a rectangle region */
int in_rectangle(point_2D_t *point, rectangle_2D_t *rect) {

    return point->x <= rect->top_right.x && point->x > rect->bot_left.x &&
            point->y >= rect->bot_left.y && point->y < rect->top_right.y;

}

/* ========================================================================== */
/* Finds which quadrant of a rectangle region a point lies in */
quadrant_t determine_quadrant(point_2D_t *point, rectangle_2D_t *rect) {

    /* defines centre of givern quadrant */
    long double centre_x = rect->bot_left.x + ((rect->top_right.x - rect->bot_left.x) / 2);
    long double centre_y = rect->bot_left.y + ((rect->top_right.y - rect->bot_left.y) / 2);

    /* creates 4 rectangles from the given quadrant */
    rectangle_2D_t *sw, *se, *nw, *ne;
    sw = make_rect(rect->bot_left, make_point(centre_x, centre_y));
    se = make_rect(make_point(centre_x, rect->bot_left.y), make_point(rect->top_right.x, centre_y));
    nw = make_rect(make_point(rect->bot_left.x, centre_y), make_point(centre_x, rect->top_right.y));
    ne = make_rect(make_point(centre_x, centre_y), rect->top_right);

    /* checks to see which quadrant the point lies in */
    if (in_rectangle(point, sw)) {
        free(sw); free(se); free(nw); free(ne);
        return SW;

    } else if (in_rectangle(point, se)) {
        free(sw); free(se); free(nw); free(ne);
        return SE;

    } else if (in_rectangle(point, nw)) {
        free(sw); free(se); free(nw); free(ne);
        return NW;

    } else if (in_rectangle(point, ne)) {
        free(sw); free(se); free(nw); free(ne);
        return NE;
    }
    return -1;

    
}


/* ========================== Range query functions ========================= */
/* Searches the quadtree for a region and returns all footpath linked lists in an array */
footpathsll_t **range_query(qt_node_t *node, rectangle_2D_t *query, footpathsll_t **fps_found, int *num_found) {

    /* if leaf node containing fps whose point is in query region */
    if (node->colour == BLACK && in_rectangle(node->coords, query)) {
        fps_found = add_footpaths(fps_found, node->footpaths, ++(*num_found));
        return fps_found;
    }

    /* if point is not in query region */
    if (node->colour == BLACK && !in_rectangle(node->coords, query)) {
        return fps_found;
    }

    /* if node rectangle coords overlap with query region then pursue deeper */
    if (node->colour == GREY && rectangle_overlap(node->rectangle, query)) {

        /* if any of nodes children overlap whose nodes are not empty the recurse */
        if (rectangle_overlap(node->SW->rectangle, query) && node->SW->colour != WHITE) {

            printf(" SW");
            fps_found = range_query(node->SW, query, fps_found, num_found);

        }

        if (rectangle_overlap(node->NW->rectangle, query) && node->NW->colour != WHITE) {

            printf(" NW");
            fps_found = range_query(node->NW, query, fps_found, num_found);

        }

        if (rectangle_overlap(node->NE->rectangle, query) && node->NE->colour != WHITE) {

            printf(" NE");
            fps_found = range_query(node->NE, query, fps_found, num_found);

        } 

        if (rectangle_overlap(node->SE->rectangle, query) && node->SE->colour != WHITE) {

            printf(" SE");
            fps_found = range_query(node->SE, query, fps_found, num_found);

        }

    }

    return fps_found;

    
}


/* ========================================================================== */
/* Checks if two rectangle regions overlap */
int rectangle_overlap(rectangle_2D_t *rect1, rectangle_2D_t *rect2) {

    /* overlap logic (allowing overlap on border) */
    return !((rect2->bot_left.x > rect1->top_right.x) || (rect1->bot_left.x > rect2->top_right.x) ||
            (rect2->bot_left.y > rect1->top_right.y) || (rect1->bot_left.y > rect2->top_right.y));

}

/* ============================= Free functions ============================= */
/* Frees the whole quadtree */
void free_tree(qt_node_t *node) {

    /* post order traversal to free tree from leaves upwards */
    if (!node) {
        return;
    }

    free_tree(node->NE);
    free_tree(node->NW);
    free_tree(node->SE);
    free_tree(node->SW);
    free_node(node);
    
}

/* ========================================================================== */
/* Frees a single quadtree node */
void free_node(qt_node_t *node) {

    /* frees all components of node if not null */
    free(node->rectangle);
    node->rectangle = NULL;

    if (node->footpaths != NULL) {
        free_list(node->footpaths);
        node->footpaths = NULL;
    }
    if (node->coords != NULL) {
        free(node->coords);
        node->coords = NULL;
    }
    
    
    free(node);
    
}


/* ===================== Creating and cloning functions ===================== */
/* Converts the command line arguments to a rectangle for the root node */
rectangle_2D_t *get_root_rect(rectangle_2D_t *rectangle, char **arguments) {

    rectangle = (rectangle_2D_t *) malloc(sizeof(*rectangle));
    rectangle->bot_left.x = strtold(arguments[4], NULL);
    rectangle->bot_left.y = strtold(arguments[5], NULL);
    rectangle->top_right.x = strtold(arguments[6], NULL);
    rectangle->top_right.y = strtold(arguments[7], NULL);
    return rectangle;
}

/* ========================================================================== */
/* Returns the type of coords (start or end) the fp is being inserted by */
point_2D_t *get_coord(footpath_t *fp, int point_type) {

    point_2D_t *coord = NULL;
    coord = (point_2D_t *) malloc(sizeof(*coord));
    assert(coord);
    coord->x = (point_type) ? get_fp_coord(fp, END_LON) : get_fp_coord(fp, START_LON);
    coord->y = (point_type) ? get_fp_coord(fp, END_LAT) : get_fp_coord(fp, START_LAT);

    return coord;
}


/* ========================================================================== */
/* Creates a 2D point with long doubles as arguments */
point_2D_t make_point(long double x, long double y) {

    point_2D_t point;
    point.x = x;
    point.y = y;
    
    return point;
}


/* ========================================================================== */
/* Creates a 2D point from doubles returning as pointer */
point_2D_t *make_point_ptr(double x, double y) {

    point_2D_t *point = NULL;
    point = (point_2D_t *) malloc(sizeof(*point));
    point->x = x;
    point->y = y;
    return point;

}

/* ========================================================================== */
/* Creates a rectangle from two 2D points (non pointers) */
rectangle_2D_t *make_rect(point_2D_t bot_left, point_2D_t top_right) {

    rectangle_2D_t *rect = NULL;
    rect = (rectangle_2D_t *) malloc(sizeof(*rect));
    assert(rect);
    rect->bot_left = bot_left;
    rect->top_right = top_right;
    return rect;
}

/* ========================================================================== */
/* Creates a rectangle from two 2D point pointers */
rectangle_2D_t *make_rect_ptrs(point_2D_t *bot_left, point_2D_t *top_right) {

    rectangle_2D_t *rect = NULL;
    rect = (rectangle_2D_t *) malloc(sizeof(*rect));
    assert(rect);
    rect->bot_left = *bot_left;
    rect->top_right = *top_right;
    return rect;
}

/* ========================================================================== */
/* Duplicates coordinates */
point_2D_t *coord_dup(point_2D_t *coord) {

    point_2D_t *copy = NULL;
    copy = (point_2D_t *) malloc(sizeof(*copy));
    copy->x = coord->x;
    copy->y = coord->y;
    return copy;


}

/* ========================================================================== */
