#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

// Constants - use conditional defines to avoid redefinition
#ifndef TABLE_SIZE
#define TABLE_SIZE 100
#endif

#ifndef MAX_SUBJECT
#define MAX_SUBJECT 20
#endif

// Hash Map for subject categorization
typedef struct SubjectNode {
    char subject[MAX_SUBJECT];
    Question* questions;
    struct SubjectNode* next;
} SubjectNode;

typedef struct {
    SubjectNode* buckets[TABLE_SIZE];
} HashMap;

// Function declarations
unsigned int hash_function(const char* key);
void init_hashmap(HashMap* map);
void insert_subject(HashMap* map, const char* subject, Question* question);
Question* get_questions_by_subject(HashMap* map, const char* subject);
void free_hashmap(HashMap* map);

#endif