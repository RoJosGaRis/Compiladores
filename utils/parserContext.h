// parser_context.h
#ifndef PARSER_CONTEXT_H
#define PARSER_CONTEXT_H

#include <stdio.h>
#include <stdlib.h>

// #include "types.h"
#include "quads/quads.h"
#include "hashDriver.h"
#include "semanticCube.h"
// #include "../consts.h"
// #include "structs.h"

void initMemoryMap(ParserContext * ctx);
void initContext(ParserContext * ctx);
void printContextVariables(ParserContext * ctx);
int handleQuad(int id1, int id2, OPERATORS op, ParserContext * ctx);
void handleOperation(char * op, ParserContext * ctx);
void handleOperator(char * id1, ParserContext * ctx);
void quadsSolve(ParserContext * ctx);
void handleConditionStart(ParserContext * ctx);
void handleConditionEnd(ParserContext * ctx);
void handleConditionElseStart(ParserContext * ctx);
void handleConditionElseEnd(ParserContext * ctx);
void handleCycleConditionStart(ParserContext * ctx);
void handleCycleStart(ParserContext * ctx);
void handleCycleEnd(ParserContext * ctx);
void handlePrintString(char* val, ParserContext * ctx);
void handlePrintExpression(char* exp, ParserContext * ctx);


#endif

