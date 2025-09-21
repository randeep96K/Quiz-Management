#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

// Priority Queue for difficulty selection
typedef struct PQNode {
    Question* question;
    int priority; // difficulty level
    struct PQNode* next;
} PQNode;

typedef struct {
    PQNode* front;
} PriorityQueue;

// Function declarations
void init_pq(PriorityQueue* pq);
void enqueue_pq(PriorityQueue* pq, Question* question, int priority);
Question* dequeue_pq(PriorityQueue* pq);
int is_pq_empty(PriorityQueue* pq);
void free_pq(PriorityQueue* pq);

#endif