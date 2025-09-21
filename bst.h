#ifndef BST_H
#define BST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constants - use conditional defines to avoid redefinition
#ifndef MAX_NAME
#define MAX_NAME 50
#endif

#ifndef MAX_QUESTION
#define MAX_QUESTION 500
#endif

#ifndef QUIZ_SIZE
#define QUIZ_SIZE 10
#endif

// User structure (BST Node)
typedef struct User {
    char name[MAX_NAME];
    int score;
    int completed;
    int current_question;
    int paused_questions[QUIZ_SIZE];
    int paused_count;
    char wrong_answers[QUIZ_SIZE][MAX_QUESTION];
    int wrong_count;
    struct User* left;
    struct User* right;
} User;

// Function declarations
User* create_user(const char* name);
User* insert_user(User* root, User* new_user);
User* search_user(User* root, const char* name);
void save_user_analysis(User* user);
void save_leaderboard(User* root, FILE* file);
void free_user_tree(User* root);

#endif