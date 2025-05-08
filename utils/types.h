#ifndef TYPES_H
#define TYPES_H

typedef enum {
  TYPE_INT,
  TYPE_FLOAT,
  TYPE_STRING,
  TYPE_VOID,
  TYPE_BOOL,
  TYPE_ERROR,
  TYPE_COUNT,
} TYPES;

typedef enum {
    OP_ADD,     // +
    OP_SUB,     // -
    OP_MUL,     // *
    OP_DIV,     // /
    OP_EQ,
    OP_DEQ,     // ==
    OP_GT,      // >
    OP_LT,      // <
    OP_COUNT
} OPERATORS;

TYPES stringToType(const char* typeStr);
const char* typeToString(TYPES type);

#endif
