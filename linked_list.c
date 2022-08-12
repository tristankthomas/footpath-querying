#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "linked_list.h"
#include "data.h"

struct node {
    footpath_t *footpath;
    node_t *next;
};

struct footpaths {
    node_t *head;
    node_t *foot;
};

footpaths_t *make_empty_list(void) {
    footpaths_t *list;
    list = (footpaths_t *) malloc(sizeof(*list));
    assert(list != NULL);
    list->head = list->foot = NULL;
    return list;
}

footpaths_t *insert_at_head(footpaths_t *list, footpath_t *footpath) {
    node_t *new;
    new = (node_t *) malloc(sizeof(*new));
    assert(list != NULL && new != NULL);
    new->footpath = footpath;
    new->next = list->head;
    list->head = new;
    if (list->foot == NULL) {
        list->foot = new;
    }
    return list;
}

int get_head_id(footpaths_t *list) {
    assert(list != NULL && list->head != NULL);
    
    return get_id(list->head->footpath);

}

void print_addrs(footpaths_t *list) {
    node_t *curr = list->head;
    while (curr != NULL) {
        printf("%s\n", get_addr(curr->footpath));
        curr = curr->next;
    }
    

}

footpath_t **linked_list_search(footpaths_t *fps, char *query, int *num_found) {

    footpath_t **fps_found = NULL;
    node_t *curr = fps->head;
    *num_found = 0;
    
    while (curr != NULL) {

        if (cmp_addr(curr->footpath, query) == 0) {
            (*num_found)++;
            fps_found = add_footpath(fps_found, curr->footpath, *num_found);
            
        }
        curr = curr->next;
    }
    
    return fps_found;




}




