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

void free_list(footpaths_t *fps) {
    node_t *curr, *prev;
    assert(fps != NULL);
    curr = fps->head;

    while (curr != NULL) {
        prev = curr;
        curr = curr->next;
        free_footpath(prev->footpath);
        free(prev);
        
        
    }
    free(fps);

}


void get_sorted_array(footpaths_t *list, node_t **arr) {

    int num = get_array(list, arr);
    node_t *tmp;
    for (int i = 1; i < num; i++) {
        for (int j = i - 1; j >= 0 && cmp_grade(arr[j]->footpath, arr[j + 1]->footpath) == 1; j--) {
            tmp = arr[j];
            arr[j] = arr[j + 1];
            arr[j + 1] = tmp;
        }
    }

}

int get_array(footpaths_t *list, node_t **arr) {
    node_t *curr = list->head;
    int cnt = 0;
    arr[cnt++] = curr;
    
    while (curr != NULL) {
        arr[cnt] = curr->next;
        cnt++;
        curr = curr->next;
    }

    return cnt - 1;
}


footpath_t *binary_search(node_t **arr, double query, int num) {

    if (query <= get_grade1in(arr[0]->footpath)) {
        return arr[0]->footpath;
        
    }
    if (query >= get_grade1in(arr[num - 1]->footpath)) {
        return arr[num - 1]->footpath;
    }

	int start_index = 0, end_index = num;
    int middle = 0;

    while (start_index < end_index){

		middle = (start_index + end_index) / 2;

		if (query == get_grade1in(arr[middle]->footpath))
			return arr[middle]->footpath;

		if (get_grade1in(arr[middle]->footpath) > query) {
			if (middle > 0 && query > get_grade1in(arr[middle - 1]->footpath))
                return get_closest(arr[middle - 1]->footpath, arr[middle]->footpath, query);
        end_index = middle;        
        }
		else {
            if (middle < num - 1 && query < get_grade1in(arr[middle + 1]->footpath))
                return get_closest(arr[middle]->footpath, arr[middle + 1]->footpath, query);
            // update i
            start_index = middle + 1;

        }
    }

    return arr[middle]->footpath;
}


