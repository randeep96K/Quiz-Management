#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>

// Stack for pause/resume functionality
typedef struct StackNode {
    int question_id;
    struct StackNode* next;
} StackNode;

typedef struct {
    StackNode* top;
} Stack;

// Function declarations
void init_stack(Stack* stack);
void push_stack(Stack* stack, int question_id);
int pop_stack(Stack* stack);
int is_stack_empty(Stack* stack);
void free_stack(Stack* stack);

#endif