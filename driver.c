/* Created by Tristan Thomas (tkthomas@student.unimelb.edu.au)
10/08/2022
*
* 
*
* To run the program type: 
* ./coins --num_coins 5 --shape_coins trapezoid --output blabla.txt
* 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "data.h"
#include "linked_list.h"
#include "array.h"

#define DICT1 1
#define DICT2 2

int process_args(int argc, char *argv[]);
footpaths_t *get_footpath_list(char *filename);
void list_querying(char *data_file_name, FILE *input, FILE *output, FILE *out_file, int dict_type);

int main(int argc, char *argv[]) {

    int dict_type = process_args(argc, argv);
    
    FILE *output_file = fopen(argv[3], "w");
    assert(output_file);

    switch (dict_type) {
		case DICT1:
        list_querying(argv[2], stdin, stdout, output_file, dict_type);
		break;
		case DICT2:
		default:
			fprintf(stderr, "Dictionary type %d not yet implemented\n", dict_type);
			exit(EXIT_FAILURE);
	}

    fclose(output_file);

    return 0;
}


int process_args(int argc, char *argv[]) {
	if (argc < 3 || (atoi(argv[1]) !=  DICT1 && atoi(argv[1]) != DICT2)) {
		fprintf(stderr, "Usage: %s dictionary_type input_file_name output_file_name, where:\n", argv[0]);
		fprintf(stderr, "       \t - dictionary_type is \"1\" for linear search lookup and \"2\" for binary search lookup\n");
		fprintf(stderr, "       \t - input_file_name: data file for building dictionary\n");
		fprintf(stderr, "       \t - output_file_name: a file for outputting statistics for searches\n");
		exit(EXIT_FAILURE);
	}
	return atoi(argv[1]);
}

footpaths_t *get_footpath_list(char *filename) {
    
	FILE *f = fopen(filename, "r");
	assert(f);

	footpaths_t *footpaths = make_empty_list();
	skip_header_line(f);

	footpath_t *fp;
	while ((fp = footpath_read(f))) {
		footpaths = insert_at_head(footpaths, fp);

	}
	
	fclose(f);

	return footpaths;
}


void list_querying(char *data_file_name, FILE *input, FILE *output, FILE *out_file, int dict_type) {
    assert(dict_type == DICT1 || dict_type == DICT2);

    footpaths_t *footpaths = get_footpath_list(data_file_name);
    int id = get_head_id(footpaths);
    printf("%d\n", id);

}
