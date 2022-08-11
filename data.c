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
    double start_long;
    double end_lat;
    double end_long;
};

void skip_header_line(FILE *f) {
    while (fgetc(f) != '\n');
}

footpath_t *footpath_read(FILE *f) {

	footpath_t *fp = NULL;
	int footpath_id, mcc_id, mccid_int, statusid, streetid, street_group;
	double delta_z, distance, grade1in, rlmax, rlmin, start_lat, start_long, end_lat, end_long;
	char address[MAX_STR_LEN + 1], clue_sa[MAX_STR_LEN + 1], asset_type[MAX_STR_LEN + 1], segside[MAX_STR_LEN + 1];

	if (fscanf(f, "%d,%[^,],%[^,],%[^,],%lf,%lf,%lf,%d,%d,%lf,%lf,%[^,],%d,%d,%d,%lf,%lf,%lf,%lf", 
            &footpath_id, address, clue_sa, asset_type, &delta_z, &distance, &grade1in, 
            &mcc_id, &mccid_int, &rlmax, &rlmin, segside, &statusid, &streetid, 
            &street_group, &start_lat, &start_long, &end_lat, &end_long) == 19) {

		fp = malloc(sizeof(*fp));     // allocates memory for s
		assert(fp);
		fp->footpath_id = footpath_id;
		fp->address = strdup(address);     // duplicates strings name top s->name
        fp->clue_sa = strdup(clue_sa); 
        fp->asset_type = strdup(asset_type);
        fp->grade1in = grade1in;
        fp->distance = distance;
        fp->mcc_id = mcc_id;
        fp->rlmax = rlmax;
        fp->rlmin = rlmin;
        fp->segside = strdup(segside);
        fp->statusid = statusid;
        fp->streetid = streetid;
        fp->street_group = street_group;
        fp->start_lat = start_lat;
        fp->start_long = start_long;
        fp->end_lat = end_lat;
        fp->end_long = end_long;
        
		assert(fp->address);
        assert(fp->clue_sa);
        assert(fp->asset_type);
        assert(fp->segside);

	}
	return fp;
}

int get_id(footpath_t *footpath) {
    return footpath->footpath_id;
}

