/* ------------------------ 
Created by Tristan Thomas (tkthomas@student.unimelb.edu.au)
10/08/2022
*
* driver.c: Contains the main function and functions not specific to the data
*           and the data structures.
* Program use: ./program <dict_type> csv_file output_file
*
* The overall format and structure of this program was inspired by the qStud
* program from workshops W1-W4.
*
 ------------------------ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "data.h"
#include "linked_list.h"
#include "quad_tree.h"

#define DICT1 1
#define DICT2 2
#define DICT3 3
#define DICT4 4

#define START 0
#define END 1

/* -- function prototypes - */
int process_args(int argc, char *argv[]);

footpathsll_t *get_footpath_list(char *filename, int *num);

qt_node_t *get_footpath_tree(char *filename, int *num, char **arguments);

void querying(char **arguments, FILE *input, FILE *output, 
    FILE *out_file, int dict_type);

int read_query(FILE *f, char *query);


/* ============================== Main function ============================= */
int main(int argc, char *argv[]) {

    /* extracts the dictionary type from command line arguments */
    int dict_type = process_args(argc, argv);
    
    FILE *output_file = fopen(argv[3], "w");
    assert(output_file);

    /* creates required data structure and performs query */
    querying(argv, stdin, stdout, output_file, dict_type);

    fclose(output_file);

    return 0;
}


/* ========================================================================== */
/* Processes the command line arguments and returns the dictionary type.
 * This function was adapted from the qStud program in workshops W1-W4 */
int process_args(int argc, char *argv[]) {
    
    /* checks the command line arguments are valid */
	if (argc < 4) {
		fprintf(stderr, "Usage: %s dictionary_type input_file_name output_file_name, where:\n", argv[0]);
		fprintf(stderr, "       \t - dictionary_type is \"1\" for linear search lookup using address and \"2\" "
                        " for binary search lookup using grade1in\n");
		fprintf(stderr, "       \t - input_file_name: data file for building dictionary\n");
		fprintf(stderr, "       \t - output_file_name: a file for outputting statistics for searches\n");
		exit(EXIT_FAILURE);
	}

	return atoi(argv[1]);
}


/* ========================================================================== */
/* Performs most of the functions for querying the list and the sorted array */
void querying(char **arguments, FILE *input, FILE *output, 
        FILE *out_file, int dict_type) {

    int num_found, num_fps = 0;
    char *data_file_name = arguments[2];
    char query[MAX_STR_LEN + 1] = "";

    if (dict_type == 1) {
        /* creates footpath linked list */
        footpathsll_t *footpaths = get_footpath_list(data_file_name, &num_fps);

        footpath_t **footpaths_found = NULL;

        /* reads query input line by line */
        while (read_query(input, query)) {
            /* returns query results into array */
            footpaths_found = linked_list_search(footpaths, query, &num_found);
            
            fprintf(out_file, "%s\n", query);

            if (footpaths_found) {
                /* prints found footpaths if applicable */
                footpaths_print(out_file, footpaths_found, num_found);
                printf("%s --> %d\n", query, num_found);

            } else {
                /* if nothing found */
                printf("%s --> NOTFOUND\n", query);

            }

            free(footpaths_found);

            /* resets query */
            strcpy(query, "");

        }
        free_list(footpaths);

    } else if (dict_type == 2) {
        /* creates footpath linked list */
        footpathsll_t *footpaths = get_footpath_list(data_file_name, &num_fps);
        
        node_t *arr[num_fps];
        footpath_t *footpath_found = NULL;
        double dquery = 0.0;

        /* creates sorted array from linked list */
        get_sorted_array(footpaths, arr);

        /* iterates through query input and stores as string (to keep formatting) */
        while (fscanf(input, "%s", query) == 1) {

            /* converts string query to double */
            dquery = atof(query);
            /* performs binary search on sorted array */
            footpath_found = binary_search(arr, dquery, num_fps);

            fprintf(out_file, "%s\n", query);
            footpath_print(out_file, footpath_found);
            printf("%s --> %.1f\n", query, get_grade1in(footpath_found));

        }
        
    } else if (dict_type == 3) {
        /* creates footpath quad tree */
        qt_node_t *quad_tree = get_footpath_tree(data_file_name, &num_fps, arguments);


    }

    

}

/* ========================================================================== */
/* Reads the footpath data file into a linked list */
footpathsll_t *get_footpath_list(char *filename, int *num) {
    
	FILE *f = fopen(filename, "r");
	assert(f);
    /* creates empty linked list */
	footpathsll_t *footpaths = make_empty_list();

    /* starts reading input file */
	skip_header_line(f);

	footpath_t *fp;
    /* reads input file line by line */
	while ((fp = footpath_read(f))) {

        /* inserts footpath struct at head of linked list */
		footpaths = insert_at_head(footpaths, fp);
        (*num)++;

	}
	
	fclose(f);

	return footpaths;
}


/* ========================================================================== */
/* Reads the footpath data into a quad tree based on longitudes and latitudes */
qt_node_t *get_footpath_tree(char *filename, int *num, char **arguments) {

    rectangle_2D_t *root_rect = NULL;
    root_rect = get_root_rect(root_rect, arguments);

    FILE *f = fopen(filename, "r");
    assert(f);
    /* creates empty quad tree */
    qt_node_t *root = create_new_node(root_rect);

    /* starts reading input file */
	skip_header_line(f);

    footpath_t *fp;

    while ((fp = footpath_read(f))) {
        point_2D_t *start_coord, *end_coord;
        start_coord = get_coord(fp, START);
        end_coord = get_coord(fp, END);
        /* inserts footpath pointer into quadtree */
		root = insert_data(root, fp, start_coord);
        root = insert_data(root, fp, end_coord);
        (*num)++;
    }

    return root;

}

/* ========================================================================== */
/* Reads query input to account for blank queries */
int read_query(FILE *f, char *query) {
	
	char ch;
    /* reads first character of query */
	int valid = fscanf(f, "%c", &ch);

	if (valid == 1) {
		if (ch == '\n') {
            /* if blank query is present */
			query = "";
			return 1;

		} else {
            /* normal query */
			query[0] = ch;
			fscanf(f, "%[^\n]\n", query + 1);
			return 1;

		}
	} else {
		return 0;
	}

	
}

/* ========================================================================== */
