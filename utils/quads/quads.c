#include "quads.h"

void addQuad(OPERATORS op, int left, int right, int result, ParserContext * ctx){
  ctx->quadList->quadruples = realloc(
    ctx->quadList->quadruples,
    sizeof(Quadruple) * (ctx->quadList->count + 1)
  );

  Quadruple newQuad;
  newQuad.op = op;
  newQuad.addLeft = left;
  newQuad.addRight = right;
  newQuad.addRes = result;
  // printf("Added quad: %d\t%d\t%d\t%d\n", newQuad.op, newQuad.addLeft, newQuad.addRight, newQuad.addRes);
  ctx->quadList->quadruples[ctx->quadList->count] = newQuad;
  ctx->quadList->count++;
}

void printQuads(FILE * fileptr, ParserContext * ctx){
  // int     INT_VARIABLES_COUNT;
  // int     INT_CONSTANTS_COUNT;
  // int     INT_TEMPS_COUNT;
  // int     FLOAT_VARIABLES_COUNT;
  // int     FLOAT_CONSTANTS_COUNT;
  // int     FLOAT_TEMPS_COUNT;
  // int     BOOL_TEMPS_COUNT;
  // int     STRING_CONSTANTS_COUNT;
  fprintf(fileptr, "%d\n", ctx->INT_VARIABLES_COUNT);
  fprintf(fileptr, "%d\n", ctx->INT_TEMPS_COUNT);
  fprintf(fileptr, "%d\n", ctx->FLOAT_VARIABLES_COUNT);
  fprintf(fileptr, "%d\n", ctx->FLOAT_TEMPS_COUNT);
  fprintf(fileptr, "%d\n", ctx->BOOL_TEMPS_COUNT);
  fprintf(fileptr, "%d\n", ctx->quadList->count);
  fprintf(fileptr, "%d\n", ctx->INT_CONSTANTS_COUNT);
  fprintf(fileptr, "%d\n", ctx->FLOAT_CONSTANTS_COUNT);
  fprintf(fileptr, "%d\n", ctx->STRING_CONSTANTS_COUNT);
  fprintf(fileptr, "%d\n", ctx->FUNCTION_COUNT);
  // printf("Quads: Function Count: %d", ctx->FUNCTION_COUNT);
  for(int i = 0; i < ctx->INT_CONSTANTS_COUNT; i++){
    fprintf(fileptr, "%d\n", ctx->INT_CONSTANTS[i]);
  }
  for(int i = 0; i < ctx->FLOAT_CONSTANTS_COUNT; i++){
    fprintf(fileptr, "%2f\n", ctx->FLOAT_CONSTANTS[i]);
  }
  for(int i = 0; i < ctx->STRING_CONSTANTS_COUNT; i++){
    fprintf(fileptr, "%s\n", ctx->STRING_CONSTANTS[i]);
  }
  fprintFunctionTable(fileptr, ctx->functionTable);
  for(int i = 0; i < ctx->quadList->count; i++)
  {
    Quadruple quad = ctx->quadList->quadruples[i];
    fprintf(fileptr, "%d\t\t%d\t\t%d\t\t%d\n",quad.op, quad.addLeft, quad.addRight, quad.addRes);
  }
}

void fprintFunctionTable(FILE * fileptr, FunctionEntry* table) {
  FunctionEntry* current;
  FunctionEntry * tmp;
  
  HASH_ITER(hh, table, current, tmp) {;
    fprintf(fileptr, "%s\n", current->name);
    fprintf(fileptr, "%d\n", current->index + 1);
    fprintf(fileptr, "%s\n", current->signature != ""? current->signature : "None");
    fprintf(fileptr, "%d\n", current->INT_VARIABLES_COUNT);
    fprintf(fileptr, "%d\n", current->INT_TEMPS_COUNT);
    fprintf(fileptr, "%d\n", current->FLOAT_VARIABLES_COUNT);
    fprintf(fileptr, "%d\n", current->FLOAT_TEMPS_COUNT);
    fprintf(fileptr, "%d\n", current->BOOL_TEMPS_COUNT);
    fprintf(fileptr, "%d\n", current->startQuad);
  }
}
