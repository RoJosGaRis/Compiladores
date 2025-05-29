#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>

typedef enum {
  TYPE_INT,
  TYPE_FLOAT,
  TYPE_BOOL,
  TYPE_STRING,
  TYPE_VOID,
  TYPE_ERROR,
  TYPE_COUNT,
} TYPES;

typedef enum {
    OP_GT,      // 0
    OP_LT,      // 1
    OP_DEQ,     // 2
    OP_EQ,      // 3
    OP_ADD,     // 4
    OP_SUB,     // 5
    OP_MUL,     // 6
    OP_DIV,     // 7
    OP_ERROR,   // 8
    OP_GOTO,    // 9
    OP_GOTOF,   // 10
    OP_GOTOT,   // 11
    OP_PRINT,
    OP_COUNT
} OPERATORS;



TYPES stringToType(const char* typeStr);
OPERATORS stringToOp(char* op);
const char* opToString(OPERATORS op);
const char* typeToString(TYPES type);
TYPES vAddressToType(int vAddress);
bool hasPriority(TYPES op1, TYPES op2); 

#endif
