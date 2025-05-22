// parser_context.h
#ifndef QUADS_H
#define QUADS_H

#include <stdbool.h>
#include "../structs.h"

// Declaraciones de funciones
void addQuad(OPERATORS op, int left, int right, int result, ParserContext * ctx);
void printQuads(ParserContext * ctx);

#endif
