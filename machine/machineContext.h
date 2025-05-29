#include <stdbool.h>

typedef struct MachineContext{
  // struct FunctionEntry * functionTable;
  // struct FunctionEntry * currentFunction;
  // struct FunctionEntry * programFunction;
  
  struct QuadList * quadList;
  
  int*    INT_VARIABLES;
  int*    INT_TEMPS;
  float*  FLOAT_VARIABLES; 
  float*  FLOAT_TEMPS;
  int     INT_CONSTANTS_COUNT;
  int*    INT_CONSTANTS;
  int     FLOAT_CONSTANTS_COUNT;
  float*  FLOAT_CONSTANTS;
  int     STRING_CONSTANTS_COUNT;
  char**  STRING_CONSTANTS;
  int     INT_VARIABLES_COUNT;
  int     INT_TEMPS_COUNT;
  int     FLOAT_VARIABLES_COUNT;
  int     FLOAT_TEMPS_COUNT;
  bool*   BOOL_TEMPS;
  int     BOOL_TEMPS_COUNT;
  
} MachineContext;