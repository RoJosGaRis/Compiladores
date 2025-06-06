#ifndef TABLES_H
#define TABLES_H

#include "structs.h"
#include "uthash.h"

typedef struct StringList {
  char *id;
  struct StringList *next;
} StringList;

typedef struct ParamList {
  char * id;
  char * type;
  struct ParamList * next;
} ParamList;

typedef struct VariableEntry {
  char * name;
  TYPES type;
  int VAddress;
  UT_hash_handle hh;
} VariableEntry;

typedef struct FunctionEntry {
  char * name;
  TYPES type;
  char * signature;
  int INT_VARIABLES_COUNT;
  int INT_TEMPS_COUNT;
  int FLOAT_VARIABLES_COUNT;
  int FLOAT_TEMPS_COUNT;
  int BOOL_TEMPS_COUNT;
  int PARAM_COUNT;
  int startQuad;
  int index;

  struct VariableEntry *variableTable;

  UT_hash_handle hh;
} FunctionEntry;

#endif