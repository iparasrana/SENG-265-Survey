/*
 * Library includes
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "input_handling.h"
#include "output.h"
#include "processing.h"
#include "emalloc.h"

/* main function: handles input, processes data, prints output */
int main(int argc, char *argv[])
{
    if (argc != 1) {
        printf("Usage: %s\n", argv[0]);
        printf("Should receive no parameters\n");
        printf("Read from the stdin instead\n");
        exit(1);
    }

    /* declaration and initialization of local variables */
    Survey *survey = emalloc(sizeof(Survey));
    survey->question_types = emalloc(NUMBER_QUESTIONS *sizeof(short));

    // to store data after line tokenization
    int num_phrases = 0;
    char phrases[MAX_NUMBER_PHRASES][MAX_PHRASE_LENGTH];

    // to store test information
    int num_tests = 0;
    short *tests = emalloc(NUMBER_TESTS * sizeof(short));

    // to store score of each respondent by questions category 
    double **scores = emalloc(MAX_NUMBER_RESPONSES * sizeof(double *));
    for (int i=0; i<MAX_NUMBER_RESPONSES; i++) {
        scores[i] = emalloc(NUMBER_SCORES * sizeof(double));
        for (int j=0; j<NUMBER_SCORES; j++) {
            scores[i][j] = 0.0;
        }
    }

    // to store survey average scores by question category
    double *average_scores = emalloc(NUMBER_SCORES * sizeof(double));

    // store response frequencies for each question 
    double **frequencies = emalloc(NUMBER_QUESTIONS * sizeof(double *));
    for (int i=0; i<NUMBER_QUESTIONS; i++) {
        frequencies[i] = emalloc(NUMBER_LIKERT_LEVELS * sizeof(double));
        for (int j=0; j<NUMBER_LIKERT_LEVELS; j++) {
            frequencies[i][j] = 0.0;
        }
    }

    char line[MAX_LINE_LENGTH];
    int phase = 0;
    int curr_respondent_index = 0;
    
    /* Read the input lines and process them */
    while ( fgets(line, sizeof(char) * MAX_LINE_LENGTH, stdin) ) {
        if (line[0] == '#') {
            continue;
        }
        switch (phase) {
        case 0: // test bits
            num_phrases = tokenize_line(line, phrases, ",\n");
            num_tests = process_tests(phrases, num_phrases, tests);
            phase++;
            break;
        case 1: // survey questions
            num_phrases = tokenize_line(line, phrases, ";\n");
            survey->num_questions = process_questions(phrases, num_phrases, survey->questions);
            phase++;
            break;
        case 2: // question types
            num_phrases = tokenize_line(line, phrases, ";\n");
            process_question_types(phrases, num_phrases, 
                survey->question_types);
            phase++;
            break;
        case 3: // likert levels 
            num_phrases = tokenize_line(line, phrases, ",\n");
            survey->num_likert_levels = process_likert_level_descriptions(phrases, 
                num_phrases, survey->likert_level_descriptions);
            phase++;
            break;
        case 4: // number of respondents
            survey->num_respondents = atoi(line);
            survey->respondents = emalloc(sizeof(Respondent)*survey->num_respondents);
            for(int i = 0; i < survey->num_respondents; i++){
                survey->respondents[i].responses = emalloc(survey->num_questions * sizeof(short));
            }
            phase++;
            break;
        case 5: // survey responses
            num_phrases = tokenize_line(line, phrases, ",\n");
            process_response(phrases, num_phrases, survey, curr_respondent_index);
            curr_respondent_index++;
            if (curr_respondent_index == survey->num_respondents){
                phase++;
            }
            break;
        case 6: // filters
            num_phrases = tokenize_line(line, phrases, ",\n");
            process_filter(phrases, num_phrases, survey);
            break;
        }
    }

    /* survey data processing */
    compute_frequencies(frequencies, survey);
    compute_scores(scores, survey);
    compute_average_scores(average_scores, scores, survey->num_respondents);

    /* print results */
    print_output(tests, num_tests, survey, frequencies, scores, average_scores);

    // free dynamic allocated memory
    for(int i = 0; i < survey->num_respondents; i++){
        free(survey->respondents[i].responses);
    }
    free(survey->question_types);
    free(survey->respondents);
    free(survey);

    for (int i=0; i<NUMBER_QUESTIONS; i++) {
        free(frequencies[i]);
    }
    free(frequencies);
    free(average_scores);

    for (int i=0; i<MAX_NUMBER_RESPONSES; i++) {
        free(scores[i]);
    }
    free(scores);
    free(tests);
    
    return(0);
}
