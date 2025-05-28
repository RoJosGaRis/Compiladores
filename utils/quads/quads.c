#include <stdio.h>
#include <stdlib.h>
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
  
  ctx->quadList->quadruples[ctx->quadList->count] = newQuad;
  ctx->quadList->count++;
}

void printQuads(ParserContext * ctx){
  // int     INT_VARIABLES_COUNT;
  // int     INT_CONSTANTS_COUNT;
  // int     INT_TEMPS_COUNT;
  // int     FLOAT_VARIABLES_COUNT;
  // int     FLOAT_CONSTANTS_COUNT;
  // int     FLOAT_TEMPS_COUNT;
  // int     BOOL_TEMPS_COUNT;
  // int     STRING_CONSTANTS_COUNT;
  printf("%d\n", ctx->INT_VARIABLES_COUNT);
  printf("%d\n", ctx->INT_TEMPS_COUNT);
  printf("%d\n", ctx->FLOAT_VARIABLES_COUNT);
  printf("%d\n", ctx->FLOAT_TEMPS_COUNT);
  printf("%d\n", ctx->BOOL_TEMPS_COUNT);
  printf("%d\n", ctx->INT_CONSTANTS_COUNT);
  for(int i = 0; i < ctx->INT_CONSTANTS_COUNT; i++){
    printf("%d\n", ctx->INT_CONSTANTS[i]);
  }
  printf("%d\n", ctx->FLOAT_CONSTANTS_COUNT);
  for(int i = 0; i < ctx->FLOAT_CONSTANTS_COUNT; i++){
    printf("%d\n", ctx->FLOAT_CONSTANTS[i]);
  }
  printf("%d\n", ctx->STRING_CONSTANTS_COUNT);
  for(int i = 0; i < ctx->STRING_CONSTANTS_COUNT; i++){
    printf("%d\n", ctx->STRING_CONSTANTS[i]);
  }

  for(int i = 0; i < ctx->quadList->count; i++)
  {
    Quadruple quad = ctx->quadList->quadruples[i];
    printf("%d\t\t%d\t\t%d\t\t%d\n",quad.op, quad.addLeft, quad.addRight, quad.addRes);
  }
}
