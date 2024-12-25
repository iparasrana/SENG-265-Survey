#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include "dyn_survey.h"

/* output functions */
void print_output(short tests[], int num_tests, Survey *survey,
    double **frequencies, double **scores,
    double average_scores[]);

#endif
