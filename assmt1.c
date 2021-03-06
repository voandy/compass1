/* Skeleton code for COMP10002 Assignment 1, March 2017.
   Jianzhong Qi, jianzhong.qi@unimelb.edu.au

   Programmer: Andy Vo, voa1@student.unimelb.edu.au

   Algorithms are fun!
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DIV "==========" /* stage header */
/* stage numbers */
#define STAGE_NUM_ONE 1 
#define STAGE_NUM_TWO 2
#define STAGE_NUM_THREE 3
#define STAGE_NUM_FOUR 4
#define STAGE_NUM_FIVE 5

#define READ_OK 1 /* reading point succeeded */
#define READ_FAILED 0 /* reading point failed */

#define MAX_NUM_POINTS 99 /* maximum number of points */
#define MAX_NUM_DIMENSIONS 10 /* maximum number of dimensions */

#define DIVIDER 100 /* Number to divide sums by in stage 2 */
#define PLUS_MARKER 10 /* What increment to place + in stage 2 */

typedef double point_t[MAX_NUM_DIMENSIONS];

/* used to track skyline points for stage 5 */
typedef struct skyline_point_t{
	int ref_no;
	double coords[MAX_NUM_DIMENSIONS];
	int points_dominated;
} skyline_point_t;

/* function prototypes */
void stage_one(point_t one_point, int *num_points, int num_dimensions);
void stage_two(point_t points[], int *num_points, int num_dimensions, 
	double *coordinate_sums);
void stage_three(double *coordinate_sums, int num_points);
void stage_four(point_t points[], int num_points, int num_dimensions);
void stage_five(point_t points[], int num_points, int num_dimensions);

void print_stage_heading(int stage);
int read_one_point(point_t one_point, int num_dimensions);

/* add your own function prototypes here */
void print_point(point_t one_point, int num_dimensions, int point_ref);
void point_cpy(point_t point_a, point_t point_b, int num_dimensions);
int is_dominated(point_t point_a, point_t point_b, int num_dimensions);
int is_skyline_point(point_t point_a, point_t points[], int num_points, 
	int num_dimensions, int skip);
int is_dominant(point_t point_a, point_t point_b, int num_dimensions);
int no_of_dominations(point_t point_a, point_t points[], int num_points, 
	int num_dimensions, int skip);
int skyline_compare(const void *v1, const void *v2);

/* main program binds it all together */
int
main(int argc, char *argv[]) {

	/* storage for the input data */
	int num_dimensions;
	point_t points[MAX_NUM_POINTS];
	
	/* storage for statistics */
	int num_points = 0; /* number of points read in */
	double coordinate_sums[MAX_NUM_POINTS]; /* coordinate sums of the points */
	
	/* read the first line - number of dimensions */
	scanf("%d", &num_dimensions);
	
	/* stage 1 */
	stage_one(points[0], &num_points, num_dimensions);

	/* stage 2 */
	stage_two(points, &num_points, num_dimensions, 
		coordinate_sums);
	
	/* stage 3 */
	stage_three(coordinate_sums, num_points);

	/* stage 4 */
	stage_four(points, num_points, num_dimensions);

	/* stage 5 */
	stage_five(points, num_points, num_dimensions);
	
	/* and done! */
	return 0;
}

/* print out a stage heading, with a prior blank line once the stage number
   goes plus one 
*/
void 
print_stage_heading(int stage) {
	/* main job is to print the stage number passed as an argument */
	if (stage > STAGE_NUM_ONE) {
		/* separate the stages */
		printf("\n");
	}
	printf("Stage %d\n%s\n", stage, DIV);
}

/* read the coordinates of one point, store them in one_point
*/
int 
read_one_point(point_t one_point, int num_dimensions) {
	int i = 0;
	
	/* keep reading until num_dimensions numbers have been obtained */
	while (i < num_dimensions) {
		if (scanf("%lf", one_point+i) != 1) {
			return READ_FAILED;
		}
		i++;
	}
	
	return READ_OK;
}

/* stage 1 - read in and print one point
*/
void 
stage_one(point_t one_point, int *num_points, int num_dimensions) {
	/* print stage heading for stage 1 */
	print_stage_heading(STAGE_NUM_ONE);
	
	/* add your code here for stage 1 */
	/* reads one point and prints the coordinates */
	read_one_point(one_point, num_dimensions);
	print_point(one_point, num_dimensions, 0);
}

/* prints a single point */
void
print_point(point_t one_point, int num_dimensions, int point_ref) {
	printf("Point %02d: <", point_ref + 1);
	int i;
	for (i = 0; i < num_dimensions; i++) {
		printf("%.2f", one_point[i]);
		if (i < num_dimensions - 1) {
			printf(", ");
		}
	}
	printf(">\n");
}

/* stage 2 - read in the rest of the points and visualise them
*/
void 
stage_two(point_t points[], int *num_points, int num_dimensions, 
	double *coordinate_sums) {
	/* print stage heading for stage 2 */
	print_stage_heading(STAGE_NUM_TWO);

	point_t one_point;
	/* add your code here for stage 2 */
	int i, j;

	/* populates points[] with data from the input, i starts at 1 as we have
	already read one point in stage one */
	i = 1;
	while (read_one_point(one_point, num_dimensions)) {
		int j = 0;
		for (j = 0; j < num_dimensions; j++) {
			points[i][j] = one_point[j];
		}
		i++;
	}

	*num_points = i;
	/* adds up sum of coords for each point and adds them to 
	coordinate_sums[] */
	for (i = 0; i < *num_points; i ++) {
		double total = 0;
		for (j = 0; j < num_dimensions; j++) {
			total += points[i][j];
		}
		coordinate_sums[i] = total;
	}

	/* prints and graphs each total from coordinate sums */
	for (i = 0; i < *num_points; i ++) {
		printf("Point %02d, sum of coordinates (/%d): %5.2f |", i + 1, DIVIDER,
			coordinate_sums[i] / DIVIDER);
		for (j = 0; j < coordinate_sums[i] / DIVIDER; j++) {
			if (((j + 1) % PLUS_MARKER) == 0) {
				printf("+");
			} 
			else {
				printf("-");
			}
		}
		printf("\n");

	}
}

/* stage 3 - print statistics
*/
void 
stage_three(double *coordinate_sums, int num_points) {
	/* print stage heading for stage 3 */
	print_stage_heading(STAGE_NUM_THREE);
	
	/* add your code here for stage 3 */
	printf("Total: %d points\n",num_points );

	/* finds the point with the largest sum of coordinates. Uses < rather
	than <= so that first such point is returned */
	int point_lgst_sum;
	int i;
	float lgst_sum = 0.0;

	for (i = 0; i < num_points; i ++) {
		if (coordinate_sums[i] > lgst_sum) {
			point_lgst_sum = i + 1;
			lgst_sum = coordinate_sums[i];
		}
	}

	printf("Point of largest sum of coordinates: %02d\n", point_lgst_sum);
	printf("Largest sum of coordinates: %.2f\n", lgst_sum);
}

/* stage 4 - find and print the skyline points
*/
void 
stage_four(point_t points[], int num_points, int num_dimensions) {
	/* print stage heading for stage 4 */
	print_stage_heading(STAGE_NUM_FOUR);
	
	/* add your code here for stage 4 */
	skyline_point_t skyline_points[num_points]; /* array of struct used to 
	store skyline points */
	int skyline_points_count = 0;
	point_t point_a; /* stores a point to compare */
	int skip = 0; /* skips a point, see is_skyline_point() */
	int i;

	for (i = 0; i < num_points; i++) {
		/* copies a point from points to point_a*/
		point_cpy(point_a, points[i], num_dimensions);

		/* if point_a is a skyline point, copies it to skyline_points[] */
		skip = i;
		if (is_skyline_point(point_a, points, num_points, 
			num_dimensions, skip)) {
			
			point_cpy(skyline_points[skyline_points_count].coords, point_a, 
				num_dimensions);
			
			skyline_points[skyline_points_count].ref_no = i;
			skyline_points_count++;
		}

	}

	/* prints out the points we found */
	printf("Skyline points:\n");
	for (i = 0; i < skyline_points_count; i++) {
		print_point(skyline_points[i].coords, num_dimensions, 
			skyline_points[i].ref_no);
	}	
}

/* given two points, will copy the coords from point_b to point_a */
void 
point_cpy(point_t point_a, point_t point_b, int num_dimensions) {
	int i;
	for (i = 0; i < num_dimensions; i++) {
		point_a[i] = point_b[i];
	}
}

/* given two points returns 1 if point_a is dominated by b and 0 otherwise */
int 
is_dominated(point_t point_a, point_t point_b, int num_dimensions) {
	int i;
	for (i = 0; i < num_dimensions; i++) {
		if (point_b[i] < point_a[i]) {
			return 0;
		}
	}
	return 1;
}

/* given a point_a and an array of points will return 1 if point_a is a 
skyline point and 0 otherwise */
int
is_skyline_point(point_t point_a, point_t points[], int num_points, 
	int num_dimensions, int skip) {
	int i;
	for (i = 0; i < num_points; i++) {
		/* skips a point to prevent comparing a point to itself */
		if (i == skip) {
			i++;
		}
		/* if point_a happens to be the last point we break */
		if (i == num_points) {
			break;
		}
		if (is_dominated(point_a, points[i], num_dimensions)) {
			return 0;
		}
	}
	return 1;
}


/* stage 5 - sort and print the skyline points by the number of points 
they dominate */
void 
stage_five(point_t points[], int num_points, int num_dimensions) {
	/* print stage heading for stage 5 */
	print_stage_heading(STAGE_NUM_FIVE);
	
	/* add your code here for stage 5 */
	skyline_point_t skyline_points[num_points]; /*stores skyline points */
	point_t point_a; /* stores a point to compare */
	int skyline_points_count = 0;
	int skip = 0;
	int i;

	/* replicated from stage 4 */
	for (i = 0; i < num_points; i++) {
		/* copies a point from points to point_a*/
		point_cpy(point_a, points[i], num_dimensions);

		/* if point_a is a skyline point, copies it to skyline_points */
		skip = i;
		if (is_skyline_point(point_a, points, num_points, 
			num_dimensions, skip)) {
			
			point_cpy(skyline_points[skyline_points_count].coords, point_a, 
				num_dimensions);
			
			skyline_points[skyline_points_count].ref_no = i;
			skyline_points_count++;
		}

	}

	/* add the number of dominations to skyline_points */
	for (i = 0; i < skyline_points_count; i++) {
		skyline_points[i].points_dominated = no_of_dominations(
			skyline_points[i].coords, points, num_points, num_dimensions, 
			skyline_points[i].ref_no);
	}

	/* sorts the skyline points by points_dominated and ref_no, see 
	skyline_compare() */
	qsort(skyline_points, skyline_points_count, sizeof(skyline_point_t), 
		skyline_compare);

	printf("Sorted skyline points:\n");
	for (i = 0; i < skyline_points_count; i++) {
		print_point(skyline_points[i].coords, num_dimensions, 
			skyline_points[i].ref_no);
		printf("Number of points dominated: %d\n", 
			skyline_points[i].points_dominated);
	}
}

/* returns 1 if point_a dominates point_b and 0 otherwise
subtly different from is_dominated, a point that isn't dominated by another 
point doesn't necessarily dominate that point in return */
int 
is_dominant(point_t point_a, point_t point_b, int num_dimensions) {
	int i;
	for (i = 0; i < num_dimensions; i++) {
		if (point_a[i] < point_b[i]) {
			return 0;
		}
	}
	return 1;
}

/* given a point_a and an array of points will return the number of points
point_a dominates */
int 
no_of_dominations(point_t point_a, point_t points[], int num_points, 
	int num_dimensions, int skip) {
	int i;
	int points_dominated = 0;

	i = 0;

	for (i = 0; i < num_points; i++) {
		/* skips a point to prevent comparing a point to itself */
		if (i == skip) {
			i++;
		}
		/* if point_a happens to be the last point we break */
		if (i == num_points) {
			break;
		}
		if (is_dominant(point_a, points[i], num_dimensions)) {
			points_dominated++;
		}
	}
	return points_dominated;
}

/* code copied from stackoverflow
http://stackoverflow.com/questions/8721189/how-to-sort-an-array-of-structs-in-c
author: Jonathan Leffler

Comparison function used for sorting in stage_five. */
int skyline_compare(const void *v1, const void *v2)
{
    const skyline_point_t *p1 = (skyline_point_t *) v1;
    const skyline_point_t *p2 = (skyline_point_t *) v2;
    
    if (p1->points_dominated > p2->points_dominated) {
        return -1;
    }
    else if (p1->points_dominated < p2->points_dominated) {
        return +1;
    }
    else if (p1->ref_no < p2->ref_no) {
        return -1;
    }
    else if (p1->ref_no > p2->ref_no) {
        return +1;
    }
    else {
        return 0;
    }
}
