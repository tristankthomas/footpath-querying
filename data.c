#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "data.h"

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

void skip_header_line(FILE *f) {
    while (fgetc(f) != '\n');
}

footpath_t *footpath_read(FILE *f) {

	footpath_t *fp = NULL;
    int footpath_id, reads = 0;
	double delta_z, distance, grade1in, rlmax, rlmin, start_lat, start_lon, 
        end_lat, end_lon, mcc_id, mccid_int, statusid, streetid, street_group;
	char address[MAX_STR_LEN + 1] = "", clue_sa[MAX_STR_LEN + 1] = "", 
        asset_type[MAX_STR_LEN + 1] = "", segside[MAX_STR_LEN + 1] = "";

	reads += fscanf(f, "%d,", &footpath_id);
    reads += read_string(f, address);
    reads += read_string(f, clue_sa);
    reads += read_string(f, asset_type);
    reads += fscanf(f, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,", &delta_z, &distance, 
        &grade1in, &mcc_id, &mccid_int, &rlmax, &rlmin);
    reads += read_string(f, segside);
    reads += fscanf(f, "%lf,%lf,%lf,%lf,%lf,%lf,%lf", &statusid, &streetid, 
        &street_group, &start_lat, &start_lon, &end_lat, &end_lon);

    if (reads == 19) {
        fp = malloc(sizeof(*fp));
        assert(fp);
        fp->footpath_id = footpath_id;
        fp->address = strdup(address);
        fp->clue_sa = strdup(clue_sa); 
        fp->asset_type = strdup(asset_type);
        fp->delta_z = delta_z;
        fp->grade1in = grade1in;
        fp->distance = distance;
        fp->mcc_id = (int) mcc_id;
        fp->mccid_int = (int) mccid_int;
        fp->rlmax = rlmax;
        fp->rlmin = rlmin;
        fp->segside = strdup(segside);
        fp->statusid = (int) statusid;
        fp->streetid = (int) streetid;
        fp->street_group = (int) street_group;
        fp->start_lat = start_lat;
        fp->start_lon = start_lon;
        fp->end_lat = end_lat;
        fp->end_lon = end_lon;
        
        assert(fp->address);
        assert(fp->clue_sa);
        assert(fp->asset_type);
        assert(fp->segside);
    }

	return fp;
}

int get_id(footpath_t *footpath) {
    int id = footpath->footpath_id;
    return id;
}

int read_string(FILE *f, char *str) {
    char ch = 0;
    fscanf(f, "%c", &ch);
    if (ch == '"'){
        fscanf(f, "%[^\"]\",", str);
        return 1;
    } else if (ch == ',') {
        str = "";
        return 1;
    } else {
        str[0] = ch;
        fscanf(f, "%[^,],", str + 1);
        return 1;
    }

    return 0;

}

double get_grade1in(footpath_t *fp) {

    return fp->grade1in;
    
}

int cmp_addr(footpath_t *fp, char *query) {
    char *addr = fp->address;
    return strcmp(query, addr);

}

footpath_t **add_footpath(footpath_t **fps, footpath_t *fp, int num) {
    static int size = INIT_SIZE;
    if (fps == NULL) {
        fps = (footpath_t **) malloc(size * sizeof(*fps));
        assert(fps);
    } else if (num >= size) {
		size *= 2;
		fps = realloc(fps, size * sizeof(*fps));
		assert(fps);
	}
    fps[num - 1] = fp;
    return fps;
}



void footpaths_print(FILE *f, footpath_t **fps, int num_found) {
    for (int i = num_found - 1; i >= 0; i--) {
        footpath_print(f, fps[i]);
    }
}

void footpath_print(FILE *f, footpath_t *fp) {

    fprintf(f, "--> footpath_id: %d || address: %s || clue_sa: %s || "
            "asset_type: %s || deltaz: %.2f || distance: %.2f || "
            "grade1in: %.1f || mcc_id: %d || mccid_int: %d || rlmax: %.2f || "
            "rlmin: %.2f || segside: %s || statusid: %d || streetid: %d || "
            "street_group: %d || start_lat: %.6f || start_lon: %.6f || "
            "end_lat: %.6f || end_lon: %.6f || \n",
            fp->footpath_id, fp->address, fp->clue_sa, fp->asset_type, fp->delta_z, 
            fp->distance, fp->grade1in, fp->mcc_id, fp->mccid_int, fp->rlmax, 
            fp->rlmin, fp->segside, fp->statusid, fp->streetid, fp->street_group, 
            fp->start_lat, fp->start_lon, fp->end_lat, fp->end_lon);

}

void free_footpath(footpath_t *fp) {
    free(fp->address);
    free(fp->clue_sa);
    free(fp->asset_type);
    free(fp->segside);
    free(fp);
}

int cmp_grade(footpath_t *fp1, footpath_t *fp2) {
    if (fp1->grade1in == fp2->grade1in)
        return 0;
    else if (fp1->grade1in > fp2->grade1in)
        return 1;
    else 
        return -1;

}

footpath_t *get_closest(footpath_t *fp1, footpath_t *fp2, double value) {

    if (value - fp1->grade1in >= fp2->grade1in - value)
        return fp2;
    else
        return fp1;

}
