# Footpath Querying System for the City of Melbourne
This project was created as part of an assignment for an algorithms subject and is split into four stages. Each stage implements a different querying mechanism for looking up footpaths in Melbourne. Various algorithms (for querying) and data structures (for storing) were created. The data structures used include linked lists, dynamic arrays and quadtrees. All code was written by Tristan Thomas with necessary credits commented throughout code. 

### Stage 1
The first stage stores the footpaths in a linkedlist which can simply be queried using the footpath `address` as the key. This outputs the footpaths found (this is same for all four stages) to the output file and the number of footpaths found to stdout.

### Stage 2
The second stage again stores the footpaths in a sorted array but now queries based on the `grade1in` attribute and returns the closest record using binary search. Stdout will now output the closest `grade1in` value.

### Stage 3
For stage three the footpaths are now stored in a quadtree based on longitude and latitude pairs and queried by a single point. An initial region must be inputting as a reference for storing the footpaths, given by the command line args. Stdout outputs the quadrants traversed in order (SW, NW, NE, SE).

### Stage 4
For stage four the footpaths are stored in the same manner as stage 3, but are now queried by a region instead and finds all footpaths within this region.

## **Dataset**
The footpath dataset can be found [here](https://data.melbourne.vic.gov.au/) and it was processed to simplify the geometric data.
The dataset has the following fields:
```
footpath_id  - The row number for this footpath in the original full dataset. (integer)
address      - A name describing the location of the footpath. (string)
clue_sa      - The CLUE small area the footpath is in. (string)
asset_type   - The name of the type of footpath. (string)
deltaz       - The change in vertical distance along the footpath. (double)
distance     - The length of the footpath (full geometry) in metres. (double)
grade1in     - The percentage gradient of the footpath (full geometry). (double)
mcc_id       - The id number identifying the footpath. (integer)
mccid_int    - A second number identifying the road or intersection the footpath borders. (integer)
rlmax        - The highest elevation on the footpath. (double)
rlmin        - The lowest elevation on the footpath. (double)
segside      - The side of the road which the footpath is on. (string)
statusid     - The status of the footpath. (integer)
streetid     - The ID of the first street in the Address. (integer)
street_group - The footpath_id of one of the footpaths connected to this footpath without a gap. (integer)
start_lat    - The latitude (y) of the starting point representing the line approximation of the footpath. (double)
start_lon    - The longitude (x) of the starting point representing the line approximation of the footpath. (double)
end_lat      - The latitude (y) of the ending point representing the line approximation of the footpath. (double)
end_lon      - The longitude (x) of the starting point representing the line approximation of the footpath. (double)
```

## **Using the program**
The provided Makefile can be used to generate each of the four dictionary executables.

The first two dictionaries can be executed by:
```
./<dict-name> <dict-num> <data-file> <output-file>
```
With stdin providing the query e.g.
```
./dict1 1 dataset_1000.csv output.txt < test1.1.in
```

\
The second two dictionaries can be executed by:
```
./<dict-name> <dict-num> <data-file> <output-file> <start-longitude> <start-latitude> <end-longitude> <end-latitude>
```
Again with stdin providing the query e.g.
```
./dict3 3 dataset_2.csv output.txt 144.969 -37.7975 144.971 -37.7955 < test9.s4.in
```

