
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>

typedef enum {
    CMD_ADD,
    CMD_DELETE,
    CMD_VIEW,
    CMD_UPDATE,
    CMD_COMPLETE,
    CMD_FIND,
    CMD_HELP,
    CMD_EXIT,
    CMD_UNKNOWN
} CommandType;

typedef enum {
    PENDING,
    DONE
} TaskStatus;

typedef struct {
    int id;
    char description[100];
    TaskStatus status;
} Task;

void trim(char *str) {

    char *start = str;

    while (isspace((unsigned char)*start)) {
        start++;
    }

    memmove(str, start, strlen(start) + 1);

    size_t len = strlen(str);

    while (len > 0 && isspace((unsigned char)str[len - 1])) {
        str[--len] = '\0';
    }
}


// 5. Function for safe bounded input
// Returns 1 = success, 0 = EOF, -1 = overlong input

int read_line(const char *message, char *buffer, size_t size) {

    printf("%s", message);

    if (fgets(buffer, size, stdin) == NULL) {
        return 0;
    }

    size_t len = strlen(buffer);

    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        return 1;
    }

    int ch = getchar();

    if (ch == EOF) {
        return 1;
    }

    while (ch != '\n' && ch != EOF) {
        ch = getchar();
    }

    return -1;
}

CommandType get_command_type(const char *commandStr) {

    if (strcmp(commandStr, "ADD") == 0)
        return CMD_ADD;

    if (strcmp(commandStr, "DELETE") == 0)
        return CMD_DELETE;

    if (strcmp(commandStr, "VIEW") == 0)
        return CMD_VIEW;

    if (strcmp(commandStr, "UPDATE") == 0)
        return CMD_UPDATE;

    if (strcmp(commandStr, "COMPLETE") == 0)
        return CMD_COMPLETE;

    if (strcmp(commandStr, "FIND") == 0)
        return CMD_FIND;

    if (strcmp(commandStr, "HELP") == 0)
        return CMD_HELP;

    if (strcmp(commandStr, "EXIT") == 0)
        return CMD_EXIT;

    return CMD_UNKNOWN;
}

int find_task_index(Task tasks[], size_t count, int id) {

    for (size_t i = 0; i < count; i++) {

        if (tasks[i].id == id) {
            return (int)i;
        }
    }

    return -1;
}


int read_task_id(int *id) {

    char input[50];

    int result = read_line("Enter Task ID: ", input, sizeof(input));

    if (result == 0) {
        return -1;
    }

    if (result == -1) {
        printf("Error: ID input is too long.\n");
        return 0;
    }

    trim(input);

    if (input[0] == '\0') {
        printf("Error: Task ID cannot be empty.\n");
        return 0;
    }

    char *end;
    errno = 0;

    long value = strtol(input, &end, 10);

    if (errno == ERANGE || end == input ||
        *end != '\0' || value <= 0 || value > INT_MAX) {

        printf("Error: Enter a valid positive integer ID.\n");
        return 0;
    }

    *id = (int)value;

    return 1;
}


int read_description(char description[], size_t size) {

    int result = read_line(
        "Enter Task Description: ",
        description,
        size
    );

    if (result == 0) {
        return -1;
    }

    if (result == -1) {
        printf("Error: Description is too long.\n");
        return 0;
    }

    trim(description);

    if (description[0] == '\0') {
        printf("Error: Description cannot be empty.\n");
        return 0;
    }

    return 1;
}


void add_task(Task **tasks, size_t *count, size_t *capacity) {

    int id;
    char description[100];

    int result = read_task_id(&id);

    if (result == -1) {
        return;
    }

    if (result == 0) {
        return;
    }

    if (find_task_index(*tasks, *count, id) != -1) {
        printf("Error: Task ID already exists.\n");
        return;
    }

    result = read_description(description, sizeof(description));

    if (result != 1) {
        return;
    }



    if (*count == *capacity) {

        size_t new_capacity = (*capacity == 0) ? 2 : (*capacity * 2);

        Task *temp = realloc(
            *tasks,
            new_capacity * sizeof(Task)
        );

        if (temp == NULL) {
            printf("Error: Memory allocation failed.\n");
            return;
        }

        *tasks = temp;
        *capacity = new_capacity;
    }

    (*tasks)[*count].id = id;

    strcpy((*tasks)[*count].description, description);

    (*tasks)[*count].status = PENDING;

    (*count)++;

    printf("Task ID %d added successfully.\n", id);
}

void delete_task(Task tasks[], size_t *count) {

    int id;

    int result = read_task_id(&id);

    if (result != 1) {
        return;
    }

    int index = find_task_index(tasks, *count, id);

    if (index == -1) {
        printf("Error: Task ID %d not found.\n", id);
        return;
    }

    for (size_t i = (size_t)index; i < *count - 1; i++) {

        tasks[i] = tasks[i + 1];
    }

    (*count)--;

    printf("Task ID %d deleted successfully.\n", id);
}


void view_tasks(const Task tasks[], size_t count) {

    if (count == 0) {
        printf("Task list is empty.\n");
        return;
    }

    printf("\n========== ALL TASKS ==========\n");

    printf("%-10s %-30s %-12s\n",
           "Task ID", "Description", "Status");

    printf("-----------------------------------------------\n");

    for (size_t i = 0; i < count; i++) {

        printf("%-10d %-30s %-12s\n",
               tasks[i].id,
               tasks[i].description,
               tasks[i].status == PENDING ? "PENDING" : "DONE");
    }

    printf("-----------------------------------------------\n");
}


void update_task(Task tasks[], size_t count) {

    int id;
    char new_description[100];

    int result = read_task_id(&id);

    if (result != 1) {
        return;
    }

    int index = find_task_index(tasks, count, id);

    if (index == -1) {
        printf("Error: Task ID %d not found.\n", id);
        return;
    }

    result = read_description(new_description, sizeof(new_description));

    if (result != 1) {
        return;
    }

    strcpy(tasks[index].description, new_description);

    printf("Task ID %d updated successfully.\n", id);
}


void complete_task(Task tasks[], size_t count) {

    int id;

    int result = read_task_id(&id);

    if (result != 1) {
        return;
    }

    int index = find_task_index(tasks, count, id);

    if (index == -1) {
        printf("Error: Task ID %d not found.\n", id);
        return;
    }

    if (tasks[index].status == DONE) {
        printf("Task ID %d is already completed.\n", id);
        return;
    }

    tasks[index].status = DONE;

    printf("Task ID %d marked as DONE.\n", id);
}

void find_task(Task tasks[], size_t count) {

    int id;

    int result = read_task_id(&id);

    if (result != 1) {
        return;
    }

    int index = find_task_index(tasks, count, id);

    if (index == -1) {
        printf("Error: Task ID %d not found.\n", id);
        return;
    }

    printf("\n========== TASK FOUND ==========\n");

    printf("Task ID     : %d\n", tasks[index].id);

    printf("Description : %s\n", tasks[index].description);

    printf("Status      : %s\n",
           tasks[index].status == PENDING ? "PENDING" : "DONE");
}

void show_help(void) {

    printf("\n========== AVAILABLE COMMANDS ==========\n");

    printf("ADD      - Add a new task\n");
    printf("DELETE   - Delete an existing task\n");
    printf("VIEW     - Display all tasks\n");
    printf("UPDATE   - Update task description\n");
    printf("COMPLETE - Mark a task as DONE\n");
    printf("FIND     - Search a task using its ID\n");
    printf("HELP     - Display available commands\n");
    printf("EXIT     - Exit the program\n");
}


int main(void) {


    Task *tasks = NULL;

    size_t count = 0;

    size_t capacity = 0;

    char input[50];

    CommandType cmd;

    printf("====================================\n");
    printf("       TASK LIST MANAGER\n");
    printf("====================================\n");

    printf("Type HELP to see available commands.\n");


    while (1) {

        int result = read_line("\nEnter command: ", input, sizeof(input));

        if (result == 0) {
            printf("\nInput ended. Closing program.\n");
            break;
        }

        if (result == -1) {
            printf("Error: Command is too long.\n");
            continue;
        }

        trim(input);

        if (input[0] == '\0') {
            printf("Error: Command cannot be empty.\n");
            continue;
        }

    

        for (size_t i = 0; input[i] != '\0'; i++) {

            input[i] = (char)toupper((unsigned char)input[i]);
        }



        cmd = get_command_type(input);

        switch (cmd) {

            case CMD_ADD:

                add_task(&tasks, &count, &capacity);
                break;


            case CMD_DELETE:

                delete_task(tasks, &count);
                break;


            case CMD_VIEW:

                view_tasks(tasks, count);
                break;


            case CMD_UPDATE:

                update_task(tasks, count);
                break;


            case CMD_COMPLETE:

                complete_task(tasks, count);
                break;


            case CMD_FIND:

                find_task(tasks, count);
                break;


            case CMD_HELP:

                show_help();
                break;


            case CMD_EXIT:

                printf("Exiting Task List Manager.\n");

                free(tasks);

                return 0;


            default:

                printf("Unknown command. Type HELP for instructions.\n");
        }
    }

    free(tasks);

    printf("Memory released successfully.\n");

    return 0;
}

