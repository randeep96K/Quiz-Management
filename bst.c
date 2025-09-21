#include "bst.h"

User* create_user(const char* name) {
    User* user = (User*)malloc(sizeof(User));
    if (!user) return NULL;
    
    strncpy(user->name, name, MAX_NAME - 1);
    user->name[MAX_NAME - 1] = '\0';
    user->score = 0;
    user->completed = 0;
    user->current_question = 0;
    user->paused_count = 0;
    user->wrong_count = 0;
    user->left = user->right = NULL;
    
    return user;
}

User* insert_user(User* root, User* new_user) {
    if (root == NULL) {
        return new_user;
    }
    
    int cmp = strcmp(new_user->name, root->name);
    if (cmp < 0) {
        root->left = insert_user(root->left, new_user);
    } else if (cmp > 0) {
        root->right = insert_user(root->right, new_user);
    } else {
        // User already exists, free the new user and return existing
        free(new_user);
        return root;
    }
    
    return root;
}

User* search_user(User* root, const char* name) {
    if (root == NULL) return NULL;
    
    int cmp = strcmp(name, root->name);
    if (cmp == 0) return root;
    else if (cmp < 0) return search_user(root->left, name);
    else return search_user(root->right, name);
}

void save_user_analysis(User* user) {
    FILE* file = fopen("user_analysis.csv", "a");
    if (!file) return;
    
    fprintf(file, "%s,%d,%d,", user->name, user->score, user->wrong_count);
    for (int i = 0; i < user->wrong_count; i++) {
        fprintf(file, "%s;", user->wrong_answers[i]);
    }
    fprintf(file, "\n");
    fclose(file);
}

void save_leaderboard(User* root, FILE* file) {
    if (root) {
        save_leaderboard(root->left, file);
        if (root->completed) {
            fprintf(file, "%s,%d\n", root->name, root->score);
        }
        save_leaderboard(root->right, file);
    }
}

void free_user_tree(User* root) {
    if (root) {
        free_user_tree(root->left);
        free_user_tree(root->right);
        free(root);
    }
}