#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

// Queue for random question ordering
typedef struct QueueNode {
    Question* question;
    struct QueueNode* next;
} QueueNode;

typedef struct {
    QueueNode* front;
    QueueNode* rear;
} Queue;

// Function declarations
void init_queue(Queue* queue);
void enqueue(Queue* queue, Question* question);
Question* dequeue(Queue* queue);
int is_queue_empty(Queue* queue);
void free_queue(Queue* queue);

#endif