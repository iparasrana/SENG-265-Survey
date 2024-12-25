#include <stdio.h>
#include "output.h"

/* Print outputs to stdout */
void print_output(short tests[], int num_tests, Survey *survey,
    double **frequencies, double **scores,
    double average_scores[]) {

    /* print header */
    printf("Examining Science and Engineering Students' Attitudes Towards Computer Science\n");
    printf("SURVEY RESPONSE STATISTICS\n");
    printf("\nNUMBER OF RESPONDENTS: %d\n", survey->num_respondents);

    /* show relative percentual frequencies */
    if (tests[0]) {
        printf("\nFOR EACH QUESTION BELOW, RELATIVE PERCENTUAL FREQUENCIES ARE COMPUTED FOR EACH LEVEL OF AGREEMENT\n");

        for (int i = 0; i < survey->num_questions; i++) {
            printf("\n%s\n", survey->questions[i]);
            for (int j = 0; j < survey->num_likert_levels; j++) {
                printf("%.2f: %s\n", frequencies[i][j], survey->likert_level_descriptions[j]);
            }
        }        
    }

    /* show respondents' scores */
    if (tests[1]) {
        printf("\nSCORES FOR ALL THE RESPONDENTS\n");
        printf("\n");
        for (int i = 0; i < survey->num_respondents; i++) {
            printf("C:%.2f,I:%.2f,G:%.2f,U:%.2f,P:%.2f\n", scores[i][0], scores[i][1], scores[i][2], scores[i][3], scores[i][4]);
        }
    }

    /* show average scores per respondent */
    if (tests[2]) {
        printf("\nAVERAGE SCORES PER RESPONDENT\n");
        printf("\n");
        printf("C:%.2f,I:%.2f,G:%.2f,U:%.2f,P:%.2f\n", average_scores[0], average_scores[1], 
            average_scores[2], average_scores[3], average_scores[4]);
    }

    return;
}