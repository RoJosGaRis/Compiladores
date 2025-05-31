#include "parserContext.h"

void initContext(ParserContext * ctx)
{
  sInitialize(&ctx->operations);
  sInitialize(&ctx->operators);

  ctx->quadList = malloc(sizeof(QuadList));
  ctx->quadList->quadruples = NULL;
  ctx->quadList->count = 0;
  
  ctx->FUNCTION_COUNT = 0;
  ctx->FUNCTION_ID_LIST = (char**)malloc(sizeof(char*));
}

void programStart(char* id, ParserContext * ctx){
  addFunction(&ctx->functionTable, id, "void");
  ctx->programFunction = findFunction(&ctx->functionTable, id);
  // printf("Found program function\n");
  ctx->currentFunction = NULL;
  initContext(ctx);
  ctx->FUNCTION_COUNT++;
  addQuad(OP_GOTO, -1, 0, -1, ctx);
}

void foundMain(ParserContext * ctx){
  ctx->programFunction->startQuad = ctx->quadList->count;
  ctx->quadList->quadruples[0].addRight = ctx->quadList->count;
}

void functionStart(char * id, ParserContext * ctx){
  addFunction(&ctx->functionTable, id, "void");
  // printf("Added function: %s\n", id);
  FunctionEntry * function = findFunction(&ctx->functionTable, id);
  // printf("Found function: %s\n", function->name);
  ctx->currentFunction = function;
  ctx->FUNCTION_COUNT++;
  ctx->FUNCTION_ID_LIST = realloc(ctx->FUNCTION_ID_LIST, sizeof(char*) * ctx->FUNCTION_COUNT);
  if (!ctx->FUNCTION_ID_LIST) {
    fprintf(stderr, "Error reallocating FUNCTION_ID_LIST\n");
    exit(EXIT_FAILURE);
  }
  ctx->FUNCTION_ID_LIST[ctx->FUNCTION_COUNT - 1] = id;
  function->index = ctx->FUNCTION_COUNT-1;
  function->startQuad = ctx->quadList->count;
}

void functionEnd(ParserContext * ctx){
  addQuad(OP_ENDFUNC, -1, -1, -1, ctx);
  ctx->currentFunction = NULL;
}

void functionCallStart(char*id, ParserContext * ctx){
  FunctionEntry * function = (FunctionEntry*)malloc(sizeof(FunctionEntry));
  HASH_FIND_STR(ctx->functionTable, id, function);
  addQuad(OP_ERA, -1, function->index, -1, ctx);
}

void functionParameterAdded(char* exp, ParserContext * ctx){
  int value = sPop(&ctx->operators);
  addQuad(OP_PARAM, -1, value, ctx->FUNCTION_CALL_PARAMETER_COUNT++,ctx);
}

void functionCallEnd(char* id, ParserContext * ctx){
  FunctionEntry * function = (FunctionEntry*)malloc(sizeof(FunctionEntry));
  HASH_FIND_STR(ctx->functionTable, id, function);
  if(ctx->FUNCTION_CALL_PARAMETER_COUNT < function->PARAM_COUNT){
    printf("Missing %d parameters for function call to %s", function->PARAM_COUNT - ctx->FUNCTION_CALL_PARAMETER_COUNT, id);
    exit(1);
  }
  if(ctx->FUNCTION_CALL_PARAMETER_COUNT > function->PARAM_COUNT){
    printf("%d - %d\n", ctx->FUNCTION_CALL_PARAMETER_COUNT, function->PARAM_COUNT);
    printf("Too many parameters for function call to %s\n", id);
    exit(1);
  }

  addQuad(OP_GOSUB, -1, function->index, -1, ctx);
  ctx->FUNCTION_CALL_PARAMETER_COUNT = 0;
}

void printContextVariables(ParserContext * ctx)
{
  // printf("============== Parser Context Variables ============\n");

  // for(int i = 0; i < ctx->INT_VARIABLES_COUNT; i++)
  // {
  //   ctx->INT_VARIABLES[i] = -999;
  //   printf("Variable at index %d has value %d\n", INT_VARIABLES_SEGMENT + i, ctx->INT_VARIABLES[i]);
  // }
  // for(int i = 0; i < ctx->FLOAT_VARIABLES_COUNT; i++)
  // {
  //   ctx->FLOAT_VARIABLES[i] = -999.10f;
  //   printf("Variable at index %d has value %.2f\n", FLOAT_VARIABLES_SEGMENT + i, ctx->FLOAT_VARIABLES[i]);
  // }
}

int handleQuad(int id1, int id2, OPERATORS op, ParserContext * ctx)
{
  // OPERATORS oper = stringToOp(op);
  //printf("\n%d\t%d\t%d\t%d\n", op, id1, id2, vAddress);
  int vAddress;
  if(op == OP_EQ) addQuad(op, -1, id2, id1, ctx);
  else {
    TYPES resultingType = getResultingType(vAddressToType(id1), op, vAddressToType(id2));
    if (resultingType == TYPE_ERROR) {
      printf("\n%d %d %d\n", id1, op, id2);
      printf(typeToString(vAddressToType(id1)));
      printf(opToString(vAddressToType(op)));
      printf(typeToString(vAddressToType(id2)));
      printf("ERROR de Tipos, no se puede ejecutar la operación\n");
      exit(EXIT_FAILURE);  // Detiene el programa con código de 
  
    }
    switch (resultingType)
    {
      case TYPE_INT:
        if(ctx->currentFunction){
          vAddress = (int)ctx->currentFunction->INT_TEMPS_COUNT + INT_TEMPS_SEGMENT;
          ctx->currentFunction->INT_TEMPS_COUNT++;
        } else {
          vAddress = (int)ctx->INT_TEMPS_COUNT + INT_TEMPS_SEGMENT;
          ctx->INT_TEMPS_COUNT++;
        }
        addQuad(op, id1, id2, vAddress, ctx);
      break;
      case TYPE_FLOAT:
        if(ctx->currentFunction){
          vAddress = (int)ctx->currentFunction->FLOAT_TEMPS_COUNT + FLOAT_TEMPS_SEGMENT;
          ctx->currentFunction->FLOAT_TEMPS_COUNT++;
        }else{
          vAddress = (int)ctx->FLOAT_TEMPS_COUNT + FLOAT_TEMPS_SEGMENT;
          ctx->FLOAT_TEMPS_COUNT++;
        }
        addQuad(op, id1, id2, vAddress, ctx);
      break;
      case TYPE_BOOL:
        if(ctx->currentFunction){
          vAddress = (int)ctx->currentFunction->BOOL_TEMPS_COUNT + BOOL_TEMPS_SEGMENT;
          ctx->currentFunction->BOOL_TEMPS_COUNT++;
        }else{
          vAddress = (int)ctx->BOOL_TEMPS_COUNT + BOOL_TEMPS_SEGMENT;
          ctx->BOOL_TEMPS_COUNT++;
        }
        addQuad(op, id1, id2, vAddress, ctx);
      break;
      
    default:
      break;
    }
  } 
  return vAddress;
}

void quadsSolve(ParserContext * ctx)
{
  while(!sIsEmpty(&ctx->operators) && !sIsEmpty(&ctx->operations))
  {
    int right = sPop(&ctx->operators);
    int left = sPop(&ctx->operators);
    // printf("Right %d Left %d\n", right, left);
    // handleQuad(left,right,sPop(&ctx->operations),&ctx);
    
    sPush(&ctx->operators, handleQuad(left,right,sPop(&ctx->operations),ctx));
  }
  // return sPop(&ctx->operators);
}

void handleOperator(char * id1, ParserContext * ctx)
{
  // printf("HandleOperator: %s\n", id1);
  VariableEntry * var1 = findVariable(id1, ctx);
  // printf("HandleOperator:exit\n");
  sPush(&ctx->operators, var1->VAddress);
}

void handleOperation(char * op, ParserContext * ctx){
  // printf("Evaluating: %s and %d\n", op, sPeek(&ctx->operations));
  while(!hasPriority(stringToOp(op), sPeek(&ctx->operations)))
  {
    int right = sPop(&ctx->operators);
    int left = sPop(&ctx->operators);
    // handleQuad(left,right,sPop(&ctx->operations),&ctx);
    // printf("Popping in handleOperation\nRight %d Left %d", right, left);
    // printf("Pushing Operation %s\n", op);
    sPush(&ctx->operators, handleQuad(left,right,sPop(&ctx->operations),ctx));
  }
  // printf("Pushing Operation %s\n", op);
  sPush(&ctx->operations, stringToOp(op));
}

void handleConditionStart(ParserContext * ctx){
  addQuad(OP_GOTOF, sPop(&ctx->operators), -1, -1,ctx);
  sPush(&ctx->goToAddresses, ctx->quadList->count-1);
}
void handleConditionEnd(ParserContext * ctx){
  int index = sPop(&ctx->goToAddresses);
  ctx->quadList->quadruples[index].addRight = ctx->quadList->count;
}
void handleConditionElseStart(ParserContext * ctx){
  int index = sPop(&ctx->goToAddresses);
  addQuad(OP_GOTO, -1, -1, -1, ctx);
  sPush(&ctx->goToAddresses, ctx->quadList->count-1);

  ctx->quadList->quadruples[index].addRight = ctx->quadList->count;
}
void handleConditionElseEnd(ParserContext * ctx){
  int index = sPop(&ctx->goToAddresses);
  ctx->quadList->quadruples[index].addRight = ctx->quadList->count;
}
void handleCycleConditionStart(ParserContext * ctx){
  sPush(&ctx->goToAddresses, ctx->quadList->count);
}
void handleCycleStart(ParserContext * ctx){
  // printf("Este");
  addQuad(OP_GOTOF, sPop(&ctx->operators), -1, -1,ctx);
  sPush(&ctx->goToAddresses, ctx->quadList->count-1);
}
void handleCycleEnd(ParserContext * ctx) {
  int index = sPop(&ctx->goToAddresses);
  int returnIndex = sPop(&ctx->goToAddresses);
  addQuad(OP_GOTO, -1, returnIndex, -1,ctx);
  ctx->quadList->quadruples[index].addRight = ctx->quadList->count;
}

void handlePrintString(char* val, ParserContext * ctx){
  // printf("Got string: %s\n", val);
  addQuad(OP_PRINT, ctx->STRING_CONSTANTS_COUNT + STRING_CONSTANTS_SEGMENT, -1, -1, ctx);
  char * val_copy = strdup(val);
  addConstantString(val_copy, ctx);
}
void handlePrintExpression(char* exp, ParserContext * ctx){
  // printf("Got expression\n");
  addQuad(OP_PRINT, sPop(&ctx->operators), -1, -1, ctx);
}
