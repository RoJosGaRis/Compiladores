#include <stdio.h>
#include <stdlib.h>
#include "syntax/example.h"
#include "syntax/queue.h"
#include "syntax/tokenNode.h"
#include "utils/parserContext.h"
#include "utils/semanticCube.h"
#include "machine/machineDriver.h"
#include "flex/scanner.h"

void* ParseAlloc(void* (*allocProc)(size_t));
void* Parse(void*, int, const char*, ParserContext * ctx);
void* ParseFree(void*, void(*freeProc)(void*));
void * parser;

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
  char *parsedText = NULL;
  yyset_extra(&parsedText, scanner);
  yyset_in(file, scanner);
  
  struct Queue q;
  qInitialize(&q);
  
  parser = ParseAlloc(malloc);
  int lexCode;
  do{
    lexCode = yylex(scanner);
    char* value = strdup(yyget_text(scanner));

    if (lexCode == TKN_STRING_CONST) {
      char * parsedCopy = strdup(parsedText);
      Parse(parser, TKN_STRING_CONST, parsedCopy, ctx);
      parsedText = NULL;
    } else {
        Parse(parser, lexCode, value, ctx);
    }
    // TokenNode* newToken = (TokenNode*)malloc(sizeof(TokenNode));
    // newToken->text = strdup(yyget_text(scanner));
    // newToken->type = lexCode;
    // // enqueue(&q, newToken);
    // Parse(parser, newToken->type, value, ctx);
  } while(lexCode > 0);
  // printFunctionTable(ctx->functionTable);
  FILE *newFilePtr;
  char filename[256];  // Enough space for most filenames

  // printf("%s", ctx->programFunction);
  sprintf(filename, "%s.txt", ctx->programFunction->name);
  newFilePtr = fopen(filename, "w");

  if (newFilePtr == NULL) {
    fprintf(stderr, "Error: Could not create file %s\n", filename);
    exit(EXIT_FAILURE);
  }
  // printf("Print Quads\n");
  printQuads(newFilePtr, ctx);
  fclose(newFilePtr);
  ParseFree(parser, free);
  yylex_destroy(scanner);
  // printf("Machine run");
  machine_run(filename);

  return 0;
}