#include "quiz.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// Global variables definition
Question* question_list = NULL;
User* user_tree = NULL;
HashMap subject_map;
PriorityQueue pq;
Stack pause_stack;
Queue random_queue;

void display_menu() {
    printf("\n=== QUIZ MANAGEMENT SYSTEM ===\n");
    printf("1. Start New Quiz\n");
    printf("2. Resume Test\n");
    printf("3. View Leaderboard\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
    fflush(stdout);
}

void conduct_quiz(User* user, int difficulty) {
    // Prepare questions based on difficulty using Priority Queue
    init_pq(&pq);
    Question* temp = question_list;
    int available_questions = 0;
    
    // Count available questions for this difficulty
    while (temp) {
        if (temp->difficulty == difficulty) {
            enqueue_pq(&pq, temp, temp->difficulty);
            available_questions++;
        }
        temp = temp->next;
    }
    
    if (available_questions == 0) {
        printf("No questions available for this difficulty level!\n");
        return;
    }
    
    // Fill random queue with selected questions
    init_queue(&random_queue);
    Question* selected[QUIZ_SIZE];
    int count = 0;
    
    while (!is_pq_empty(&pq) && count < QUIZ_SIZE && count < available_questions) {
        Question* q = dequeue_pq(&pq);
        if (q) {
            selected[count++] = q;
        }
    }
    
    if (count == 0) {
        printf("No questions could be loaded!\n");
        return;
    }
    
    // Shuffle questions for randomness
    srand(time(NULL));
    for (int i = count - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Question* temp_q = selected[i];
        selected[i] = selected[j];
        selected[j] = temp_q;
    }
    
    // Add shuffled questions to queue
    for (int i = 0; i < count; i++) {
        enqueue(&random_queue, selected[i]);
    }
    
    printf("\n=== QUIZ STARTED ===\n");
    printf("Difficulty: %s\n", (difficulty == 1) ? "Easy" : (difficulty == 2) ? "Medium" : "Hard");
    printf("Total Questions: %d\n\n", count);
    fflush(stdout);
    
    int question_num = 1;
    char answer;
    
    while (!is_queue_empty(&random_queue) && question_num <= count) {
        Question* current = dequeue(&random_queue);
        if (!current) break;
        
        printf("Question %d: %s\n", question_num, current->question);
        printf("A) %s\n", current->optionA);
        printf("B) %s\n", current->optionB);
        printf("C) %s\n", current->optionC);
        printf("D) %s\n", current->optionD);
        printf("\nYour answer (A/B/C/D) or P to pause: ");
        fflush(stdout);
        
        if (scanf(" %c", &answer) != 1) {
            printf("Error reading answer!\n");
            break;
        }
        answer = toupper(answer);
        
        if (answer == 'P') {
            // Pause functionality using Stack
            push_stack(&pause_stack, current->id);
            if (user->paused_count < QUIZ_SIZE) {
                user->paused_questions[user->paused_count++] = current->id;
            }
            printf("Quiz paused. You can resume later.\n");
            return;
        }
        
        if (answer == current->correct) {
            user->score++;
            printf("Correct!\n\n");
        } else {
            if (user->wrong_count < QUIZ_SIZE) {
                strncpy(user->wrong_answers[user->wrong_count], current->question, MAX_QUESTION - 1);
                user->wrong_answers[user->wrong_count][MAX_QUESTION - 1] = '\0';
                user->wrong_count++;
            }
            printf("Wrong! Correct answer was %c\n\n", current->correct);
        }
        
        question_num++;
        user->current_question = question_num;
        fflush(stdout);
    }
    
    user->completed = 1;
    printf("=== QUIZ COMPLETED ===\n");
    printf("Your Score: %d/%d\n", user->score, count);
    printf("Percentage: %.2f%%\n", (user->score * 100.0) / count);
    
    if (user->wrong_count > 0) {
        printf("\nWrong Answers:\n");
        for (int i = 0; i < user->wrong_count; i++) {
            printf("- %s\n", user->wrong_answers[i]);
        }
    }
    
    // Save user analysis
    save_user_analysis(user);
    
    // Update leaderboard
    FILE* leaderboard = fopen("leaderboard.csv", "w");
    if (leaderboard) {
        fprintf(leaderboard, "Name,Score\n");
        save_leaderboard(user_tree, leaderboard);
        fclose(leaderboard);
    }
}

void start_new_quiz() {
    char name[MAX_NAME];
    int difficulty;
    
    printf("\nEnter your name: ");
    fflush(stdout);
    if (scanf("%49s", name) != 1) {
        printf("Error reading name!\n");
        return;
    }
    
    // Check if user already exists
    User* existing_user = search_user(user_tree, name);
    if (existing_user && existing_user->completed) {
        printf("User already completed the quiz. Score: %d\n", existing_user->score);
        return;
    }
    
    User* user;
    if (existing_user) {
        user = existing_user;
    } else {
        user = create_user(name);
        if (!user) {
            printf("Error creating user!\n");
            return;
        }
        user_tree = insert_user(user_tree, user);
    }
    
    printf("\nSelect Difficulty:\n");
    printf("1. Easy\n");
    printf("2. Medium\n");
    printf("3. Hard\n");
    printf("Enter choice: ");
    fflush(stdout);
    
    if (scanf("%d", &difficulty) != 1) {
        printf("Error reading difficulty!\n");
        return;
    }
    
    if (difficulty < 1 || difficulty > 3) {
        printf("Invalid difficulty level!\n");
        return;
    }
    
    conduct_quiz(user, difficulty);
}

void resume_quiz() {
    char name[MAX_NAME];
    printf("\nEnter your name to resume: ");
    fflush(stdout);
    
    if (scanf("%49s", name) != 1) {
        printf("Error reading name!\n");
        return;
    }
    
    User* user = search_user(user_tree, name);
    if (!user) {
        printf("User not found!\n");
        return;
    }
    
    if (user->completed) {
        printf("Quiz already completed! Score: %d\n", user->score);
        return;
    }
    
    if (user->paused_count == 0) {
        printf("No paused quiz found for this user!\n");
        return;
    }
    
    printf("Resuming quiz for %s...\n", user->name);
    printf("Current score: %d\n", user->score);
    
    // Resume from stack (LIFO)
    init_queue(&random_queue);
    while (!is_stack_empty(&pause_stack)) {
        int question_id = pop_stack(&pause_stack);
        Question* temp = find_question_by_id(question_list, question_id);
        
        if (temp) {
            enqueue(&random_queue, temp);
        }
    }
    
    // Continue quiz with remaining questions
    int question_num = user->current_question;
    char answer;
    
    while (!is_queue_empty(&random_queue)) {
        Question* current = dequeue(&random_queue);
        if (!current) break;
        
        printf("\nQuestion %d: %s\n", question_num, current->question);
        printf("A) %s\n", current->optionA);
        printf("B) %s\n", current->optionB);
        printf("C) %s\n", current->optionC);
        printf("D) %s\n", current->optionD);
        printf("\nYour answer (A/B/C/D): ");
        fflush(stdout);
        
        if (scanf(" %c", &answer) != 1) {
            printf("Error reading answer!\n");
            break;
        }
        answer = toupper(answer);
        
        if (answer == current->correct) {
            user->score++;
            printf("Correct!\n");
        } else {
            if (user->wrong_count < QUIZ_SIZE) {
                strncpy(user->wrong_answers[user->wrong_count], current->question, MAX_QUESTION - 1);
                user->wrong_answers[user->wrong_count][MAX_QUESTION - 1] = '\0';
                user->wrong_count++;
            }
            printf("Wrong! Correct answer was %c\n", current->correct);
        }
        
        question_num++;
    }
    
    user->completed = 1;
    user->paused_count = 0;
    
    printf("\n=== QUIZ COMPLETED ===\n");
    printf("Final Score: %d/%d\n", user->score, QUIZ_SIZE);
    
    save_user_analysis(user);
}

void view_leaderboard() {
    FILE* file = fopen("leaderboard.csv", "r");
    if (!file) {
        printf("No leaderboard data available!\n");
        return;
    }
    
    printf("\n=== LEADERBOARD ===\n");
    char line[200];
    if (fgets(line, sizeof(line), file)) { // Skip header
        printf("%-20s %s\n", "Name", "Score");
        printf("%-20s %s\n", "----", "-----");
        
        while (fgets(line, sizeof(line), file)) {
            char* name = strtok(line, ",");
            char* score = strtok(NULL, ",\n\r");
            if (name && score) {
                printf("%-20s %s\n", name, score);
            }
        }
    }
    
    fclose(file);
}

// Function to clear input buffer
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    printf("Starting Quiz Management System...\n");
    fflush(stdout);
    
    // Initialize data structures
    init_hashmap(&subject_map);
    init_pq(&pq);
    init_stack(&pause_stack);
    init_queue(&random_queue);
    
    printf("Loading questions from CSV...\n");
    fflush(stdout);
    
    // Load questions from CSV
    question_list = load_questions("questions.csv");
    if (!question_list) {
        printf("Failed to load questions. Please ensure questions.csv exists.\n");
        printf("Creating sample questions.csv file...\n");
        fflush(stdout);
        
        FILE* sample = fopen("questions.csv", "w");
        if (sample) {
            fprintf(sample, "ID,Question,OptionA,OptionB,OptionC,OptionD,Correct,Subject,Difficulty\n");
            fprintf(sample, "1,What is 2+2?,2,3,4,5,C,Math,1\n");
            fprintf(sample, "2,Capital of France?,London,Paris,Berlin,Rome,B,Geography,1\n");
            fprintf(sample, "3,Who wrote Romeo and Juliet?,Shakespeare,Dickens,Austen,Tolkien,A,Literature,2\n");
            fprintf(sample, "4,What is the chemical symbol for gold?,Au,Ag,Fe,Cu,A,Chemistry,2\n");
            fprintf(sample, "5,Which planet is closest to the sun?,Venus,Mercury,Earth,Mars,B,Science,1\n");
            fprintf(sample, "6,What is the square root of 144?,10,11,12,13,C,Math,1\n");
            fprintf(sample, "7,Who painted the Mona Lisa?,Van Gogh,Picasso,Da Vinci,Monet,C,Art,2\n");
            fprintf(sample, "8,What is the largest ocean?,Atlantic,Indian,Arctic,Pacific,D,Geography,1\n");
            fprintf(sample, "9,What year did World War II end?,1944,1945,1946,1947,B,History,2\n");
            fprintf(sample, "10,What is the speed of light?,300000 km/s,150000 km/s,450000 km/s,600000 km/s,A,Physics,3\n");
            fclose(sample);
            
            printf("Sample questions.csv created. Restarting with sample data...\n");
            fflush(stdout);
            
            // Try loading again
            question_list = load_questions("questions.csv");
        }
        
        if (!question_list) {
            printf("Still failed to load questions. Exiting.\n");
            return 1;
        }
    }
    
    printf("Questions loaded successfully!\n");
    fflush(stdout);
    
    // Populate hash map with questions by subject
    Question* temp = question_list;
    while (temp) {
        insert_subject(&subject_map, temp->subject, temp);
        temp = temp->next;
    }
    
    int choice;
    
    while (1) {
        display_menu();
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            clear_input_buffer();
            continue;
        }
        
        switch (choice) {
            case 1:
                start_new_quiz();
                break;
            case 2:
                resume_quiz();
                break;
            case 3:
                view_leaderboard();
                break;
            case 4:
                printf("Thank you for using Quiz Management System!\n");
                
                // Clean up memory
                free_questions(question_list);
                free_user_tree(user_tree);
                free_hashmap(&subject_map);
                free_pq(&pq);
                free_stack(&pause_stack);
                free_queue(&random_queue);
                
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
    
    return 0;
}