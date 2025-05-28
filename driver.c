#include <stdio.h>
#include <stdlib.h>
#include "syntax/example.h"
#include "syntax/queue.h"
#include "syntax/tokenNode.h"
#include "utils/parserContext.h"
#include "utils/semanticCube.h"
#include "flex/scanner.h"

void* ParseAlloc(void* (*allocProc)(size_t));
void* Parse(void*, int, const char*, ParserContext * ctx);
void* ParseFree(void*, void(*freeProc)(void*));


int main(int argc, char** argv){
  
  FILE* file = fopen(argv[1], "r");
  if (!file) {
    fprintf(stderr, "Error: Cannot open file %s\n", argv[1]);
    return 1;
  }

  initSemanticCube();

  ParserContext * ctx = (ParserContext*)malloc(sizeof(ParserContext));
  ctx->functionTable = NULL;
  ctx->currentFunction = NULL;
  ctx->INT_VARIABLES_COUNT = 0;
  ctx->INT_CONSTANTS_COUNT = 0;
  ctx->INT_TEMPS_COUNT = 0;
  ctx->FLOAT_VARIABLES_COUNT = 0;
  ctx->FLOAT_CONSTANTS_COUNT = 0;
  ctx->FLOAT_TEMPS_COUNT = 0;
  ctx->STRING_CONSTANTS_COUNT = 0;
  ctx->BOOL_TEMPS_COUNT = 0;

  ctx->INT_CONSTANTS = malloc(sizeof(int));
  ctx->FLOAT_CONSTANTS = malloc(sizeof(float));
  ctx->STRING_CONSTANTS = malloc(sizeof(char*));

  yyscan_t scanner;
  yylex_init(&scanner);
  yyset_in(file, scanner);
  
  struct Queue q;
  qInitialize(&q);
  
  void * parser = ParseAlloc(malloc);
  
  int lexCode;
  do{
    lexCode = yylex(scanner);
    TokenNode* newToken = (TokenNode*)malloc(sizeof(TokenNode));
    newToken->text = strdup(yyget_text(scanner));
    newToken->type = lexCode;
    enqueue(&q, newToken);
    
    Parse(parser, newToken->type, newToken->text, ctx);
  } while(lexCode > 0);
  printFunctionTable(ctx->functionTable);
  printQuads(ctx);
  // initMemoryMap(ctx);
  // printContextVariables(ctx);
  // TokenNode* token;
  // while(!isEmpty(&q))
  // {
  //   token = dequeue(&q);
  //   printf("Token: %s - Value: %d\n", token->text, token->type);
  //   free(token->text);
  //   free(token);
  // }

  yylex_destroy(scanner);
  ParseFree(parser, free);
  return 0;
}