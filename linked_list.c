#include "linked_list.h"

Question* load_questions(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open questions file!\n");
        return NULL;
    }
    
    Question* head = NULL;
    char line[1000];
    fgets(line, sizeof(line), file); // Skip header
    
    while (fgets(line, sizeof(line), file)) {
        Question* q = (Question*)malloc(sizeof(Question));
        if (!q) continue;
        
        char* token = strtok(line, ",");
        if (!token) {
            free(q);
            continue;
        }
        q->id = atoi(token);
        
        token = strtok(NULL, ",");
        if (!token) {
            free(q);
            continue;
        }
        strncpy(q->question, token, MAX_QUESTION - 1);
        q->question[MAX_QUESTION - 1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) {
            free(q);
            continue;
        }
        strncpy(q->optionA, token, MAX_OPTION - 1);
        q->optionA[MAX_OPTION - 1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) {
            free(q);
            continue;
        }
        strncpy(q->optionB, token, MAX_OPTION - 1);
        q->optionB[MAX_OPTION - 1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) {
            free(q);
            continue;
        }
        strncpy(q->optionC, token, MAX_OPTION - 1);
        q->optionC[MAX_OPTION - 1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) {
            free(q);
            continue;
        }
        strncpy(q->optionD, token, MAX_OPTION - 1);
        q->optionD[MAX_OPTION - 1] = '\0';
        
        token = strtok(NULL, ",");
        if (!token) {
            free(q);
            continue;
        }
        q->correct = token[0];
        
        token = strtok(NULL, ",");
        if (!token) {
            free(q);
            continue;
        }
        strncpy(q->subject, token, MAX_SUBJECT - 1);
        q->subject[MAX_SUBJECT - 1] = '\0';
        
        token = strtok(NULL, ",\n\r");
        if (!token) {
            free(q);
            continue;
        }
        q->difficulty = atoi(token);
        
        q->next = NULL;
        add_question(&head, q);
    }
    
    fclose(file);
    return head;
}

void add_question(Question** head, Question* new_question) {
    if (*head == NULL) {
        *head = new_question;
    } else {
        Question* temp = *head;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = new_question;
    }
}

void free_questions(Question* head) {
    while (head) {
        Question* temp = head;
        head = head->next;
        free(temp);
    }
}

Question* find_question_by_id(Question* head, int id) {
    Question* temp = head;
    while (temp) {
        if (temp->id == id) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}