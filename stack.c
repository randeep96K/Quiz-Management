#include "stack.h"

void init_stack(Stack* stack) {
    stack->top = NULL;
}

void push_stack(Stack* stack, int question_id) {
    StackNode* new_node = (StackNode*)malloc(sizeof(StackNode));
    if (!new_node) return;
    
    new_node->question_id = question_id;
    new_node->next = stack->top;
    stack->top = new_node;
}

int pop_stack(Stack* stack) {
    if (stack->top == NULL) return -1;
    
    StackNode* temp = stack->top;
    int question_id = temp->question_id;
    stack->top = stack->top->next;
    free(temp);
    
    return question_id;
}

int is_stack_empty(Stack* stack) {
    return stack->top == NULL;
}

void free_stack(Stack* stack) {
    while (!is_stack_empty(stack)) {
        pop_stack(stack);
    }
}