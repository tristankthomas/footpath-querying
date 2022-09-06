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

// combine these two structs some how
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

enum quadrant {
    SW, NW, NE, SE
};

// will need to remove struct for ll and footpath (only here to make accessing variables easier)
struct footpath {
    int footpath_id;
    char *address;
    char *clue_sa;
    char *asset_type;
    double delta_z;
    double distance;
    double grade1in;
    int mcc_id;
    int mccid_int;
    double rlmax;
    double rlmin;
    char *segside;
    int statusid;
    int streetid;
    int street_group;
    double start_lat;
    double start_lon;
    double end_lat;
    double end_lon;
};

struct node {
    footpath_t *footpath;
    node_t *next;
};

struct footpaths_ll {
    node_t *head;
    int num_items;
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
            old = insert_at_head(old, fp_dup(new->head->footpath));
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
            node->NW = insert_data(node->NW, footpaths, coords); // if next quadrant is leaf node will either subdivide or insert

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
qt_node_t *subdivide(qt_node_t *node, footpathsll_t *fps_old, footpathsll_t *fps_new, point_2D_t *new_coords, point_2D_t *old_coords) {

    /* centre point of rectangle */
    long double centre_x = node->rectangle->bot_left.x + ((node->rectangle->top_right.x - node->rectangle->bot_left.x) / 2);
    long double centre_y = node->rectangle->bot_left.y + ((node->rectangle->top_right.y - node->rectangle->bot_left.y) / 2);

    rectangle_2D_t *sw, *se, *nw, *ne;

    /* creates the rectangle boundaries for all children nodes */
    sw = make_rect(node->rectangle->bot_left, make_point(centre_x, centre_y));
    se = make_rect(make_point(centre_x, node->rectangle->bot_left.y), make_point(node->rectangle->top_right.x, centre_y));
    nw = make_rect(make_point(node->rectangle->bot_left.x, centre_y), make_point(centre_x, node->rectangle->top_right.y));
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
// check that the point lies in the coords and return true if so
int in_rectangle(point_2D_t *point, rectangle_2D_t *rect) {
    return point->x <= rect->top_right.x && point->x > rect->bot_left.x &&
            point->y >= rect->bot_left.y && point->y < rect->top_right.y;

}

/* ========================================================================== */
// call in_rectangle for all four quadrants (define rects) and return the quadrant from enum
quadrant_t determine_quadrant(point_2D_t *point, rectangle_2D_t *rect) {

    long double centre_x = rect->bot_left.x + ((rect->top_right.x - rect->bot_left.x) / 2);
    long double centre_y = rect->bot_left.y + ((rect->top_right.y - rect->bot_left.y) / 2);

    rectangle_2D_t *sw, *se, *nw, *ne;
    sw = make_rect(rect->bot_left, make_point(centre_x, centre_y));
    se = make_rect(make_point(centre_x, rect->bot_left.y), make_point(rect->top_right.x, centre_y));
    nw = make_rect(make_point(rect->bot_left.x, centre_y), make_point(centre_x, rect->top_right.y));
    ne = make_rect(make_point(centre_x, centre_y), rect->top_right);

    if (in_rectangle(point, sw)) {
        free(sw);
        free(se);
        free(nw);
        free(ne);
        return SW;
    } else if (in_rectangle(point, se)) {
        free(sw);
        free(se);
        free(nw);
        free(ne);
        return SE;
    } else if (in_rectangle(point, nw)) {
        free(sw);
        free(se);
        free(nw);
        free(ne);
        return NW;
    } else if (in_rectangle(point, ne)) {
        free(sw);
        free(se);
        free(nw);
        free(ne);
        return NE;
    }
    return -1;

    
}


/* ========================== Range query functions ========================= */
footpathsll_t **range_query(qt_node_t *node, rectangle_2D_t *query, footpathsll_t **fps_found, int *num_found) {
    //static int is_found = 0;

    if (node->colour == BLACK && in_rectangle(node->coords, query)) {
        fps_found = add_footpaths(fps_found, node->footpaths, ++(*num_found));
        //is_found = 1;
        return fps_found;
    }

    if (node->colour == BLACK && !in_rectangle(node->coords, query)) {
        //is_found = 1;
        return fps_found;
    }

    if (node->colour == WHITE) {
        //is_found = 0;
        return fps_found;
    }

    if (node->colour == GREY && rectangle_overlap(node->rectangle, query)) {
        if (rectangle_overlap(node->SW->rectangle, query)) {
            if (is_black_node(node->SW)) 
                printf(" SW");
            fps_found = range_query(node->SW, query, fps_found, num_found);

        }

        if (rectangle_overlap(node->NW->rectangle, query)) {
            if (is_black_node(node->NW)) 
                printf(" NW");
            fps_found = range_query(node->NW, query, fps_found, num_found);

        }
        if (rectangle_overlap(node->NE->rectangle, query)) {
            if (is_black_node(node->NE)) 
                printf(" NE");
            fps_found = range_query(node->NE, query, fps_found, num_found);

        } 

        if (rectangle_overlap(node->SE->rectangle, query)) {
            if (is_black_node(node->SE)) 
                printf(" SE");
            fps_found = range_query(node->SE, query, fps_found, num_found);

        }

    }
    //is_found = 0;

    return fps_found;

    
}

int is_black_node(qt_node_t *node) {
    static int is_black = 0;
    static int count = 0;

    if (node == NULL) {
        return 0;
    }

    if (node->colour == BLACK) {
        if (count == 0) {
            is_black = 0;
            return 1;
        }
        is_black = 1;
        return 1;
    }

    is_black_node(node->NE);
    count = 1;
    is_black_node(node->NW);
    count = 1;
    is_black_node(node->SE);
    count = 1;
    is_black_node(node->SW);
    count = 1;
    
    if (is_black == 1) {
        is_black = 0;
        count = 0;
        return 1;
    }
    count = 0;
    return 0;
}



int rectangle_overlap(rectangle_2D_t *rect1, rectangle_2D_t *rect2) {
    /* overlap logic (allowing overlap on border) */

    return !((rect2->bot_left.x > rect1->top_right.x) || (rect1->bot_left.x > rect2->top_right.x) ||
            (rect2->bot_left.y > rect1->top_right.y) || (rect1->bot_left.y > rect2->top_right.y));

}



/* ============================= Free functions ============================= */
void free_tree(qt_node_t *node) {
    if (!node) {
        return;
    }

    free_tree(node->NE);
    free_tree(node->NW);
    free_tree(node->SE);
    free_tree(node->SW);
    free_node(node);
    
}


void free_node(qt_node_t *node) {
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

// void free_rect(rectangle_2D_t *rect) {
//     free(rect->top_right);
//     free(rect->bot_left);
//     free(rect);
// }


/* ===================== Creating and cloning functions ===================== */
rectangle_2D_t *get_root_rect(rectangle_2D_t *rectangle, char **arguments) {
    rectangle = (rectangle_2D_t *) malloc(sizeof(*rectangle));
    rectangle->bot_left.x = strtold(arguments[4], NULL);
    rectangle->bot_left.y = strtold(arguments[5], NULL);
    rectangle->top_right.x = strtold(arguments[6], NULL);
    rectangle->top_right.y = strtold(arguments[7], NULL);
    return rectangle;
}

/* ========================================================================== */
point_2D_t *get_coord(footpath_t *fp, int point_type) {
    point_2D_t *coord = NULL;
    coord = (point_2D_t *) malloc(sizeof(*coord));
    assert(coord);
    coord->x = (point_type) ? fp->end_lon : fp->start_lon;
    coord->y = (point_type) ? fp->end_lat : fp->start_lat;

    return coord;
}


/* ========================================================================== */
// merge these two functions
point_2D_t make_point(long double x, long double y) {
    point_2D_t point;
    point.x = x;
    point.y = y;
    
    return point;
}


point_2D_t *make_point_ptr(double x, double y) {
    point_2D_t *point = NULL;
    point = (point_2D_t *) malloc(sizeof(*point));
    point->x = x;
    point->y = y;
    return point;

}

/* ========================================================================== */
rectangle_2D_t *make_rect(point_2D_t bot_left, point_2D_t top_right) {
    rectangle_2D_t *rect = NULL;
    rect = (rectangle_2D_t *) malloc(sizeof(*rect));
    assert(rect);
    rect->bot_left = bot_left;
    rect->top_right = top_right;
    return rect;
}

rectangle_2D_t *make_rect_ptrs(point_2D_t *bot_left, point_2D_t *top_right) {
    rectangle_2D_t *rect = NULL;
    rect = (rectangle_2D_t *) malloc(sizeof(*rect));
    assert(rect);
    rect->bot_left = *bot_left;
    rect->top_right = *top_right;
    return rect;
}

/* ========================================================================== */
point_2D_t *coord_dup(point_2D_t *coord) {
    point_2D_t *copy = NULL;
    copy = (point_2D_t *) malloc(sizeof(*copy));
    copy->x = coord->x;
    copy->y = coord->y;
    return copy;


}

/* ========================================================================== */
