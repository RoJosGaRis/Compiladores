#include <stdbool.h>
#include "../utils/stack.h"

typedef struct FunctionData {
  char* name;
  int Index;
  char* Firma;
  int Variables;
  int IntVariable;
  int IntTemps;
  int FloatVariables;
  int FloatTemps;
  int BoolTemps;
  int StartQuad;
} FunctionData;

typedef struct LocalContext{
  
  int*    INT_VARIABLES;
  int     INT_VARIABLES_COUNT;
  int*    INT_TEMPS;
  int     INT_TEMPS_COUNT;
  float*  FLOAT_VARIABLES; 
  int     FLOAT_VARIABLES_COUNT;
  float*  FLOAT_TEMPS;
  int     FLOAT_TEMPS_COUNT;
  bool*   BOOL_TEMPS;
  int     BOOL_TEMPS_COUNT;
  struct LocalContext* prev;
  
} LocalContext;

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
  int     QUAD_COUNT;

  Stack   instructionLog;

  int     FUNCTION_COUNT;
  FunctionData* functions;
  LocalContext* currentContext;
  LocalContext* nextContext;
  
} MachineContext;