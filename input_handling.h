#ifndef _INPUT_HANDLING_H_
#define _INPUT_HANDLING_H_

#include "dyn_survey.h"

/* input handling functions */
int tokenize_line(char *line, char phrases[][MAX_PHRASE_LENGTH], char delimiter[]);
int process_tests(char phrases[][MAX_PHRASE_LENGTH], int num_phrases, short tests[]);
int process_questions(char phrases[][MAX_PHRASE_LENGTH], 
    int num_phrases, char questions[][MAX_QUESTION_LENGTH]);
void process_question_types(char phrases[][MAX_PHRASE_LENGTH], 
    int num_phrases, short question_types[]);
int process_likert_level_descriptions(char phrases[][MAX_PHRASE_LENGTH], 
    int num_phrases, char likert_level_descriptions[][MAX_WORD_LENGTH]);
void process_response(char phrases[][MAX_PHRASE_LENGTH], int num_phrases, Survey *survey,
    int curr_respondent_index);
short convert_response_to_scale(char *token, short direct_scale);
void process_filter(char phrases[][MAX_PHRASE_LENGTH], int num_phrases, Survey *survey);

#endif
