/* ------------------------ 
Created by Tristan Thomas (tkthomas@student.unimelb.edu.au)
10/08/2022
*
* data.c: Includes the functions and declarations for individual footpath structs.
* 
 ------------------------ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "data.h"

// maybe change long and lat to long doubles
/* Single footpath structure */
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


/* ========================= Reading data functions ========================= */
/* Skips the first line of the input data file.
 * This function was adapted from the qStud program in workshops W1-W4 */
void skip_header_line(FILE *f) {
    while (fgetc(f) != '\n');
}


/* ========================================================================== */
/* Reads individual footpath data. */
footpath_t *footpath_read(FILE *f) {

	footpath_t *fp = NULL;
    int footpath_id, reads = 0;
	double delta_z, distance, grade1in, rlmax, rlmin, mcc_id, mccid_int, 
        statusid, streetid, street_group, start_lat, start_lon, end_lat, end_lon;
	char address[MAX_STR_LEN + 1] = "", clue_sa[MAX_STR_LEN + 1] = "", 
        asset_type[MAX_STR_LEN + 1] = "", segside[MAX_STR_LEN + 1] = "";

    /* reading data into variables (seperated to accomodate strings) */
	reads += fscanf(f, "%d,", &footpath_id);
    reads += read_string(f, address);
    reads += read_string(f, clue_sa);
    reads += read_string(f, asset_type);
    reads += fscanf(f, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,", &delta_z, &distance, 
        &grade1in, &mcc_id, &mccid_int, &rlmax, &rlmin);
    reads += read_string(f, segside);
    reads += fscanf(f, "%lf,%lf,%lf,%lf,%lf,%lf,%lf", &statusid, &streetid, 
        &street_group, &start_lat, &start_lon, &end_lat, &end_lon);

    /* if read is successful assign variables to struct */
    if (reads == 19) {

        fp = malloc(sizeof(*fp));
        assert(fp);
        fp->footpath_id = footpath_id;
        fp->address = strdup(address);
        assert(fp->address);
        fp->clue_sa = strdup(clue_sa); 
        assert(fp->clue_sa);
        fp->asset_type = strdup(asset_type);
        assert(fp->asset_type);
        fp->delta_z = delta_z;
        fp->grade1in = grade1in;
        fp->distance = distance;
        fp->mcc_id = (int) mcc_id;
        fp->mccid_int = (int) mccid_int;
        fp->rlmax = rlmax;
        fp->rlmin = rlmin;
        fp->segside = strdup(segside);
        assert(fp->segside);
        fp->statusid = (int) statusid;
        fp->streetid = (int) streetid;
        fp->street_group = (int) street_group;
        fp->start_lat = start_lat;
        fp->start_lon = start_lon;
        fp->end_lat = end_lat;
        fp->end_lon = end_lon;
        
        
    }
    //fp->is_freed = 0;

	return fp;
}


/* ========================================================================== */
/* Reads the string attributes of the footpath */
int read_string(FILE *f, char *str) {

    char ch = 0;
    fscanf(f, "%c", &ch);

    if (ch == '"'){
        /* checks for quotes surrounding string (i.e comma within string) */
        fscanf(f, "%[^\"]\",", str);
        return 1;

    } else if (ch == ',') {
        /* checks for empty string */
        str = "";
        return 1;

    } else {
        /* scans string as normal */
        str[0] = ch;
        fscanf(f, "%[^,],", str + 1);
        return 1;
    }

    return 0;

}



/* ========================================================================== */
/* Adds a footpath to the dynamic array */
footpath_t **add_footpath(footpath_t **fps, footpath_t *fp, int num) {

    /* static variable to keep track of footpath size (not used anywhere else) */
    static int size = INIT_SIZE;

    if (fps == NULL) {
        /* allocates initial array memory */
        fps = (footpath_t **) malloc(size * sizeof(*fps));
        assert(fps);

    } else if (num >= size) {
        /* expands array */
		size *= 2;
		fps = realloc(fps, size * sizeof(*fps));
		assert(fps);
	}

    fps[num - 1] = fp;
    return fps;
}


/* ========================================================================== */
/* Frees the allocated footpath memory including strings */
void free_footpath(footpath_t *fp) {

    free(fp->address);
    free(fp->clue_sa);
    free(fp->asset_type);
    free(fp->segside);
    //fp->is_freed = 1;
    free(fp);

}

/* ========================= Printing data functions ======================== */
/* Prints an array of footpaths */
void footpaths_print(FILE *f, footpath_t **fps, int num_found) {

    for (int i = num_found - 1; i >= 0; i--) {
        footpath_print(f, fps[i]);
    }

}



/* ========================================================================== */
/* Prints a single footpath */
void footpath_print(FILE *f, footpath_t *fp) {

    fprintf(f, "--> footpath_id: %d || address: %s || clue_sa: %s || "
            "asset_type: %s || deltaz: %.2f || distance: %.2f || "
            "grade1in: %.1f || mcc_id: %d || mccid_int: %d || rlmax: %.2f || "
            "rlmin: %.2f || segside: %s || statusid: %d || streetid: %d || "
            "street_group: %d || start_lat: %.6lf || start_lon: %.6lf || "
            "end_lat: %.6lf || end_lon: %.6lf || \n",
            fp->footpath_id, fp->address, fp->clue_sa, fp->asset_type, fp->delta_z, 
            fp->distance, fp->grade1in, fp->mcc_id, fp->mccid_int, fp->rlmax, 
            fp->rlmin, fp->segside, fp->statusid, fp->streetid, fp->street_group, 
            fp->start_lat, fp->start_lon, fp->end_lat, fp->end_lon);

}




/* ==================== Comparison and getting functions ==================== */
/* Compares grade1in of two footpaths */


// GENERALISE THIS FUNCTION
int cmp_grade(footpath_t *fp1, footpath_t *fp2) {

    if (fp1->grade1in == fp2->grade1in)
        return 0;
    else if (fp1->grade1in > fp2->grade1in)
        return 1;
    else 
        return -1;

}

int cmp_id(footpath_t *fp1, footpath_t *fp2) {

    if (fp1->footpath_id == fp2->footpath_id)
        return 0;
    else if (fp1->footpath_id > fp2->footpath_id)
        return 1;
    else 
        return -1;

}

/* ========================================================================== */
/* Returns the closest footpath based on grade1in value
 * This function was taken from the webpage:
 *      https://www.geeksforgeeks.org/find-closest-number-array/
 * and adapted to work the grade1in attribute */
footpath_t *get_closest(footpath_t *fp1, footpath_t *fp2, double value) {

    if (value - fp1->grade1in >= fp2->grade1in - value)
        return fp2;
    else
        return fp1;

}

/* ========================================================================== */
/* Returns the 'grade1in' footpath variable */
double get_grade1in(footpath_t *fp) {

    return fp->grade1in;
    
}

int get_id(footpath_t *fp) {
    return fp->footpath_id;
}

/* ========================================================================== */
/* Compares the query to the footpath address */
int cmp_addr(footpath_t *fp, char *query) {

    return strcmp(query, fp->address);

}

// int get_is_freed(footpath_t *fp) {
//     return fp->is_freed;
// }

/* ========================================================================== */


footpath_t *fp_dup(footpath_t *fp) {
    footpath_t *copy = NULL;
    copy = (footpath_t *) malloc(sizeof(*fp));
    assert(copy);
    copy->footpath_id = fp->footpath_id;
    copy->address = strdup(fp->address);
    assert(copy->address);
    copy->clue_sa = strdup(fp->clue_sa); 
    assert(copy->clue_sa);
    copy->asset_type = strdup(fp->asset_type);
    assert(copy->asset_type);
    copy->delta_z = fp->delta_z;
    copy->grade1in = fp->grade1in;
    copy->distance = fp->distance;
    copy->mcc_id = fp->mcc_id;
    copy->mccid_int = fp->mccid_int;
    copy->rlmax = fp->rlmax;
    copy->rlmin = fp->rlmin;
    copy->segside = strdup(fp->segside);
    assert(copy->segside);
    copy->statusid = fp->statusid;
    copy->streetid = fp->streetid;
    copy->street_group = fp->street_group;
    copy->start_lat = fp->start_lat;
    copy->start_lon = fp->start_lon;
    copy->end_lat = fp->end_lat;
    copy->end_lon = fp->end_lon;

    return copy;
}

