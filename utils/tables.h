#include "structs.h"

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
  struct VariableEntry *variableTable;

  UT_hash_handle hh;
} FunctionEntry;