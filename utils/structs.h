#ifndef STRUCTS_H
#define STRUCTS_H

#include "stack.h"
#include "types.h"

typedef struct ParserContext{
  struct FunctionEntry * functionTable;
  struct FunctionEntry * currentFunction;
  struct FunctionEntry * programFunction;
  
  struct QuadList * quadList;
  Stack operations;
  Stack operators;
  Stack goToAddresses;
  
  // int*    INT_VARIABLES;
  // int*    INT_TEMPS;
  // float*  FLOAT_VARIABLES; 
  // float*  FLOAT_TEMPS;
  // char**  STRING_TEMPS;
  int*    INT_CONSTANTS;
  float*  FLOAT_CONSTANTS;
  char**  STRING_CONSTANTS;
  int     TEMP_COUNT;
  int     INT_VARIABLES_COUNT;
  int     INT_CONSTANTS_COUNT;
  int     INT_TEMPS_COUNT;
  int     FLOAT_VARIABLES_COUNT;
  int     FLOAT_CONSTANTS_COUNT;
  int     FLOAT_TEMPS_COUNT;
  int     BOOL_TEMPS_COUNT;
  int     STRING_CONSTANTS_COUNT;

  char**  FUNCTION_ID_LIST;
  int     FUNCTION_COUNT;
  int    FUNCTION_CALL_PARAMETER_COUNT;
  
} ParserContext;


typedef struct Quadruple{
  OPERATORS op;
  int addLeft;
  int addRight;
  int addRes;
} Quadruple;

typedef struct QuadList {
  Quadruple * quadruples;
  int count;
} QuadList;

#endif