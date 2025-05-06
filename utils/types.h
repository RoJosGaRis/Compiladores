#ifndef TYPES_H
#define TYPES_H

typedef enum {
  TYPE_INT,
  TYPE_FLOAT,
  TYPE_STRING,
  TYPE_VOID,
  TYPE_BOOL,
  TYPE_UNKNOWN,
} TYPES;

TYPES stringToType(const char* typeStr);
const char* typeToString(TYPES type);

#endif