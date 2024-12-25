#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "processing.h"
#include "input_handling.h"

/* tokenize a line and store it in a string array of phrases */
int tokenize_line(char *line, char phrases[][MAX_PHRASE_LENGTH], char delimiter[]) {

    int num_phrases = 0;
    char *token;
    
    /* get the first token from line */
    token = strtok(line, delimiter);
    
    /* for every token in line, store it */
    while (token) {
        strncpy(phrases[num_phrases], token, MAX_PHRASE_LENGTH);
        num_phrases++;

        /* get the next token from line or reach end of line */
        token = strtok(NULL, delimiter);
    } 
    return num_phrases;          
}

/* store test configuration to only run specified tests */
int process_tests(char phrases[][MAX_PHRASE_LENGTH], int num_phrases, short tests[]) {
    /* for every phrase in phrases, convert it to a bit and store it  */
    for (int i = 0; i < num_phrases; i++) {
        if (phrases[i][0] == '1') {
            tests[i] = 1;
        }
        else {
            tests[i] = 0;
        }
    }
    return num_phrases;          
}

/* store survey questions */
int process_questions(char phrases[][MAX_PHRASE_LENGTH], int num_phrases, 
    char questions[][MAX_QUESTION_LENGTH]) {

    /* for every phrase in phrases, store it as a question  */
    for (int i = 0; i < num_phrases; i++) {
        strncpy(questions[i], phrases[i], MAX_QUESTION_LENGTH);
    }
    return num_phrases;          
}

/* store survey question types (direct or reverse) */
void process_question_types(char phrases[][MAX_PHRASE_LENGTH], int num_phrases, 
    short question_types[]) {

    /* for every phrase in phrases, convert it to a bit and store it  */
    for (int i = 0; i < num_phrases; i++) {
        if (strcmp(phrases[i], "Direct") == 0) {
            question_types[i] = 1;
        }
        else {
            question_types[i] = 0;
        }
    }   
    return;     
}

/* store possible answers - Likert levels */
int process_likert_level_descriptions(char phrases[][MAX_PHRASE_LENGTH], int num_phrases, 
    char likert_level_descriptions[][MAX_WORD_LENGTH]) {

    /* for every phrase in phrases, store it as a possible Likert answer  */
    for (int i = 0; i < num_phrases; i++) {
        strncpy(likert_level_descriptions[i], phrases[i], MAX_WORD_LENGTH);
    }
    return num_phrases;          
}

/* stores next survey response */
void process_response(char phrases[][MAX_PHRASE_LENGTH], int num_phrases, Survey *survey,
    int curr_respondent_index) {
    strcpy(survey->respondents[curr_respondent_index].field_of_study, phrases[0]);
    strcpy(survey->respondents[curr_respondent_index].born_in_canada, phrases[1]);
    strcpy(survey->respondents[curr_respondent_index].date_of_birth, phrases[2]);

    /* for every phrase in phrases, convert it to a scale and store it as a response
       indices start from 0 in question_types and responses
       index starts from 3 in phrases to ignore respondent's information
     */
    for (int i = 3; i < num_phrases; i++) {
        short scale = convert_response_to_scale(phrases[i], survey->question_types[i-3]);
        survey->respondents[curr_respondent_index].responses[i-3] = scale;
    }
    return;     
}

/* convert Likert item to numerical scale */
short convert_response_to_scale(char *token, short direct_scale) {
    short scale;
    if (strcmp(token, "fully disagree") == 0) {
        scale = 1;
    }
    else if (strcmp(token, "disagree") == 0) {
        scale = 2;
    }
    else if (strcmp(token, "partially disagree") == 0) {
        scale = 3;
    }
    else if (strcmp(token, "partially agree") == 0) {
        scale = 4;
    }
    else if (strcmp(token, "agree") == 0) {
        scale = 5;
    }
    else {
        scale = 6;
    }

    return scale;
}

/* identify the type of filter and perform filtering */
void process_filter(char phrases[][MAX_PHRASE_LENGTH], int num_phrases, Survey *survey) {
    int filter_type = atoi(phrases[0]);
    switch (filter_type)
    {
    case 0: // filter by field of study
        filter_by_field_of_study(survey, phrases[1]);
        break;
        
    case 1: // filter by born in canada
        filter_by_born_in_canada(survey, phrases[1]);
        break;

    case 2: // filter by age range
        filter_by_age_range(survey, atoi(phrases[1]), atoi(phrases[2]));
        break;

    default:
        break;
    }
}
