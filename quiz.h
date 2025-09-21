#ifndef QUIZ_H
#define QUIZ_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// Define constants first
#define MAX_NAME 50
#define MAX_QUESTION 500
#define MAX_OPTION 200
#define MAX_SUBJECT 20
#define QUIZ_SIZE 10
#define TABLE_SIZE 100

// Include all data structure headers
#include "linked_list.h"
#include "bst.h"
#include "hashmap.h"
#include "priority_queue.h"
#include "stack.h"
#include "queue.h"

// Global variables
extern Question* question_list;
extern User* user_tree;
extern HashMap subject_map;
extern PriorityQueue pq;
extern Stack pause_stack;
extern Queue random_queue;

// Quiz operation function declarations
void display_menu();
void conduct_quiz(User* user, int difficulty);
void start_new_quiz();
void resume_quiz();
void view_leaderboard();
void clear_input_buffer();

#endif