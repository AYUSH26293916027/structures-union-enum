
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    TYPE_EMPTY,
    INT,
    FLOAT,
    CHAR,
    STRING
} DataType;

typedef union {
    int int_value;
    float float_value;
    char char_value;
    char *string_value;
} Data;

typedef struct {
    Data data;
    DataType type;
} Container;

void container_init(Container *c);

int container_set_int(Container *c, int value);

int container_set_float(Container *c, float value);

int container_set_char(Container *c, char value);

int container_set_string(Container *c, const char *value);

int container_get_int(const Container *c, int *out_value);

int container_get_float(const Container *c, float *out_value);

int container_get_char(const Container *c, char *out_value);

int container_get_string(const Container *c, const char **out_value);

void container_clear(Container *c);

void container_print(const Container *c);

const char *container_type_name(const Container *c);

int container_copy(Container *destination, const Container *source);


void container_init(Container *c) {

    if (c == NULL) {
        return;
    }

    c->type = TYPE_EMPTY;
}


int container_set_int(Container *c, int value) {

    if (c == NULL) {
        return 0;
    }

    container_clear(c);

    c->data.int_value = value;
    c->type = INT;

    return 1;
}

int container_set_float(Container *c, float value) {

    if (c == NULL) {
        return 0;
    }

    container_clear(c);

    c->data.float_value = value;
    c->type = FLOAT;

    return 1;
}


int container_set_char(Container *c, char value) {

    if (c == NULL) {
        return 0;
    }

    container_clear(c);

    c->data.char_value = value;
    c->type = CHAR;

    return 1;
}

int container_set_string(Container *c, const char *value) {

    if (c == NULL || value == NULL) {
        return 0;
    }

    size_t len = strlen(value);
    
    char *new_string = malloc(len + 1);

    if (new_string == NULL) {
        return 0;
    }

   
    memcpy(new_string, value, len + 1);


    container_clear(c);

    c->data.string_value = new_string;
    c->type = STRING;

    return 1;
}


int container_get_int(const Container *c, int *out_value) {

    if (c == NULL || out_value == NULL || c->type != INT) {
        return 0;
    }

    *out_value = c->data.int_value;

    return 1;
}



int container_get_float(const Container *c, float *out_value) {

    if (c == NULL || out_value == NULL || c->type != FLOAT) {
        return 0;
    }

    *out_value = c->data.float_value;

    return 1;
}


int container_get_char(const Container *c, char *out_value) {

    if (c == NULL || out_value == NULL || c->type != CHAR) {
        return 0;
    }

    *out_value = c->data.char_value;

    return 1;
}


int container_get_string(const Container *c, const char **out_value) {

    if (c == NULL || out_value == NULL || c->type != STRING ||
        c->data.string_value == NULL) {
        return 0;
    }

    *out_value = c->data.string_value;

    return 1;
}

const char *container_type_name(const Container *c) {

    if (c == NULL) {
        return "INVALID";
    }

    switch (c->type) {

        case TYPE_EMPTY:
            return "EMPTY";

        case INT:
            return "INTEGER";

        case FLOAT:
            return "FLOAT";

        case CHAR:
            return "CHARACTER";

        case STRING:
            return "STRING";

        default:
            return "INVALID";
    }
}

void container_print(const Container *c) {

    if (c == NULL) {
        printf("Invalid container.\n");
        return;
    }

    switch (c->type) {

        case TYPE_EMPTY:
            printf("Container is empty.\n");
            break;

        case INT:
            printf("Integer: %d\n", c->data.int_value);
            break;

        case FLOAT:
            printf("Float: %.2f\n", c->data.float_value);
            break;

        case CHAR:
            printf("Character: %c\n", c->data.char_value);
            break;

        case STRING:

            if (c->data.string_value != NULL) {
                printf("String: %s\n", c->data.string_value);
            } else {
                printf("Invalid string data.\n");
            }

            break;

        default:
            printf("Invalid data type.\n");
    }
}


void container_clear(Container *c) {

    if (c == NULL) {
        return;
    }

    if (c->type == STRING) {
        free(c->data.string_value);
    }

    c->type = TYPE_EMPTY;
}



int container_copy(Container *destination, const Container *source) {

    if (destination == NULL || source == NULL) {
        return 0;
    }


    if (destination == source) {
        return 1;
    }


    if (source->type == TYPE_EMPTY) {
        container_clear(destination);
        return 1;
    }


    if (source->type == STRING) {

        if (source->data.string_value == NULL) {
            return 0;
        }

        size_t len = strlen(source->data.string_value);

        char *new_string = malloc(len + 1);

        if (new_string == NULL) {
            return 0;
        }

        memcpy(new_string, source->data.string_value, len + 1);

        container_clear(destination);

        destination->data.string_value = new_string;
        destination->type = STRING;

        return 1;
    }

  Data temp = source->data;
    DataType temp_type = source->type;

    if (temp_type != INT && temp_type != FLOAT &&
        temp_type != CHAR) {
        return 0;
    }

    container_clear(destination);

    destination->data = temp;
    destination->type = temp_type;

    return 1;
}

int main(void) {


    Container records[6];


    for (int i = 0; i < 6; i++) {
        container_init(&records[i]);
    }


    printf("========== HETEROGENEOUS DATA CONTAINER ==========\n");

    container_set_int(&records[0], 10);

    container_set_float(&records[1], 3.14f);

    container_set_char(&records[2], 'A');

    container_set_string(&records[3], "Hello");

    container_set_int(&records[4], 0);

    container_set_string(&records[5], "Data");


    printf("\n========== MIXED CONTAINER RECORDS ==========\n");

    for (int i = 0; i < 6; i++) {

        printf("\nRecord Index: %d\n", i);

        printf("Data Type: %s\n",
               container_type_name(&records[i]));

        container_print(&records[i]);
    }


    printf("\n========== DEEP COPY DEMONSTRATION ==========\n");

    Container copied;

    container_init(&copied);

    if (container_copy(&copied, &records[5])) {

        printf("Original: ");
        container_print(&records[5]);

        printf("Copied: ");
        container_print(&copied);


        container_set_string(&records[5], "Modified Data");

        printf("\nAfter changing original:\n");

        printf("Original: ");
        container_print(&records[5]);

        printf("Copied: ");
        container_print(&copied);
    }


    printf("\n========== TYPE TRANSITION ==========\n");

    container_set_string(&records[0], "First String");

    printf("STRING: ");
    container_print(&records[0]);

    container_set_int(&records[0], 100);

    printf("INT: ");
    container_print(&records[0]);

    container_set_string(&records[0], "Second String");

    printf("STRING: ");
    container_print(&records[0]);

    container_set_float(&records[0], 99.5f);

    printf("FLOAT: ");
    container_print(&records[0]);

    printf("\n========== WRONG TYPE TEST ==========\n");

    int number;

    if (!container_get_int(&records[1], &number)) {
        printf("Error: Container does not hold an integer.\n");
    }


    printf("\n========== INTEGER ZERO TEST ==========\n");

    if (container_get_int(&records[4], &number)) {
        printf("Successfully retrieved integer: %d\n", number);
    }

    printf("\n========== EMPTY CONTAINER TEST ==========\n");

    Container empty;

    container_init(&empty);

    container_print(&empty);


    printf("\n========== EMPTY STRING TEST ==========\n");

    if (container_set_string(&empty, "")) {
        printf("Empty string accepted:\n");
        container_print(&empty);
    }


    printf("\n========== SAME STRING POINTER TEST ==========\n");

    const char *same_text;

    if (container_get_string(&records[5], &same_text)) {

        if (container_set_string(&records[5], same_text)) {
            printf("String replaced successfully using its own source.\n");
            container_print(&records[5]);
        }
    }

    printf("\n========== SELF COPY TEST ==========\n");

    if (container_copy(&records[3], &records[3])) {
        printf("Self-copy handled successfully.\n");
    }


    printf("\n========== REPEATED CLEAR TEST ==========\n");

    container_clear(&records[3]);

    container_clear(&records[3]);

    printf("Container cleared safely multiple times.\n");

    container_print(&records[3]);

    printf("\n========== MEMORY CLEANUP ==========\n");

    for (int i = 0; i < 6; i++) {
        container_clear(&records[i]);
    }

    container_clear(&copied);

    container_clear(&empty);

    printf("All containers cleared successfully.\n");

    printf("\nProgram completed successfully.\n");

    return 0;
}
