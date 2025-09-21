#include "hashmap.h"

unsigned int hash_function(const char* key) {
    unsigned int hash_value = 5381;
    int c;
    while ((c = *key++)) {
        hash_value = ((hash_value << 5) + hash_value) + c;
    }
    return hash_value % TABLE_SIZE;
}

void init_hashmap(HashMap* map) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        map->buckets[i] = NULL;
    }
}

void insert_subject(HashMap* map, const char* subject, Question* question) {
    unsigned int index = hash_function(subject);
    SubjectNode* node = map->buckets[index];
    
    while (node) {
        if (strcmp(node->subject, subject) == 0) {
            // Create a copy of the question for the hash map
            Question* q_copy = (Question*)malloc(sizeof(Question));
            if (q_copy) {
                *q_copy = *question;
                q_copy->next = NULL;
                add_question(&(node->questions), q_copy);
            }
            return;
        }
        node = node->next;
    }
    
    // Create new subject node
    SubjectNode* new_node = (SubjectNode*)malloc(sizeof(SubjectNode));
    if (!new_node) return;
    
    strncpy(new_node->subject, subject, MAX_SUBJECT - 1);
    new_node->subject[MAX_SUBJECT - 1] = '\0';
    new_node->questions = NULL;
    
    // Create a copy of the question
    Question* q_copy = (Question*)malloc(sizeof(Question));
    if (q_copy) {
        *q_copy = *question;
        q_copy->next = NULL;
        add_question(&(new_node->questions), q_copy);
    }
    
    new_node->next = map->buckets[index];
    map->buckets[index] = new_node;
}

Question* get_questions_by_subject(HashMap* map, const char* subject) {
    unsigned int index = hash_function(subject);
    SubjectNode* node = map->buckets[index];
    
    while (node) {
        if (strcmp(node->subject, subject) == 0) {
            return node->questions;
        }
        node = node->next;
    }
    return NULL;
}

void free_hashmap(HashMap* map) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        SubjectNode* node = map->buckets[i];
        while (node) {
            SubjectNode* temp = node;
            node = node->next;
            free_questions(temp->questions);
            free(temp);
        }
        map->buckets[i] = NULL;
    }
}