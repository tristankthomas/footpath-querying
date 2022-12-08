/* ------------------------ 
Created by Tristan Thomas (tkthomas@student.unimelb.edu.au)
10/08/2022 (NOTE: This template was taken from assignment 1 also created by Tristan Thomas)
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

footpathsll_t *get_footpath_list(char *filename);

qt_node_t *get_footpath_tree(char *filename, char **arguments);

void querying(char **arguments, FILE *out_file, int dict_type);

void list_querying(footpathsll_t *footpaths, FILE *out_file);

void array_querying(footpath_t **footpaths, int num_fps, FILE *out_file);

void point_querying(qt_node_t *quad_tree, FILE *out_file);

void range_querying(qt_node_t *quad_tree, FILE *out_file);

int read_query(char *query);


/* ============================== Main function ============================= */
int main(int argc, char *argv[]) {

    /* extracts the dictionary type from command line arguments */
    int dict_type = process_args(argc, argv);
    
    FILE *output_file = fopen(argv[3], "w");
    assert(output_file);

    /* creates required data structure and performs query */
    querying(argv, output_file, dict_type);

    fclose(output_file);

    return 0;
}


/* ========================================================================== */
/* Processes the command line arguments and returns the dictionary type.
 * This function was adapted from the qStud program in workshops W1-W4 */
int process_args(int argc, char *argv[]) {
    
    /* makes sure command line arguments are valid */
	assert(argc < 9);

	return atoi(argv[1]);
}


/* ========================================================================== */
/* Performs most of the functions for querying the list and the sorted array */
void querying(char **arguments, FILE *out_file, int dict_type) {

    char *data_file_name = arguments[2];

    if (dict_type == DICT1) {

        /* creates footpath linked list */
        footpathsll_t *footpaths = get_footpath_list(data_file_name);

        /* performs linked list query */
        list_querying(footpaths, out_file);

        free_list(footpaths);


    } else if (dict_type == DICT2) {

        int num_fps = 0;

        /* creates footpath linked list */
        footpathsll_t *footpaths = get_footpath_list(data_file_name);
        num_fps = get_num_items(footpaths);
        footpath_t *arr[num_fps];

        /* creates sorted array from linked list */
        get_sorted_array(footpaths, arr, cmp_grade);

        /* performs array query */
        array_querying(arr, num_fps, out_file);

        free_list(footpaths);
        

    } else if (dict_type == DICT3) {
       
        /* creates footpath quad tree */
        qt_node_t *quad_tree = get_footpath_tree(data_file_name, arguments);
        
        /* performs quadtree point query */
        point_querying(quad_tree, out_file);

        free_tree(quad_tree);
        quad_tree = NULL;


    } else if (dict_type == DICT4) {

        /* creates footpath quad tree */
        qt_node_t *quad_tree = get_footpath_tree(data_file_name, arguments);

        /* performs quadtree range query */
        range_querying(quad_tree, out_file);

        free_tree(quad_tree);
        quad_tree = NULL;

    }

}

/* ================================= Stage 1 ================================ */
/* Searches for footpath in linked list based on address */
void list_querying(footpathsll_t *footpaths, FILE *out_file) {
        
    int num_found;
    footpath_t **footpaths_found = NULL;
    char query[MAX_STR_LEN + 1] = "";

    /* reads query input line by line */
    while (read_query(query)) {
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

}

/* ================================= Stage 2 ================================ */
/* Uses binary search to find closest footpaths based on grade1in query */
void array_querying(footpath_t **footpaths, int num_fps, FILE *out_file) {

    char query[MAX_STR_LEN + 1] = "";
    double dquery = 0.0;
    footpath_t *footpath_found = NULL;

    /* iterates through query input and stores as string (to keep formatting) */
    while (scanf("%s", query) == 1) {

        /* converts string query to double */
        dquery = atof(query);
        /* performs binary search on sorted array */
        footpath_found = binary_search(footpaths, dquery, num_fps);

        fprintf(out_file, "%s\n", query);
        footpath_print(out_file, footpath_found);
        printf("%s --> %.1f\n", query, get_grade1in(footpath_found));

        footpath_found = NULL;
    }
}

/* ================================= Stage 3 ================================ */
/* Searches the quadtree footpaths for a given coordinate point */
void point_querying(qt_node_t *quad_tree, FILE *out_file) {

    char lon_str[MAX_STR_LEN + 1] = "", lat_str[MAX_STR_LEN + 1] = "";
    double lon_query = 0.0, lat_query = 0.0;
    point_2D_t *point_query;

    footpathsll_t *footpaths_found = NULL;

    while (scanf("%s %s", lon_str, lat_str) == 2) {

        /* converts strings query to double */
        lon_query = atof(lon_str);
        lat_query = atof(lat_str);
        point_query = make_point_ptr(lon_query, lat_query);

        printf("%s %s -->", lon_str, lat_str);
        /* searches query in quadtree */
        footpaths_found = search_tree(quad_tree, point_query);
        printf("\n");

        footpath_t *arr[get_num_items(footpaths_found)];

        // make this function flexible for any attribute
        get_sorted_array(footpaths_found, arr, cmp_id);
        
        fprintf(out_file, "%s %s\n", lon_str, lat_str);
        print_array(out_file, arr, get_num_items(footpaths_found));

        free(point_query);

        footpaths_found = NULL;
            
        }
}

/* ================================= Stage 4 ================================ */
/* Searches the quadtree for all footpaths within a given region */
void range_querying(qt_node_t *quad_tree, FILE *out_file) {

    footpathsll_t **footpaths_found = NULL;
    int num_lists_found, total;
    char rlon_str[MAX_STR_LEN + 1] = "", tlat_str[MAX_STR_LEN + 1] = "", 
        llon_str[MAX_STR_LEN + 1] = "", blat_str[MAX_STR_LEN + 1] = "";
    long double right_lon, top_lat, left_lon, bot_lat;
    point_2D_t *bot_left, *top_right;
    rectangle_2D_t *query;

    footpath_t **arr = NULL;

    while(scanf("%s %s %s %s", llon_str, blat_str, rlon_str, tlat_str) == 4) {

        num_lists_found = 0;
        total = 0;

        /* converts string query to double */
        left_lon = strtold(llon_str, NULL);
        bot_lat = strtold(blat_str, NULL);
        right_lon = strtold(rlon_str, NULL);
        top_lat = strtold(tlat_str, NULL);
        /* converts doubles to point */
        bot_left = make_point_ptr(left_lon, bot_lat);
        top_right = make_point_ptr(right_lon, top_lat);
        /* converts points to rectangle */
        query = make_rect_ptrs(bot_left, top_right);

        printf("%s %s %s %s -->", llon_str, blat_str, rlon_str, tlat_str);
        /* searches quadtree from region query */
        footpaths_found = range_query(quad_tree, query, footpaths_found, &num_lists_found);
        printf("\n");

        /* converts array of linked lists to a dynamic array of footpaths */
        arr = to_array(footpaths_found, num_lists_found, &total);

        fprintf(out_file, "%s %s %s %s\n", llon_str, blat_str, rlon_str, tlat_str);
        print_array_no_dup(out_file, arr, total);

        /* frees query and footpath arrays */
        free(top_right);
        free(bot_left);
        free(query);
        query = NULL;

        free(footpaths_found);
        footpaths_found = NULL;
        free(arr);
        arr = NULL;
        
    }


}

/* ========================================================================== */
/* Reads the footpath data file into a linked list */
footpathsll_t *get_footpath_list(char *filename) {
    
	FILE *f = fopen(filename, "r");
	assert(f);
    /* creates empty linked list */
	footpathsll_t *footpaths = NULL;
    footpaths = make_empty_list();

    /* starts reading input file */
	skip_header_line(f);

	footpath_t *fp;
    /* reads input file line by line */
	while ((fp = footpath_read(f))) {

        /* inserts footpath struct at head of linked list */
		footpaths = insert_at_head(footpaths, fp);

	}
	
	fclose(f);

	return footpaths;
}


/* ========================================================================== */
/* Reads the footpath data into a quad tree based on longitudes and latitudes */
qt_node_t *get_footpath_tree(char *filename, char **arguments) {

    rectangle_2D_t *root_rect = NULL;
    root_rect = get_root_rect(root_rect, arguments);

    FILE *f = fopen(filename, "r");
    assert(f);
    /* creates empty quad tree */
    qt_node_t *root = NULL;
    root = create_new_node(root, root_rect);

    /* starts reading input file */
	skip_header_line(f);

    footpath_t *fp1 = NULL, *fp2 = NULL;
    footpathsll_t *fps1 = NULL, *fps2 = NULL;

    while ((fp1 = footpath_read(f))) {
        /* duplicates footpath to help with freeing */
        fp2 = fp_dup(fp1);

        /* creates footpath linked lists */
        fps1 = make_empty_list();
        fps2 = make_empty_list();
        fps1 = insert_at_head(fps1, fp1);
        fps2 = insert_at_head(fps2, fp2);

        /* creates coordinates */
        point_2D_t *start_coord, *end_coord;
        start_coord = get_coord(fp1, START);
        end_coord = get_coord(fp1, END);

        /* inserts footpath pointer into quadtree */
        root = insert_data(root, fps1, start_coord);
        root = insert_data(root, fps2, end_coord);
		
        fps1 = NULL;
        fps2 = NULL;
        fp1 = NULL;
        fp2 = NULL;
    }

    fclose(f);

    return root;

}

/* ========================================================================== */
/* Reads query input to account for blank queries */
int read_query(char *query) {
	
	char ch;
    /* reads first character of query */
	int valid = scanf("%c", &ch);

	if (valid == 1) {
		if (ch == '\n') {
            /* if blank query is present */
			query = "";
			return 1;

		} else {
            /* normal query */
			query[0] = ch;
			scanf("%[^\n]\n", query + 1);
			return 1;

		}
	} else {
		return 0;
	}

	
}

/* ========================================================================== */
