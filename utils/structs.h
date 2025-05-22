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
  
  int*    INT_VARIABLES;
  int*    INT_CONSTANTS;
  int*    INT_TEMPS;
  float*  FLOAT_VARIABLES; 
  float*  FLOAT_CONSTANTS;
  float*  FLOAT_TEMPS;
  char**  STRING_CONSTANTS;
  char**  STRING_TEMPS;
  int     TEMP_COUNT;
  int     INT_VARIABLES_COUNT;
  int     INT_CONSTANTS_COUNT;
  int     INT_TEMPS_COUNT;
  int     FLOAT_VARIABLES_COUNT;
  int     FLOAT_CONSTANTS_COUNT;
  int     FLOAT_TEMPS_COUNT;
  int     STRING_CONSTANTS_COUNT;
  
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