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

struct pointd_2D {
    double x;
    double y;
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
};

/* =========================== Quad tree functions ========================== */
/* Creates an empty quadtree by allocating memory for root node */

qt_node_t *create_new_node(rectangle_2D_t *rect) {
    qt_node_t *new_node = NULL;
    new_node = (qt_node_t *) malloc(sizeof(*new_node));
    assert(new_node);
    new_node->footpaths = make_empty_list();
    new_node->rectangle = rect;

    new_node->SW = new_node->NW = new_node->NE = new_node->SE = NULL;
    new_node->colour = WHITE;

    return new_node;
}


rectangle_2D_t *get_root_rect(rectangle_2D_t *rectangle, char **arguments) {
    rectangle = (rectangle_2D_t *) malloc(sizeof(*rectangle));
    rectangle->bot_left.x = strtold(arguments[4], NULL);
    rectangle->bot_left.y = strtold(arguments[5], NULL);
    rectangle->top_right.x = strtold(arguments[6], NULL);
    rectangle->top_right.y = strtold(arguments[7], NULL);
    return rectangle;
}

point_2D_t *get_coord(footpath_t *fp, int point_type) {
    point_2D_t *coord = NULL;
    coord = (point_2D_t *) malloc(sizeof(*coord));
    assert(coord);
    coord->x = (point_type) ? fp->end_lon : fp->start_lon;
    coord->y = (point_type) ? fp->end_lat : fp->start_lat;

    return coord;
}

/* Inserts a footpath pointer into the correct quad tree node */
// depending on what is returned from determine_quadrant (which node from enum) will recursively insert node based on coords
// note: if the node is white then it is a leaf node with no data (so can insert)
// if black then a leaf node with data (so need to recursively split up until the two points are in seperate quadrants and create new nodes)
// if grey then need to keep going to find white or black (without creating new nodes) (grey nodes only occur after subdiving black nodes)
qt_node_t *insert_data(qt_node_t *node, footpathsll_t *footpaths, point_2D_t *coords) {


    if (!in_rectangle(coords, node->rectangle)){
         return node;
    }

    if (node->colour == WHITE) {

        qt_node_t *newnode = create_new_node(node->rectangle);
        newnode->footpaths = footpaths;//insert_at_head(newnode->footpaths, footpath);

        newnode->coords = coords;
        newnode->colour = BLACK;
        return newnode;

    } 

    if (node->colour == BLACK) {

        footpathsll_t *new = footpaths;
        footpathsll_t *old = node->footpaths;
        point_2D_t *new_coords = coords;
        point_2D_t *old_coords = node->coords;

        if (new_coords->x == old_coords->x && new_coords->y == old_coords->y)
            old = insert_at_head(old, new->head->footpath);
        else 
            node = subdivide(node, old, new, new_coords, old_coords); // subdivide and store rectangles to each of 4 quadrants (white nodes) (change parent node colour to grey)
        return node;
    }

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

    return node;

}

qt_node_t *subdivide(qt_node_t *node, footpathsll_t *fps_old, footpathsll_t *fps_new, point_2D_t *new_coords, point_2D_t *old_coords) {

    long double centre_x = node->rectangle->bot_left.x + ((node->rectangle->top_right.x - node->rectangle->bot_left.x) / 2);
    long double centre_y = node->rectangle->bot_left.y + ((node->rectangle->top_right.y - node->rectangle->bot_left.y) / 2);


    rectangle_2D_t *sw, *se, *nw, *ne;
    sw = make_rect(node->rectangle->bot_left, make_point(centre_x, centre_y));
    se = make_rect(make_point(centre_x, node->rectangle->bot_left.y), make_point(node->rectangle->top_right.x, centre_y));
    nw = make_rect(make_point(node->rectangle->bot_left.x, centre_y), make_point(centre_x, node->rectangle->top_right.y));
    ne = make_rect(make_point(centre_x, centre_y), node->rectangle->top_right);

    node->NE = create_new_node(ne);
    node->NW = create_new_node(nw);
    node->SE = create_new_node(se);
    node->SW = create_new_node(sw);

    node->colour = GREY;
    node->footpaths = NULL;
    // free ll
    node->coords = NULL;

 //   printf("this is %d\n", determine_quadrant(old_coords, node->rectangle));
 //   printf("this is %d\n", determine_quadrant(new_coords, node->rectangle));
    if (determine_quadrant(old_coords, node->rectangle) == determine_quadrant(new_coords, node->rectangle)) {
        quadrant_t quadrant = determine_quadrant(old_coords, node->rectangle);
            
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
                node->SW =insert_data(node->SW, fps_new, new_coords);
                break;   
        }

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

footpathsll_t *search_tree(qt_node_t *node, point_2D_t *query) {

    if (!in_rectangle(query, node->rectangle))
         return NULL;


    if (node->colour == WHITE)
        return NULL;


    if (node->colour == BLACK)
        return node->footpaths;


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



// check that the point lies in the coords and return true if so
int in_rectangle(point_2D_t *point, rectangle_2D_t *rect) {
    return point->x <= rect->top_right.x && point->x > rect->bot_left.x &&
            point->y >= rect->bot_left.y && point->y < rect->top_right.y;

}

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
        return SW;
    } else if (in_rectangle(point, se)) {
        return SE;
    } else if (in_rectangle(point, nw)) {
        return NW;
    } else if (in_rectangle(point, ne)) {
        return NE;
    }
    return -1;

    
}

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

rectangle_2D_t *make_rect(point_2D_t bot_left, point_2D_t top_right) {
    rectangle_2D_t *rect;
    rect = (rectangle_2D_t *) malloc(sizeof(*rect));
    assert(rect);
    rect->bot_left = bot_left;
    rect->top_right = top_right;
    return rect;
}


