#ifndef __ITEM__
#define __ITEM__
typedef union Value {
    char symbol;
    double number;
} Value;

typedef enum ValueType {
    OPERATOR, VARIABLE, NUMBER
} ValueType;

typedef struct Item {
    Value value;
    ValueType type;
} Item;

#endif