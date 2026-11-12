// Structures - 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <math.h>

typedef struct {
    int roll_number;
    char name[50];
    int age;
    float gpa;
} Student;

int read_line(char *buffer, size_t size) {
    if (fgets(buffer, size, stdin) == NULL) {
        return 0;
    }

    size_t len = strlen(buffer);

    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        return 1;
    }

    if (!feof(stdin)) {
        int ch;

        while ((ch = getchar()) != '\n' && ch != EOF) {
        }

        return -1;
    }

    return 1;
}



int read_integer(const char *message, int *value) {
    char input[100];
    char *end;

    printf("%s", message);

    int status = read_line(input, sizeof(input));

    if (status != 1) {
        return 0;
    }

    errno = 0;
    long number = strtol(input, &end, 10);

    if (input == end || *end != '\0' ||
        errno == ERANGE || number < INT_MIN ||
        number > INT_MAX) {
        return 0;
    }

    *value = (int)number;
    return 1;
}



int read_gpa(const char *message, float *value) {
    char input[100];
    char *end;

    printf("%s", message);

    int status = read_line(input, sizeof(input));

    if (status != 1) {
        return 0;
    }

    errno = 0;
    float number = strtof(input, &end);

    if (input == end || *end != '\0' ||
        errno == ERANGE || !isfinite(number) ||
        number < 0.0f || number > 4.0f) {
        return 0;
    }

    *value = number;
    return 1;
}

int read_name(char name[50]) {
    char input[100];

    printf("Enter student name: ");

    int status = read_line(input, sizeof(input));

    if (status != 1) {
        return 0;
    }

    size_t len = strlen(input);

    if (len == 0 || len > 49) {
        return 0;
    }

    strcpy(name, input);
    return 1;
}


Student *create_student_list(size_t capacity) {

    if (capacity == 0 ||
        capacity > SIZE_MAX / sizeof(Student)) {
        return NULL;
    }

    Student *students = malloc(capacity * sizeof(Student));

    return students;
}

int resize_student_list(Student **students, size_t *capacity) {

    if (*capacity == 0 ||
        *capacity > SIZE_MAX / 2) {
        return 0;
    }

    size_t new_capacity = (*capacity) * 2;

    if (new_capacity > SIZE_MAX / sizeof(Student)) {
        return 0;
    }

    Student *temp = realloc(
        *students,
        new_capacity * sizeof(Student)
    );

    if (temp == NULL) {
        return 0;
    }

    *students = temp;
    *capacity = new_capacity;

    return 1;
}

void free_student_list(Student *students) {
    free(students);
}

int find_student(Student *students, size_t count, int roll_number) {

    for (size_t i = 0; i < count; i++) {

        if (students[i].roll_number == roll_number) {
            return (int)i;
        }
    }

    return -1;
}

void add_student(
    Student **students,
    size_t *count,
    size_t *capacity
) {

    int roll_number;
    int age;
    float gpa;
    char name[50];

    printf("\n========== ADD STUDENT ==========\n");

    if (!read_integer("Enter roll number: ", &roll_number) ||
        roll_number <= 0) {

        printf("Error: Roll number must be a positive integer.\n");
        return;
    }

    if (find_student(*students, *count, roll_number) != -1) {
        printf("Error: Duplicate roll number is not allowed.\n");
        return;
    }

    if (!read_name(name)) {
        printf("Error: Name must contain 1 to 49 characters.\n");
        return;
    }

    if (!read_integer("Enter age (16-100): ", &age) ||
        age < 16 || age > 100) {

        printf("Error: Age must be between 16 and 100.\n");
        return;
    }

    if (!read_gpa("Enter GPA (0.00-4.00): ", &gpa)) {

        printf("Error: GPA must be between 0.00 and 4.00.\n");
        return;
    }

    if (*count == *capacity) {

        if (!resize_student_list(students, capacity)) {
            printf("Error: Unable to allocate more memory.\n");
            return;
        }

        printf("Memory expanded successfully. New capacity: %zu\n",
               *capacity);
    }

    (*students)[*count].roll_number = roll_number;

    strcpy((*students)[*count].name, name);

    (*students)[*count].age = age;

    (*students)[*count].gpa = gpa;

    (*count)++;

    printf("\nStudent added successfully!\n");
}
void display_students(Student *students, size_t count) {

    printf("\n========== STUDENT RECORDS ==========\n");

    if (count == 0) {
        printf("No student records available.\n");
        return;
    }

    printf("%-12s %-20s %-8s %-8s\n",
           "Roll No.", "Name", "Age", "GPA");

    printf("--------------------------------------------------\n");

    for (size_t i = 0; i < count; i++) {

        printf("%-12d %-20s %-8d %-8.2f\n",
               students[i].roll_number,
               students[i].name,
               students[i].age,
               students[i].gpa);
    }
}


void search_student(Student *students, size_t count) {

    int roll_number;

    printf("\n========== SEARCH STUDENT ==========\n");

    if (count == 0) {
        printf("No records available to search.\n");
        return;
    }

    if (!read_integer("Enter roll number to search: ",
                     &roll_number)) {

        printf("Error: Invalid roll number.\n");
        return;
    }

    int index = find_student(students, count, roll_number);

    if (index == -1) {
        printf("Student not found.\n");
        return;
    }

    printf("\nStudent Found!\n");

    printf("Roll Number: %d\n", students[index].roll_number);
    printf("Name: %s\n", students[index].name);
    printf("Age: %d\n", students[index].age);
    printf("GPA: %.2f\n", students[index].gpa);
}

void update_student(Student *students, size_t count) {

    int roll_number;

    printf("\n========== UPDATE STUDENT ==========\n");

    if (count == 0) {
        printf("No records available to update.\n");
        return;
    }

    if (!read_integer("Enter roll number to update: ",
                     &roll_number)) {

        printf("Error: Invalid roll number.\n");
        return;
    }

    int index = find_student(students, count, roll_number);

    if (index == -1) {
        printf("Student not found.\n");
        return;
    }

    printf("\nEnter new details:\n");

    char name[50];
    int age;
    float gpa;

    if (!read_name(name)) {
        printf("Error: Invalid name.\n");
        return;
    }

    if (!read_integer("Enter new age (16-100): ", &age) ||
        age < 16 || age > 100) {

        printf("Error: Invalid age.\n");
        return;
    }

    if (!read_gpa("Enter new GPA (0.00-4.00): ", &gpa)) {

        printf("Error: Invalid GPA.\n");
        return;
    }

    strcpy(students[index].name, name);
    students[index].age = age;
    students[index].gpa = gpa;

    printf("Student record updated successfully!\n");
}


void delete_student(Student *students, size_t *count) {

    int roll_number;

    printf("\n========== DELETE STUDENT ==========\n");

    if (*count == 0) {
        printf("No records available to delete.\n");
        return;
    }

    if (!read_integer("Enter roll number to delete: ",
                     &roll_number)) {

        printf("Error: Invalid roll number.\n");
        return;
    }

    int index = find_student(students, *count, roll_number);

    if (index == -1) {
        printf("Student not found.\n");
        return;
    }

    
    for (size_t i = (size_t)index; i < *count - 1; i++) {

        students[i] = students[i + 1];
    }

    (*count)--;

    printf("Student record deleted successfully!\n");
}


void sort_students(Student *students, size_t count) {

    if (count == 0) {
        printf("No records available to sort.\n");
        return;
    }

    for (size_t i = 0; i < count - 1; i++) {

        for (size_t j = 0; j < count - i - 1; j++) {

            if (students[j].gpa < students[j + 1].gpa ||
                (students[j].gpa == students[j + 1].gpa &&
                 students[j].roll_number >
                 students[j + 1].roll_number)) {

                Student temp = students[j];

                students[j] = students[j + 1];

                students[j + 1] = temp;
            }
        }
    }

    printf("\nStudents sorted successfully!\n");

    display_students(students, count);
}

void display_gpa_summary(Student *students, size_t count) {

    printf("\n========== ACADEMIC ANALYTICS ==========\n");

    if (count == 0) {
        printf("No student records available for analysis.\n");
        return;
    }

    size_t highest = 0;
    size_t lowest = 0;

    double total_gpa = 0.0;

    for (size_t i = 0; i < count; i++) {

        total_gpa += students[i].gpa;

        if (students[i].gpa > students[highest].gpa) {
            highest = i;
        }

        if (students[i].gpa < students[lowest].gpa) {
            lowest = i;
        }
    }

    double average_gpa = total_gpa / count;

    printf("\nHighest GPA Student:\n");

    printf("Roll Number: %d\n", students[highest].roll_number);
    printf("Name: %s\n", students[highest].name);
    printf("GPA: %.2f\n", students[highest].gpa);

    printf("\nLowest GPA Student:\n");

    printf("Roll Number: %d\n", students[lowest].roll_number);
    printf("Name: %s\n", students[lowest].name);
    printf("GPA: %.2f\n", students[lowest].gpa);

    printf("\nAverage GPA: %.2f\n", average_gpa);

    printf("Total Students: %zu\n", count);
}
int main(void) {

    size_t number_of_students;
    size_t capacity;

    int initial_capacity;

    printf("============================================\n");
    printf("   STUDENT RECORDS AND ACADEMIC ANALYTICS\n");
    printf("============================================\n");

    if (!read_integer("Enter initial student capacity: ",
                     &initial_capacity) ||
        initial_capacity <= 0) {

        printf("Error: Capacity must be a positive integer.\n");
        return 1;
    }

    capacity = (size_t)initial_capacity;

    number_of_students = 0;

    Student *student_list = create_student_list(capacity);

    if (student_list == NULL) {
        printf("Error: Initial memory allocation failed.\n");
        return 1;
    }

    printf("\nMemory allocated successfully!\n");

    int choice;

    while (1) {

        printf("\n============================================\n");
        printf("               MAIN MENU\n");
        printf("============================================\n");

        printf("1. Add Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Sort Students by GPA\n");
        printf("7. Display GPA Summary\n");
        printf("8. Exit\n");

        printf("============================================\n");

        if (!read_integer("Enter your choice: ", &choice)) {

            if (feof(stdin)) {
                printf("\nInput ended. Exiting program.\n");
                break;
            }

            printf("Error: Please enter a valid menu choice.\n");
            continue;
        }

        switch (choice) {

            case 1:
                add_student(
                    &student_list,
                    &number_of_students,
                    &capacity
                );
                break;

            case 2:
                display_students(
                    student_list,
                    number_of_students
                );
                break;

            case 3:
                search_student(
                    student_list,
                    number_of_students
                );
                break;

            case 4:
                update_student(
                    student_list,
                    number_of_students
                );
                break;

            case 5:
                delete_student(
                    student_list,
                    &number_of_students
                );
                break;

            case 6:
                sort_students(
                    student_list,
                    number_of_students
                );
                break;

            case 7:
                display_gpa_summary(
                    student_list,
                    number_of_students
                );
                break;

            case 8:
                printf("\nExiting Student Records System...\n");
                goto cleanup;

            default:
                printf("Invalid choice! Please select 1-8.\n");
        }
    }

cleanup:
    free_student_list(student_list);

    printf("Memory released successfully.\n");
    printf("Program terminated successfully.\n");

    return 0;
}
