#include "queue.h"

void init_queue(Queue* queue) {
    queue->front = queue->rear = NULL;
}

void enqueue(Queue* queue, Question* question) {
    QueueNode* new_node = (QueueNode*)malloc(sizeof(QueueNode));
    if (!new_node) return;
    
    new_node->question = question;
    new_node->next = NULL;
    
    if (queue->rear == NULL) {
        queue->front = queue->rear = new_node;
    } else {
        queue->rear->next = new_node;
        queue->rear = new_node;
    }
}

Question* dequeue(Queue* queue) {
    if (queue->front == NULL) return NULL;
    
    QueueNode* temp = queue->front;
    Question* question = temp->question;
    queue->front = queue->front->next;
    
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    
    free(temp);
    return question;
}

int is_queue_empty(Queue* queue) {
    return queue->front == NULL;
}

void free_queue(Queue* queue) {
    while (!is_queue_empty(queue)) {
        dequeue(queue);
    }
}