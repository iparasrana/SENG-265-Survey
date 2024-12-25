#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "processing.h"

/* compile time constants - intended to used in proccessing.c only */
#define MONTHS_PER_YEAR 12
#define TWENTY_EIGHT_DAYS 28
#define TWENTY_NINE_DAYS 29
#define THIRTY_DAYS 30
#define THIRTY_ONE_DAYS 31

/* function declarations - intended to used in proccessing.c only */
int calculate_age(char *date_of_birth);
int is_leap_year(int year);
int get_days_by_month(int month, int year) ;

/* compute Likert item relative frequencies */
void compute_frequencies(double **frequencies, Survey *survey) {
    for (int i = 0; i < survey->num_questions; i++) {
        /* compute absolute frequencies first */
        for (int j = 0; j < survey->num_respondents; j++) {
            frequencies[i][survey->respondents[j].responses[i]-1]++;
        }
        /* compute relative percentual frequencies */
        for (int k = 0; k < survey->num_likert_levels; k++) {
            if (survey->num_respondents > 0) {
                frequencies[i][k] = 100.0 * frequencies[i][k] / ((double)survey->num_respondents);
            }
            else {
                frequencies[i][k] = 0;
            }
        }
    }
    return;
}

/* compute scores for each respondent */
void compute_scores(double **scores, Survey *survey) {
    /* compute scores by questions category for each respondent */
    for (int i = 0; i < survey->num_respondents; i++) {
        scores[i][0] = compute_c(survey->respondents[i].responses, survey->question_types);
        scores[i][1] = compute_i(survey->respondents[i].responses, survey->question_types);
        scores[i][2] = compute_g(survey->respondents[i].responses, survey->question_types);
        scores[i][3] = compute_u(survey->respondents[i].responses, survey->question_types);
        scores[i][4] = compute_p(survey->respondents[i].responses, survey->question_types);
    }
    return;
}

/* compute Confidence score */
double compute_c(short response[], short question_types[]) {
    int sum = 0;
    for (int i=0; i<=7; i++) {
        if (question_types[i]) {
            sum += response[i];
        }
        else {
            sum +=  7 - response[i];
        }
    }
    return sum / 8.0;
}

/* compute Interest score */
double compute_i(short response[], short question_types[]) {
    int sum = 0;
    for (int i=8; i<=17; i++) {
        if (question_types[i]) {
            sum += response[i];
        }
        else {
            sum +=  7 - response[i];
        }
    }
    return sum / 10.0;
}

/* compute Gender score */
double compute_g(short response[], short question_types[]) {
    int sum = 0;
    for (int i=18; i<=27; i++) {
        if (question_types[i]) {
            sum += response[i];
        }
        else {
            sum +=  7 - response[i];
        }
    }
    return sum / 10.0;
}

/* compute Usefulness score */
double compute_u(short response[], short question_types[]) {
    int sum = 0;
    for (int i=28; i<=33; i++) {
        if (question_types[i]) {
            sum += response[i];
        }
        else {
            sum +=  7 - response[i];
        }
    }
    return sum / 6.0;
}

/* compute Professional score */
double compute_p(short response[], short question_types[]) {
    int sum = 0;
    for (int i=34; i<=37; i++) {
        if (question_types[i]) {
            sum += response[i];
        }
        else {
            sum +=  7 - response[i];
        }
    }
    return sum / 4.0;
}


/* Compute average scores per respondent */
void compute_average_scores(double average_scores[], double **scores, int num_respondents) {
    for (int i = 0; i < NUMBER_SCORES; i++) {
        average_scores[i] = 0.0;
        for (int j = 0; j < num_respondents; j++) {
            average_scores[i] += scores[j][i];
        }
        if (num_respondents > 0) {
            average_scores[i] /= ((double)num_respondents);
        }
    }
    return;
}

/* filter respondents by field of study */
void filter_by_field_of_study(Survey *survey, char *field_of_study) {
    int j = 0;
    
    // for each respondent, compare field of study with given value of field of study
    for (int i = 0; i < survey->num_respondents; i++){
        if (!strcmp(survey->respondents[i].field_of_study, field_of_study)){
            survey->respondents[j] = survey->respondents[i];
            j++;
        }
    }

    // adjust number of respondents and respondents array size after filtering 
    survey->num_respondents= j;
    survey->respondents = realloc(survey->respondents, survey->num_respondents*sizeof(Respondent));
}

/* filter respondents by born in canada */
void filter_by_born_in_canada(Survey *survey, char *born_in_canada) {
    int j = 0;

    // for each respondent, compare born in canada with given value of born in canada
    for (int i = 0; i < survey->num_respondents; i++){
        if (!strcmp(survey->respondents[i].born_in_canada, born_in_canada)){
            survey->respondents[j] = survey->respondents[i];
            j++;
        }
    }

    // adjust number of respondents and respondents array size after filtering
    survey->num_respondents= j;
    survey->respondents = realloc(survey->respondents, survey->num_respondents*sizeof(Respondent));
}

/* filter respondents by age range */
void filter_by_age_range(Survey *survey, int min_age, int max_age) {
    int j = 0;

    // for each respondent, chack if age is within given range
    for (int i = 0; i < survey->num_respondents; i++){
        int age = calculate_age(survey->respondents[i].date_of_birth);
        if (age >= min_age && age <= max_age){
            survey->respondents[j] = survey->respondents[i];
            j++;
        }
    }

    // adjust number of respondents and respondents array size after filtering
    survey->num_respondents= j;
    survey->respondents = realloc(survey->respondents, survey->num_respondents*sizeof(Respondent));
}

/* calculate respondent age */
int calculate_age(char *date_of_birth) {
    // Hardcoded assignement due date to calculate current age
    int current_day = 15;
    int current_month = 10;
    int current_year = 2024;

    // DOB format is "YYYY-MM-DD"
    int dob_year = atoi(date_of_birth);
    int dob_month = atoi(date_of_birth + 5);
    int dob_day = atoi(date_of_birth + 8);

    // calculate age in years - rounding down
    if(current_day < dob_day) {
        current_day += get_days_by_month(dob_month, dob_year);
        current_month--;
    }
    if(current_month < dob_month) {
        current_month += MONTHS_PER_YEAR;
        current_year--;
    }

    return current_year - dob_year;
}

/* get number of days by given month */
int get_days_by_month(int month, int year) {
    int days = 0;
    switch(month) {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            days = THIRTY_ONE_DAYS;
            break;

        case 4:
        case 6:
        case 9:
        case 11: 
            days = THIRTY_DAYS;
            break;
        
        case 2:
            days = is_leap_year(year) ? TWENTY_NINE_DAYS : TWENTY_EIGHT_DAYS;
            break;
    }

    return days;
}

/* check if given year is leap year or not */
int is_leap_year(int year) {
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}
