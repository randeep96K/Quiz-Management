#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constants moved to quiz.h to avoid redefinition
#ifndef MAX_QUESTION
#define MAX_QUESTION 500
#endif

#ifndef MAX_OPTION
#define MAX_OPTION 200
#endif

#ifndef MAX_SUBJECT
#define MAX_SUBJECT 20
#endif

// Question structure (Linked List Node)
typedef struct Question {
    int id;
    char question[MAX_QUESTION];
    char optionA[MAX_OPTION];
    char optionB[MAX_OPTION];
    char optionC[MAX_OPTION];
    char optionD[MAX_OPTION];
    char correct;
    char subject[MAX_SUBJECT];
    int difficulty; // 1=Easy, 2=Medium, 3=Hard
    struct Question* next;
} Question;

// Function declarations
Question* load_questions(const char* filename);
void add_question(Question** head, Question* new_question);
void free_questions(Question* head);
Question* find_question_by_id(Question* head, int id);

#endif