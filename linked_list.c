/* ------------------------ 
Created by Tristan Thomas (tkthomas@student.unimelb.edu.au)
10/08/2022
*
* linked_list.c: Includes the functions and declarations related to the linked list
* 
* The functions make_empty_list, insert_at_head and free_list were adapted from 
* Alistair Moffat textbook, chapter 11.
*
 ------------------------ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "linked_list.h"
#include "data.h"

/* Linked list definition */
struct node {
    footpath_t *footpath;
    node_t *next;
};

struct footpaths_ll {
    node_t *head;
    node_t *foot;
};

/* ========================== Linked list functions ========================= */
/* Allocates and initialises an empty linked list */
footpathsll_t *make_empty_list(void) {

    footpathsll_t *list;

    /* allocates memory for linked list (2 ptrs - 16 bytes) */
    list = (footpathsll_t *) malloc(sizeof(*list));
    assert(list != NULL);
    list->head = list->foot = NULL;

    return list;
}

/* ========================================================================== */
/* Inserts an element at the head of the list */
footpathsll_t *insert_at_head(footpathsll_t *list, footpath_t *footpath) {

    node_t *new;

    /* allocates memory for new node (2 ptrs - 16 bytes) */
    new = (node_t *) malloc(sizeof(*new));
    assert(list != NULL && new != NULL);
    new->footpath = footpath;
    new->next = list->head;
    list->head = new;

    /* if first element in list */
    if (list->foot == NULL) {
        list->foot = new;
    }

    return list;
}

/* ========================================================================== */
/* Frees the linked list including the individual nodes and the footpath structs */
void free_list(footpathsll_t *fps) {

    assert(fps != NULL);
    node_t *curr, *prev;

    curr = fps->head;

    /* iterates through linked list */
    while (curr != NULL) {

        prev = curr;
        curr = curr->next;
        /* frees footpath */
        free_footpath(prev->footpath);
        /* frees node */
        free(prev);
        
    }

    /* frees linked list */
    free(fps);

}


/* ========================= Sorted array functions ========================= */
/* Produces an array from the linked list and sorts it using insertion sort.
 * The insertion sort component was adapted from Alistair Moffat textbook
 * chapter 7. */
void get_sorted_array(footpathsll_t *list, node_t **arr) {

    int num = get_array(list, arr);
    node_t *tmp;

    /* performs insertion sort on array */
    for (int i = 1; i < num; i++) {
        for (int j = i - 1; j >= 0 && cmp_grade(arr[j]->footpath, arr[j + 1]->footpath) == 1; j--) {
            tmp = arr[j];
            arr[j] = arr[j + 1];
            arr[j + 1] = tmp;
        }
    }
}


/* ========================================================================== */
/* Produces array from linked list */
int get_array(footpathsll_t *list, node_t **arr) {

    node_t *curr = list->head;
    int cnt = 0;
    /* increments count after assigning first element of array */
    arr[cnt++] = curr;
    
    /* iterates through linked list */
    while (curr != NULL) {
        arr[cnt++] = curr->next;
        curr = curr->next;
    }

    return cnt - 1;
}

/* ============================ Search functions ============================ */
/* Linearly searches the linked list and returns the footpath based on the query */
footpath_t **linked_list_search(footpathsll_t *fps, char *query, int *num_found) {

    footpath_t **fps_found = NULL;
    node_t *curr = fps->head;

    *num_found = 0;

    /* iterate through linked list */
    while (curr != NULL) {
        /* adds footpath to array if matches query */
        if (cmp_addr(curr->footpath, query) == 0) {
            (*num_found)++;
            fps_found = add_footpath(fps_found, curr->footpath, *num_found);
            
        }
        curr = curr->next;
    }
    
    return fps_found;

}

/* ========================================================================== */
/* Finds the footpath with the grade1in value closest to the query using binary search 
 *   of the sorted array 
 * This function was taken from the webpage:
 *      https://www.geeksforgeeks.org/find-closest-number-array/
 * and adapted to work the grade1in attribute */
footpath_t *binary_search(node_t **arr, double query, int num) {

	int start_index = 0, end_index = num;
    int middle = 0;

    /* checks if the query is within the edge footpaths of the sorted array */
    if (query <= get_grade1in(arr[0]->footpath))
        return arr[0]->footpath;

    if (query >= get_grade1in(arr[num - 1]->footpath))
        return arr[num - 1]->footpath;


    while (start_index < end_index) {
        /* middle index updated */
		middle = (start_index + end_index) / 2;

        /* returns if exact match is found */
		if (query == get_grade1in(arr[middle]->footpath))
			return arr[middle]->footpath;

        /* if query in left subarray */
		if (get_grade1in(arr[middle]->footpath) > query) {

            /* if query is between the middle and one left from the middle returns
                the closest footpath of the two */
			if (middle > 0 && query > get_grade1in(arr[middle - 1]->footpath))
                return get_closest(arr[middle - 1]->footpath, arr[middle]->footpath, query);

        end_index = middle;

        /* if query in right subarray */
        } else {

            /* if query is between the middle and one right from middle returns the
                closest footpath of the two */
            if (middle < num - 1 && query < get_grade1in(arr[middle + 1]->footpath))
                return get_closest(arr[middle]->footpath, arr[middle + 1]->footpath, query);

        start_index = middle + 1;

        }
    }

    /* returns the remaining footpath */
    return arr[middle]->footpath;
}

/* ========================================================================== */
