#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USERNAME "admin"
#define PASSWORD "1234"

typedef struct Task {
    int id;
    char title[100];
    char description[200];
    int priority;
    struct Task* next;
} Task;

Task* head = NULL;
int taskCounter = 1;

int login() {
    char username[50], password[50];
    int attempts = 3;

    while (attempts--) {
        printf("\n--- Login ---\n");
        printf("Username: ");
        scanf("%s", username);
        printf("Password: ");
        scanf("%s", password);

        if (strcmp(username, USERNAME) == 0 && strcmp(password, PASSWORD) == 0) {
            printf("Login successful.\n");
            return 1;
        } else {
            printf("Invalid credentials. Attempts left: %d\n", attempts);
        }
    }

    printf("Too many failed attempts. Exiting...\n");
    return 0;
}

void addTask() {
    Task* newTask = (Task*)malloc(sizeof(Task));
    newTask->id = taskCounter++;

    printf("Enter title: ");
    scanf(" %[^\n]", newTask->title);
    printf("Enter description: ");
    scanf(" %[^\n]", newTask->description);
    printf("Enter priority (1-High, 2-Medium, 3-Low): ");
    scanf("%d", &newTask->priority);

    newTask->next = head;
    head = newTask;

    printf("Task added successfully!\n");
}

void sortTasksByPriority() {
    if (!head || !head->next) return;

    Task *i, *j;
    for (i = head; i != NULL; i = i->next) {
        for (j = i->next; j != NULL; j = j->next) {
            if (i->priority > j->priority) {
               // Swap date between I and J
                int tempId = i->id;
                char tempTitle[100], tempDesc[200];
                int tempPriority = i->priority;

                strcpy(tempTitle, i->title);
                strcpy(tempDesc, i->description);

                i->id = j->id;
                strcpy(i->title, j->title);
                strcpy(i->description, j->description);
                i->priority = j->priority;

                j->id = tempId;
                strcpy(j->title, tempTitle);
                strcpy(j->description, tempDesc);
                j->priority = tempPriority;
            }
        }
    }
}


void displayTasks() {
    sortTasksByPriority();

    Task* current = head;
    if (!current) {
        printf("No tasks found.\n");
        return;
    }
    while (current) {
        printf("\nTask ID: %d\nTitle: %s\nDescription: %s\nPriority: %d\n",
               current->id, current->title, current->description, current->priority);
        current = current->next;
    }
}

void deleteTask(int id) {
    Task *temp = head, *prev = NULL;

    while (temp && temp->id != id) {
        prev = temp;
        temp = temp->next;
    }

    if (!temp) {
        printf("Task not found.\n");
        return;
    }

    if (!prev)
        head = temp->next;
    else
        prev->next = temp->next;

    free(temp);
    printf("Task deleted successfully.\n");
}

void updateTask(int id) {
    Task* current = head;
    while (current) {
        if (current->id == id) {
            printf("Enter new title: ");
            scanf(" %[^\n]", current->title);
            printf("Enter new description: ");
            scanf(" %[^\n]", current->description);
            printf("Enter new priority (1-High, 2-Medium, 3-Low): ");
            scanf("%d", &current->priority);
            printf("Task updated successfully.\n");
            return;
        }
        current = current->next;
    }
    printf("Task not found.\n");
}

void saveTasksToFile() {
    FILE* file = fopen("tasks.txt", "w");
    if (!file) {
        printf("Error opening file.\n");
        return;
    }

    Task* current = head;
    while (current) {
        fprintf(file, "%d|%s|%s|%d\n", current->id, current->title, current->description, current->priority);
        current = current->next;
    }

    fclose(file);
    printf("Tasks saved to file.\n");
}

void loadTasksFromFile() {
    FILE* file = fopen("tasks.txt", "r");
    if (!file){
        printf("Error opening file.\n");
        return;
    }

    Task* newTask;
    while (!feof(file)) {
        newTask = (Task*)malloc(sizeof(Task));
        if (fscanf(file, "%d|%[^|]|%[^|]|%d\n", &newTask->id, newTask->title, newTask->description, &newTask->priority) != 4) {
            free(newTask);
            break;
        }

        if (newTask->id >= taskCounter)
            taskCounter = newTask->id + 1;

        newTask->next = head;
        head = newTask;
    }

    fclose(file);
}

int main() {
    int choice, id;

    if (!login()) return 0;

    loadTasksFromFile();

    while (1) {
        printf("\n--- Task Manager ---\n");
        printf("1. Add Task\n2. Display Tasks (Sorted by Priority)\n3. Delete Task\n4. Exit\n5. Update Task\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addTask(); break;
            case 2: displayTasks(); break;
            case 3:
                printf("Enter task ID to delete: ");
                scanf("%d", &id);
                deleteTask(id);
                break;
            case 4:
                saveTasksToFile();
                printf("Exiting...\n");
                return 0;
            case 5:
                printf("Enter task ID to update: ");
                scanf("%d", &id);
                updateTask(id);
                break;
            default:
                printf("Invalid choice.\n");
        }
    }
}
