#ifndef _PROCESSING_H_
#define _PROCESSING_H_

#include "dyn_survey.h"

/* processing functions */
void compute_frequencies(double **frequencies, Survey *survey);
void compute_scores(double **scores, Survey *survey);
void compute_average_scores(double average_scores[], 
    double **scores, int num_respondents);
double compute_c(short response[], short question_types[]);
double compute_i(short response[], short question_types[]);
double compute_g(short response[], short question_types[]);
double compute_u(short response[], short question_types[]);
double compute_p(short response[], short question_types[]);
void filter_by_field_of_study(Survey *survey, char *field_of_study);
void filter_by_born_in_canada(Survey *survey, char *born_in_canada);
void filter_by_age_range(Survey *survey, int min_age, int max_age);

#endif
