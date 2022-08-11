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
    int id = get_id(list->head->footpath);
    return id;

}

