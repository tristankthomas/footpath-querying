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
    int num_items;
};

/* ========================== Linked list functions ========================= */
/* Allocates and initialises an empty linked list */
footpathsll_t *make_empty_list() {

    footpathsll_t *list;

    /* allocates memory for linked list (2 ptrs - 16 bytes) */
    list = (footpathsll_t *) malloc(sizeof(*list));
    assert(list != NULL);

    list->head = NULL;
    list->num_items = 0;

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
    (list->num_items)++;

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
/* Produces an array from the linked list and sorts it using insertion sort from 
 * a given sorting function.
 * The insertion sort component was adapted from Alistair Moffat textbook
 * chapter 7. */
void get_sorted_array(footpathsll_t *list, footpath_t **arr, int (*cmp) (footpath_t *, footpath_t *)) {

    int num = get_array(list, arr);
    footpath_t *tmp;

    /* performs insertion sort on array */
    for (int i = 1; i < num; i++) {
        for (int j = i - 1; j >= 0 && cmp(arr[j], arr[j + 1]) == 1; j--) {
            tmp = arr[j];
            arr[j] = arr[j + 1];
            arr[j + 1] = tmp;
        }
    }
}

/* ========================================================================== */
/* Produces array from linked list */
int get_array(footpathsll_t *list, footpath_t **arr) {

    node_t *curr = list->head;
    int cnt = 0;
       
    /* iterates through linked list */
    while (curr != NULL) {
        arr[cnt++] = curr->footpath;
        curr = curr->next;
    }

    return cnt;
}


/* ========================================================================== */
/* Prints the array of footpaths */
void print_array(FILE *f, footpath_t **arr, int num_found) {

    for (int i = 0; i < num_found; i++) {
        footpath_print(f, arr[i]);
    }


}

/* ========================================================================== */
/* Prints the array of footpaths ignoring duplicate footpaths */
void print_array_no_dup(FILE *f, footpath_t **arr, int num_found) {

    for (int i = 0; i < num_found; i++) {
        if (id_check(arr, i, num_found)) footpath_print(f, arr[i]);
    }
}

/* ========================================================================== */
/* Checks the array for duplicate footpaths */
int id_check(footpath_t **arr, int index, int num_found) {

    /* checks behind the given index and returns true if no duplicates */
    for (int i = index - 1; i >= 0; i--) {

        if (index == 0)
            return 1;

        else if (get_id(arr[i]) == get_id(arr[index])) {
            return 0;
        }
    }
    return 1;
}



/* ========================================================================== */
/* Converts the list of linked lists to an array and sorts it */
footpath_t **to_array(footpathsll_t **fps_list, int num, int *total) {

    footpath_t **fps_found = NULL;
    footpath_t *tmp;
    int tot = 0;
    node_t *curr = NULL;

    /* iterates through array of linked lists */
    for (int i = 0; i < num; i++) {
        curr = fps_list[i]->head;

        /* for each linked list adds all footpaths to new dynamic array */
        while (curr != NULL) {
            fps_found = add_footpath(fps_found, curr->footpath, ++tot);
            curr = curr->next;
        }
        curr = NULL;
        
    }
    
    /* sorts this new dynamic array by footpath ID */
    for (int i = 1; i < tot; i++) {

        for (int j = i - 1; j >= 0 && cmp_id(fps_found[j], fps_found[j + 1]) == 1; j--) {
            tmp = fps_found[j];
            fps_found[j] = fps_found[j + 1];
            fps_found[j + 1] = tmp;
        }
    }
    
    *total = tot;

    return fps_found;

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
footpath_t *binary_search(footpath_t **arr, double query, int num) {

	int start_index = 0, end_index = num;
    int middle = 0;

    /* checks if the query is within the edge footpaths of the sorted array */
    if (query <= get_grade1in(arr[0]))
        return arr[0];

    if (query >= get_grade1in(arr[num - 1]))
        return arr[num - 1];


    while (start_index < end_index) {
        /* middle index updated */
		middle = (start_index + end_index) / 2;

        /* returns if exact match is found */
		if (query == get_grade1in(arr[middle]))
			return arr[middle];

        /* if query in left subarray */
		if (get_grade1in(arr[middle]) > query) {

            /* if query is between the middle and one left from the middle returns
                the closest footpath of the two */
			if (middle > 0 && query > get_grade1in(arr[middle - 1]))
                return get_closest(arr[middle - 1], arr[middle], query);

        end_index = middle;

        /* if query in right subarray */
        } else {

            /* if query is between the middle and one right from middle returns the
                closest footpath of the two */
            if (middle < num - 1 && query < get_grade1in(arr[middle + 1]))
                return get_closest(arr[middle], arr[middle + 1], query);

        start_index = middle + 1;

        }
    }

    /* returns the remaining footpath */
    return arr[middle];
}

/* ========================================================================== */
/* Adds a footpaths linked list to the dynamic array */
footpathsll_t **add_footpaths(footpathsll_t **fps_list, footpathsll_t *fps, int num) {

    /* static variable to keep track of footpath size (not used anywhere else) */
    static int size = INIT_SIZE;

    if (fps_list == NULL) {
        /* allocates initial array memory */
        fps_list = (footpathsll_t **) malloc(size * sizeof(*fps_list));
        assert(fps_list);

    } else if (num >= size) {
        /* expands array */
		size *= 2;
		fps_list = realloc(fps_list, size * sizeof(*fps_list));
		assert(fps_list);
	}

    fps_list[num - 1] = fps;
    return fps_list;
}

/* ====================== Getting and cloning functions ===================== */
/* Returns the number of items a linked list has */
int get_num_items(footpathsll_t *fps) {

    return fps->num_items;

}

/* ========================================================================== */
/* Returns the head footpath of a footpath linked list */
footpath_t *get_footpath_head(footpathsll_t *fps) {
    
    return fps->head->footpath;
}

/* ========================================================================== */
/* Clones a given linked list recursively */
node_t *clone_list(node_t* list) {

    if (list == NULL) 
        return NULL;

    /* creates node and assign og to it and repeats until NULL reached */
    node_t *result = (node_t *) malloc(sizeof(*result));
    result->footpath = fp_dup(list->footpath);
    result->next = clone_list(list->next);
    
    return result;
}

/* ========================================================================== */
/* Clones a given footpath linked list */
footpathsll_t *clone_fp(footpathsll_t *fps) {

    /* makes copy linked list */
    footpathsll_t *copy = make_empty_list();
    copy->head = clone_list(fps->head);
    copy->num_items = fps->num_items;
    return copy;
}



/* ========================================================================== */
