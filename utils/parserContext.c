#include "parserContext.h"

void initMemoryMap(ParserContext * ctx)
{
  ctx->INT_VARIABLES = NULL;
  ctx->INT_CONSTANTS = NULL;
  ctx->INT_TEMPS = NULL;
  ctx->FLOAT_VARIABLES = NULL; 
  ctx->FLOAT_CONSTANTS = NULL;
  ctx->FLOAT_TEMPS = NULL;
  ctx->STRING_CONSTANTS = NULL;
  ctx->STRING_TEMPS = NULL;
  
  ctx->INT_VARIABLES = malloc(sizeof(int) * ctx->INT_VARIABLES_COUNT);
  ctx->FLOAT_VARIABLES = malloc(sizeof(int) * ctx->FLOAT_VARIABLES_COUNT);
}

void initContext(ParserContext * ctx)
{
  sInitialize(&ctx->operations);
  sInitialize(&ctx->operators);

  ctx->quadList = malloc(sizeof(QuadList));
  ctx->quadList->quadruples = NULL;
  ctx->quadList->count = 0;
}

void printContextVariables(ParserContext * ctx)
{
  printf("============== Parser Context Variables ============\n");

  for(int i = 0; i < ctx->INT_VARIABLES_COUNT; i++)
  {
    ctx->INT_VARIABLES[i] = -999;
    printf("Variable at index %d has value %d\n", INT_VARIABLES_SEGMENT + i, ctx->INT_VARIABLES[i]);
  }
  for(int i = 0; i < ctx->FLOAT_VARIABLES_COUNT; i++)
  {
    ctx->FLOAT_VARIABLES[i] = -999.10f;
    printf("Variable at index %d has value %.2f\n", FLOAT_VARIABLES_SEGMENT + i, ctx->FLOAT_VARIABLES[i]);
  }
}

int handleQuad(int id1, int id2, OPERATORS op, ParserContext * ctx)
{
  if (getResultingType(vAddressToType(id1), op, vAddressToType(id2)) == TYPE_ERROR) {
    printf("\n%d %d %d\n", id1, op, id2);
    fprintf(stderr, "ERROR de Tipos, no se puede ejecutar la operación\n");
    exit(EXIT_FAILURE);  // Detiene el programa con código de error
  }
  // OPERATORS oper = stringToOp(op);
  int vAddress = (int)ctx->INT_TEMPS_COUNT + INT_TEMPS_SEGMENT;
  //printf("\n%d\t%d\t%d\t%d\n", op, id1, id2, vAddress);
  if(op == OP_EQ) addQuad(op, -1, id2, id1, ctx);
  else {
    addQuad(op, id1, id2, vAddress, ctx);
    ctx->INT_TEMPS_COUNT++;
  } 
  return vAddress;
}

void quadsSolve(ParserContext * ctx)
{
  //printf("Solving");
  while(!sIsEmpty(&ctx->operators) && !sIsEmpty(&ctx->operations))
  {
    int right = sPop(&ctx->operators);
    int left = sPop(&ctx->operators);
    // handleQuad(left,right,sPop(&ctx->operations),&ctx);
    
    sPush(&ctx->operators, handleQuad(left,right,sPop(&ctx->operations),ctx));
  }
  // return sPop(&ctx->operators);
}

void handleOperator(char * id1, ParserContext * ctx)
{
  VariableEntry * var1 = findVariable(&ctx->programFunction->variableTable, id1);
  sPush(&ctx->operators, var1->VAddress);
}

void handleOperation(char * op, ParserContext * ctx){
  // printf("Evaluating: %s and %d\n", op, sPeek(&ctx->operations));
  while(!hasPriority(stringToOp(op), sPeek(&ctx->operations)))
  {
    int right = sPop(&ctx->operators);
    int left = sPop(&ctx->operators);
    // handleQuad(left,right,sPop(&ctx->operations),&ctx);
    sPush(&ctx->operators, handleQuad(left,right,sPop(&ctx->operations),ctx));
  }
  sPush(&ctx->operations, stringToOp(op));
}

void handleConditionStart(ParserContext * ctx){
  addQuad(stringToOp("GOTOF"), sPop(&ctx->operators), -1, -1,ctx);
  sPush(&ctx->goToAddresses, ctx->quadList->count-1);
}
void handleConditionEnd(ParserContext * ctx){
  int index = sPop(&ctx->goToAddresses);
  ctx->quadList->quadruples[index].addRight = ctx->quadList->count;
}
void handleConditionElseStart(ParserContext * ctx){
  int index = sPop(&ctx->goToAddresses);
  addQuad(stringToOp("GOTO"), -1, -1, -1, ctx);
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
  addQuad(stringToOp("GOTOF"), sPop(&ctx->operators), -1, -1,ctx);
  sPush(&ctx->goToAddresses, ctx->quadList->count-1);
}
void handleCycleEnd(ParserContext * ctx) {
  int index = sPop(&ctx->goToAddresses);
  int returnIndex = sPop(&ctx->goToAddresses);
  addQuad(stringToOp("GOTO"), -1, returnIndex, -1,ctx);
  ctx->quadList->quadruples[index].addRight = ctx->quadList->count;
}