// Structures - 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <limits.h>
#include <stdint.h>

typedef struct {
    int id;
    float width;
    float height;
    float area;
    float perimeter;
} Rectangle;

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

    if (read_line(input, sizeof(input)) != 1) {
        return 0;
    }

    errno = 0;

    long number = strtol(input, &end, 10);

    if (input == end || *end != '\0' ||
        errno == ERANGE ||
        number < INT_MIN || number > INT_MAX) {

        return 0;
    }

    *value = (int)number;

    return 1;
}

int read_dimension(const char *message, float *value) {

    char input[100];
    char *end;

    printf("%s", message);

    if (read_line(input, sizeof(input)) != 1) {
        return 0;
    }

    errno = 0;

    float number = strtof(input, &end);

    if (input == end || *end != '\0' ||
        errno == ERANGE ||
        !isfinite(number) || number <= 0.0f) {

        return 0;
    }

    *value = number;

    return 1;
}

int read_threshold(float *value) {

    char input[100];
    char *end;

    printf("Enter area threshold: ");

    if (read_line(input, sizeof(input)) != 1) {
        return 0;
    }

    errno = 0;

    float number = strtof(input, &end);

    if (input == end || *end != '\0' ||
        errno == ERANGE ||
        !isfinite(number) || number < 0.0f) {

        return 0;
    }

    *value = number;

    return 1;
}

void calculate_area(Rectangle *r) {

    r->area = r->width * r->height;

    r->perimeter = 2.0f * (r->width + r->height);
}

int valid_geometry(float width, float height) {

    float area = width * height;

    float perimeter = 2.0f * (width + height);

    return isfinite(area) && isfinite(perimeter);
}


Rectangle *create_rectangle_array(size_t capacity) {

    if (capacity == 0 ||
        capacity > SIZE_MAX / sizeof(Rectangle)) {

        return NULL;
    }

    Rectangle *rectangles =
        malloc(capacity * sizeof(Rectangle));

    return rectangles;
}

int resize_rectangle_array(
    Rectangle **rectangles,
    size_t *capacity
) {

    
    if (*capacity >= 1000) {
        return 0;
    }

    size_t new_capacity = *capacity * 2;

    if (new_capacity > 1000) {
        new_capacity = 1000;
    }

    if (new_capacity > SIZE_MAX / sizeof(Rectangle)) {
        return 0;
    }

    Rectangle *temp = realloc(
        *rectangles,
        new_capacity * sizeof(Rectangle)
    );

    if (temp == NULL) {
        return 0;
    }

    *rectangles = temp;

    *capacity = new_capacity;

    return 1;
}


void free_rectangle_array(Rectangle *rectangles) {

    free(rectangles);
}

int find_rectangle(
    Rectangle *rectangles,
    size_t count,
    int id
) {

    for (size_t i = 0; i < count; i++) {

        if (rectangles[i].id == id) {
            return (int)i;
        }
    }

    return -1;
}


void add_rectangle(
    Rectangle **rectangles,
    size_t *count,
    size_t *capacity
) {

    int id;

    float width;
    float height;

    printf("\n========== ADD RECTANGLE ==========\n");

    if (!read_integer("Enter rectangle ID: ", &id) ||
        id <= 0) {

        printf("Error: ID must be a positive integer.\n");
        return;
    }

   
    if (find_rectangle(*rectangles, *count, id) != -1) {

        printf("Error: Duplicate rectangle ID.\n");
        return;
    }

    if (!read_dimension("Enter width: ", &width)) {

        printf("Error: Width must be positive and finite.\n");
        return;
    }

    if (!read_dimension("Enter height: ", &height)) {

        printf("Error: Height must be positive and finite.\n");
        return;
    }

    if (!valid_geometry(width, height)) {

        printf("Error: Dimensions produce invalid geometry.\n");
        return;
    }


    if (*count == *capacity) {

        if (!resize_rectangle_array(rectangles, capacity)) {

            printf("Error: Unable to expand memory.\n");
            return;
        }

        printf("Memory expanded successfully.\n");
        printf("New capacity: %zu\n", *capacity);
    }

    (*rectangles)[*count].id = id;

    (*rectangles)[*count].width = width;

    (*rectangles)[*count].height = height;

    calculate_area(&(*rectangles)[*count]);

    (*count)++;

    printf("Rectangle added successfully!\n");
}

#define TOLERANCE 0.0001f

void display_rectangles(Rectangle *rectangles, size_t count) {

    printf("\n========== RECTANGLE RECORDS ==========\n");

    if (count == 0) {

        printf("No rectangles available.\n");
        return;
    }

    printf("%-8s %-10s %-10s %-12s %-12s %-12s\n",
           "ID", "Width", "Height", "Area",
           "Perimeter", "Type");

    printf("--------------------------------------------------------------------------\n");

    for (size_t i = 0; i < count; i++) {

        char *type;

        if (fabsf(rectangles[i].width -
                  rectangles[i].height) <= TOLERANCE) {

            type = "Square";
        }
        else {

            type = "Non-Square";
        }

        printf("%-8d %-10.2f %-10.2f %-12.2f %-12.2f %-12s\n",
               rectangles[i].id,
               rectangles[i].width,
               rectangles[i].height,
               rectangles[i].area,
               rectangles[i].perimeter,
               type);
    }
}


void search_rectangle(Rectangle *rectangles, size_t count) {

    int id;

    printf("\n========== SEARCH RECTANGLE ==========\n");

    if (count == 0) {

        printf("No rectangles available to search.\n");
        return;
    }

    if (!read_integer("Enter rectangle ID: ", &id)) {

        printf("Invalid ID.\n");
        return;
    }

    int index = find_rectangle(rectangles, count, id);

    if (index == -1) {

        printf("Rectangle not found.\n");
        return;
    }

    printf("\nRectangle Found!\n");

    printf("ID: %d\n", rectangles[index].id);

    printf("Width: %.2f\n", rectangles[index].width);

    printf("Height: %.2f\n", rectangles[index].height);

    printf("Area: %.2f\n", rectangles[index].area);

    printf("Perimeter: %.2f\n", rectangles[index].perimeter);
}


void update_rectangle(Rectangle *rectangles, size_t count) {

    int id;

    float width;
    float height;

    printf("\n========== UPDATE RECTANGLE ==========\n");

    if (count == 0) {

        printf("No rectangles available to update.\n");
        return;
    }

    if (!read_integer("Enter rectangle ID: ", &id)) {

        printf("Invalid ID.\n");
        return;
    }

    int index = find_rectangle(rectangles, count, id);

    if (index == -1) {

        printf("Rectangle not found.\n");
        return;
    }

    if (!read_dimension("Enter new width: ", &width)) {

        printf("Invalid width.\n");
        return;
    }

    if (!read_dimension("Enter new height: ", &height)) {

        printf("Invalid height.\n");
        return;
    }

    if (!valid_geometry(width, height)) {

        printf("Error: Dimensions produce invalid geometry.\n");
        return;
    }

    rectangles[index].width = width;

    rectangles[index].height = height;


    calculate_area(&rectangles[index]);

    printf("Rectangle updated successfully!\n");
}


void delete_rectangle(Rectangle *rectangles, size_t *count) {

    int id;

    printf("\n========== DELETE RECTANGLE ==========\n");

    if (*count == 0) {

        printf("No rectangles available to delete.\n");
        return;
    }

    if (!read_integer("Enter rectangle ID: ", &id)) {

        printf("Invalid ID.\n");
        return;
    }

    int index = find_rectangle(rectangles, *count, id);

    if (index == -1) {

        printf("Rectangle not found.\n");
        return;
    }

    for (size_t i = (size_t)index; i < *count - 1; i++) {

        rectangles[i] = rectangles[i + 1];
    }

    (*count)--;

    printf("Rectangle deleted successfully!\n");
}


void rectangle_analysis(Rectangle *rectangles, size_t count) {

    printf("\n========== GEOMETRY ANALYSIS ==========\n");

    if (count == 0) {

        printf("No rectangles available for analysis.\n");
        return;
    }

    size_t largest = 0;

    size_t smallest = 0;

    double total_area = 0.0;

    for (size_t i = 0; i < count; i++) {

        total_area += rectangles[i].area;

        if (rectangles[i].area > rectangles[largest].area) {

            largest = i;
        }

        if (rectangles[i].area < rectangles[smallest].area) {

            smallest = i;
        }
    }

    double average_area = total_area / count;

    printf("\nLargest Rectangle:\n");

    printf("ID: %d\n", rectangles[largest].id);

    printf("Area: %.2f\n", rectangles[largest].area);

    printf("\nSmallest Rectangle:\n");

    printf("ID: %d\n", rectangles[smallest].id);

    printf("Area: %.2f\n", rectangles[smallest].area);

    printf("\nTotal Area: %.2f\n", total_area);

    printf("Average Area: %.2f\n", average_area);

    printf("Total Rectangles: %zu\n", count);
}

void sort_rectangles(Rectangle *rectangles, size_t count) {

    if (count == 0) {

        printf("No rectangles available to sort.\n");
        return;
    }

    for (size_t i = 0; i < count - 1; i++) {

        for (size_t j = 0; j < count - i - 1; j++) {

            if (rectangles[j].area > rectangles[j + 1].area ||
                (rectangles[j].area == rectangles[j + 1].area &&
                 rectangles[j].id > rectangles[j + 1].id)) {

                Rectangle temp = rectangles[j];

                rectangles[j] = rectangles[j + 1];

                rectangles[j + 1] = temp;
            }
        }
    }

    printf("\nRectangles sorted successfully!\n");

    display_rectangles(rectangles, count);
}

void filter_rectangles(Rectangle *rectangles, size_t count) {

    float threshold;

    printf("\n========== AREA FILTER ==========\n");

    if (count == 0) {

        printf("No rectangles available to filter.\n");
        return;
    }

    if (!read_threshold(&threshold)) {

        printf("Invalid threshold.\n");
        return;
    }

    int found = 0;

    printf("\nRectangles with area greater than %.2f:\n",
           threshold);

    for (size_t i = 0; i < count; i++) {

        if (rectangles[i].area > threshold) {

            printf("ID: %d | Width: %.2f | Height: %.2f | Area: %.2f\n",
                   rectangles[i].id,
                   rectangles[i].width,
                   rectangles[i].height,
                   rectangles[i].area);

            found = 1;
        }
    }

    if (!found) {

        printf("No rectangles found above this area.\n");
    }
}



int main(void) {

    int initial_size;

    size_t count;

    size_t capacity;

    printf("============================================\n");
    printf("     RECTANGLE INVENTORY AND ANALYSIS\n");
    printf("============================================\n");

    // Read initial number of rectangles
    if (!read_integer("Enter initial number of rectangles (1-1000): ",
                      &initial_size) ||
        initial_size < 1 || initial_size > 1000) {

        printf("Error: Enter a number between 1 and 1000.\n");

        return 1;
    }

    capacity = (size_t)initial_size;

    count = 0;


    Rectangle *rectangles =
        create_rectangle_array(capacity);

    if (rectangles == NULL) {

        printf("Error: Memory allocation failed.\n");

        return 1;
    }

    printf("\nMemory allocated successfully!\n");


    for (int i = 0; i < initial_size; i++) {

        printf("\nEnter details for Rectangle %d:\n", i + 1);

        add_rectangle(&rectangles, &count, &capacity);
    }

    int choice;


    while (1) {

        printf("\n============================================\n");
        printf("                MAIN MENU\n");
        printf("============================================\n");

        printf("1. Add Rectangle\n");
        printf("2. Display All Rectangles\n");
        printf("3. Search Rectangle\n");
        printf("4. Update Rectangle\n");
        printf("5. Delete Rectangle\n");
        printf("6. Geometry Analysis\n");
        printf("7. Sort by Area\n");
        printf("8. Filter by Area\n");
        printf("9. Exit\n");

        printf("============================================\n");

        if (!read_integer("Enter your choice: ", &choice)) {

            if (feof(stdin)) {
                printf("\nInput ended. Exiting program.\n");
                break;
            }

            printf("Invalid menu input.\n");
            continue;
        }

        switch (choice) {

            case 1:
                add_rectangle(&rectangles, &count, &capacity);
                break;

            case 2:
                display_rectangles(rectangles, count);
                break;

            case 3:
                search_rectangle(rectangles, count);
                break;

            case 4:
                update_rectangle(rectangles, count);
                break;

            case 5:
                delete_rectangle(rectangles, &count);
                break;

            case 6:
                rectangle_analysis(rectangles, count);
                break;

            case 7:
                sort_rectangles(rectangles, count);
                break;

            case 8:
                filter_rectangles(rectangles, count);
                break;

            case 9:
                printf("\nExiting Rectangle System...\n");
                goto cleanup;

            default:
                printf("Invalid choice! Select between 1 and 9.\n");
        }
    }

cleanup:

    free_rectangle_array(rectangles);

    printf("Memory released successfully.\n");

    printf("Program terminated successfully.\n");

    return 0;
}
