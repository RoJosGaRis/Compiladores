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
  for(int i = 0; i < ctx->quadList->count; i++)
  {
    Quadruple quad = ctx->quadList->quadruples[i];
    printf("%d :: %d\t\t%d\t\t%d\t\t%d\n",i, quad.op, quad.addLeft, quad.addRight, quad.addRes);
  }
}
