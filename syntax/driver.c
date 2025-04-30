#include <stdio.h>
#include <stdlib.h>
#include "example.h"
#include "../flex/scanner.h"

void* ParseAlloc(void* (*allocProc)(size_t));
void* Parse(void*, int, const char*);
void* ParseFree(void*, void(*freeProc)(void*));

int main(int argc, char** argv){

  FILE* file = fopen(argv[1], "r");
  if (!file) {
      fprintf(stderr, "Error: Cannot open file %s\n", argv[1]);
      return 1;
  }

  yyscan_t scanner;
  yylex_init(&scanner);
  yyset_in(file, scanner);

  void * parser = ParseAlloc(malloc);

  int lexCode;
  do{
  
    lexCode = yylex(scanner);
    Parse(parser, lexCode, yyget_text(scanner));
    // printf("ID: %d\nVALUE: %s\n\n", lexCode, lexCode > 0 ? yyget_text(scanner) : "EOF");
  } while(lexCode > 0);

  yylex_destroy(scanner);
  ParseFree(parser, free);
  return 0;
}