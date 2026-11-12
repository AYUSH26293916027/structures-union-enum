// Unions

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>


typedef union {
    int i;
    float f;
    char str[20];
} UnionData;

typedef struct {
    int i;
    float f;
    char str[20];
} StructData;

    VALUE_INT,
    VALUE_FLOAT,
    VALUE_TEXT
} ValueType;

typedef struct {
    UnionData data;
    ValueType type;
} TaggedRecord;

int set_int(TaggedRecord *record, int value) {
    if (record == NULL) {
        return 0;
    }

    record->data.i = value;
    record->type = VALUE_INT;

    return 1;
}


int set_float(TaggedRecord *record, float value) {
    if (record == NULL) {
        return 0;
    }

    record->data.f = value;
    record->type = VALUE_FLOAT;

    return 1;
}

int set_text(TaggedRecord *record, const char *text) {
    if (record == NULL || text == NULL) {
        return 0;
    }

    if (strlen(text) >= sizeof(record->data.str)) {
        return 0;
    }

    strcpy(record->data.str, text);
    record->type = VALUE_TEXT;

    return 1;
}

void display_record(const TaggedRecord *record) {

    if (record == NULL) {
        printf("Invalid record!\n");
        return;
    }

    switch (record->type) {

        case VALUE_INT:
            printf("Integer: %d\n", record->data.i);
            break;

        case VALUE_FLOAT:
            printf("Float: %.2f\n", record->data.f);
            break;

        case VALUE_TEXT:
            printf("Text: %s\n", record->data.str);
            break;

        default:
            printf("Invalid value type!\n");
    }
}


int main(void) {

    UnionData udata;
    StructData sdata;

    sdata.i = 10;
    sdata.f = 220.5f;
    strcpy(sdata.str, "Struct Programming");

    printf("========== UNION DEMONSTRATION ==========\n");

    udata.i = 10;
    printf("Integer: %d\n", udata.i);

    strcpy(udata.str, "Union Programming");
    printf("String: %s\n", udata.str);

    udata.f = 220.5f;
    printf("Float: %.2f\n", udata.f);

    printf("\n========== UNION MEMORY ADDRESSES ==========\n");

    printf("Address of udata.i   : %p\n", (void *)&udata.i);
    printf("Address of udata.f   : %p\n", (void *)&udata.f);
    printf("Address of udata.str : %p\n", (void *)&udata.str);

    printf("\n========== STRUCTURE MEMORY ADDRESSES ==========\n");

    printf("Address of sdata.i   : %p\n", (void *)&sdata.i);
    printf("Address of sdata.f   : %p\n", (void *)&sdata.f);
    printf("Address of sdata.str : %p\n", (void *)&sdata.str);

    printf("\n========== STRUCTURE MEMBER OFFSETS ==========\n");

    printf("Offset of i   : %zu bytes\n", offsetof(StructData, i));
    printf("Offset of f   : %zu bytes\n", offsetof(StructData, f));
    printf("Offset of str : %zu bytes\n", offsetof(StructData, str));

    printf("\n========== SIZE AND ALIGNMENT ==========\n");

    printf("Size of UnionData     : %zu bytes\n", sizeof(UnionData));
    printf("Size of StructData    : %zu bytes\n", sizeof(StructData));

    printf("Alignment of UnionData  : %zu bytes\n", _Alignof(UnionData));
    printf("Alignment of StructData : %zu bytes\n", _Alignof(StructData));

    printf("\n========== MEMORY FOR 100 RECORDS ==========\n");

    printf("100 Union records     : %zu bytes\n",
           100 * sizeof(UnionData));

    printf("100 Structure records : %zu bytes\n",
           100 * sizeof(StructData));

    printf("\n========== TAGGED RECORD SYSTEM ==========\n");

    TaggedRecord records[5];

    set_int(&records[0], 10);
    set_float(&records[1], 220.5f);
    set_text(&records[2], "Union Programming");
    set_int(&records[3], 500);
    set_text(&records[4], "C Programming");

    for (int i = 0; i < 5; i++) {

        printf("\nRecord %d: ", i + 1);
        display_record(&records[i]);
    }

    printf("\n========== EDGE CASE TESTING ==========\n");

    TaggedRecord test_record;

    if (set_text(&test_record, "1234567890123456789")) {
        printf("19-character string accepted: ");
        display_record(&test_record);
    }


    if (!set_text(&test_record, "12345678901234567890")) {
        printf("20-character string rejected successfully.\n");
    }

    printf("\n========== TYPE SWITCHING TEST ==========\n");

    set_int(&test_record, 100);
    printf("After integer assignment: ");
    display_record(&test_record);

    set_text(&test_record, "Hello Union");
    printf("After text assignment: ");
    display_record(&test_record);

    set_float(&test_record, 99.5f);
    printf("After float assignment: ");
    display_record(&test_record);

    printf("\n========== INVALID TAG TEST ==========\n");

    test_record.type = (ValueType)99;
    display_record(&test_record);


    printf("\n========== FINAL SIZE COMPARISON ==========\n");

    printf("Size of UnionData    : %zu bytes\n", sizeof(UnionData));
    printf("Size of StructData   : %zu bytes\n", sizeof(StructData));
    printf("Size of TaggedRecord : %zu bytes\n", sizeof(TaggedRecord));


    printf("\nProgram completed successfully.\n");

    return 0;
}
