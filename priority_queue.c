#include "priority_queue.h"

void init_pq(PriorityQueue* pq) {
    pq->front = NULL;
}

void enqueue_pq(PriorityQueue* pq, Question* question, int priority) {
    PQNode* new_node = (PQNode*)malloc(sizeof(PQNode));
    if (!new_node) return;
    
    new_node->question = question;
    new_node->priority = priority;
    new_node->next = NULL;
    
    if (pq->front == NULL || priority < pq->front->priority) {
        new_node->next = pq->front;
        pq->front = new_node;
    } else {
        PQNode* temp = pq->front;
        while (temp->next && temp->next->priority <= priority) {
            temp = temp->next;
        }
        new_node->next = temp->next;
        temp->next = new_node;
    }
}

Question* dequeue_pq(PriorityQueue* pq) {
    if (pq->front == NULL) return NULL;
    
    PQNode* temp = pq->front;
    Question* question = temp->question;
    pq->front = pq->front->next;
    free(temp);
    
    return question;
}

int is_pq_empty(PriorityQueue* pq) {
    return pq->front == NULL;
}

void free_pq(PriorityQueue* pq) {
    while (!is_pq_empty(pq)) {
        dequeue_pq(pq);
    }
}