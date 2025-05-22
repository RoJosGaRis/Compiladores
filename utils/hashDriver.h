#ifndef HASH_DRIVER_H
#define HASH_DRIVER_H

// #include <stdio.h>
// #include <stdlib.h>
// #include <stdbool.h>
#include "uthash.h"
// #include "parserContext.h"
#include "tables.h"
// #include "types.h"
#include "../consts.h"
#include "structs.h"

void handleVariableList(StringList* vars, ParserContext * ctx, const char* type);
void addVariable(VariableEntry **table, const char *id, const char* type, int VAddress);
void deleteVariable(struct VariableEntry **table, char * id);
struct VariableEntry * findVariable(struct VariableEntry **table, char *id);
void printVariableTable(VariableEntry* table);
void addFunction(FunctionEntry **table, const char *id, const char* type);
void deleteFunction(struct FunctionEntry **table, char * id);
struct FunctionEntry *findFunction(struct FunctionEntry **table, const char *id);
void printFunctionTable(FunctionEntry* table);

#endif