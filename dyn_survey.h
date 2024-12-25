#ifndef _DYN_SURVEY_H_
#define _DYN_SURVEY_H_

/*
 * Compile-time constants
 */
#define NUMBER_TESTS 6
#define NUMBER_QUESTIONS 38
#define NUMBER_LIKERT_LEVELS 6
#define NUMBER_SCORES 5
#define MAX_NUMBER_PHRASES 50
#define MAX_PHRASE_LENGTH 120
#define MAX_NUMBER_RESPONSES 50
#define MAX_WORD_LENGTH 20
#define MAX_QUESTION_LENGTH 120
#define MAX_LINE_LENGTH 3000
#define DOB_LENGTH 11
#define BORN_IN_CANADA_LENGTH 4

/*
 * Storing respodent data including responses
 */
typedef struct respondent {
    char field_of_study[MAX_WORD_LENGTH]; // field of study
    char born_in_canada[BORN_IN_CANADA_LENGTH]; // born in canada
    char date_of_birth[DOB_LENGTH]; // date of birth of respondent
    short *responses; // pointer to list of responses provided by respondent
} Respondent;

/*
 * To store survey information 
 */
typedef struct survey {
    int num_questions;
    int num_respondents;
    int num_likert_levels;
    char questions[NUMBER_QUESTIONS][MAX_QUESTION_LENGTH];
    short *question_types;
    char likert_level_descriptions[NUMBER_LIKERT_LEVELS][MAX_WORD_LENGTH];
    Respondent *respondents; // pointer to list of respondents
} Survey;

#endif
