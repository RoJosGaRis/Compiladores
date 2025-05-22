#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashDriver.h"

void handleVariableList(StringList* vars, ParserContext * ctx, const char* type){
  StringList* node = vars;
  while (node != NULL) {
    // printf("Adding variable %s of type %s to %s\n", node->id, type, ctx->currentFunction ? ctx->currentFunction->name : ctx->programFunction->name);
    TYPES typeObj = stringToType(type);
    int vAddress = -1;
    switch(typeObj)
    {
      case TYPE_INT:
        vAddress = (int)ctx->INT_VARIABLES_COUNT + INT_VARIABLES_SEGMENT;
        // printf("VAddress %d\n", vAddress);
        ctx->INT_VARIABLES_COUNT++;
        printf("VAddress: %d\n", vAddress);
        printf("Int Variables Count: %d\n", ctx->INT_VARIABLES_COUNT);
      // ctx->INT_VARIABLES = realloc(ctx->INT_VARIABLES, sizeof(int) * ctx->INT_VARIABLES_COUNT);
      break;
      case TYPE_FLOAT:
        vAddress = (int)ctx->FLOAT_VARIABLES_COUNT + FLOAT_VARIABLES_SEGMENT;
        // printf("VAddress %d\n", vAddress);
        ctx->FLOAT_VARIABLES_COUNT++;
        printf("VAddress: %d\n", vAddress);
        printf("Float Variables Count: %d\n", ctx->FLOAT_VARIABLES_COUNT);
      // ctx->INT_VARIABLES = realloc(ctx->INT_VARIABLES, sizeof(int) * ctx->INT_VARIABLES_COUNT);
      break;

    }
    if(ctx->currentFunction == NULL)
    {
      addVariable(&ctx->programFunction->variableTable, node->id, type, vAddress);
    } 
    else
    {
      addVariable(&ctx->currentFunction->variableTable, node->id, type, vAddress);
    }


    StringList* temp = node;
    node = node->next;
    free(temp->id);
    free(temp);
  }
}

void addVariable(VariableEntry **table, const char *id, const char* type, int VAddress){
  VariableEntry *entry;

  HASH_FIND_STR(*table, id, entry);
  if(entry == NULL){
    entry = (VariableEntry*)malloc(sizeof *entry);
    entry->name = strdup(id);
    entry->type = stringToType(type);
    entry->VAddress = VAddress;
    printf("Agregando variable %s\n", id);
    HASH_ADD_STR(*table, name, entry);
  } else {
    printf("Error: Variable '%s' ya declarada\n", id);
  }
}

void deleteVariable(struct VariableEntry **table, char * id){
  struct VariableEntry *entry;

  HASH_FIND_STR(*table, id, entry);
  if(entry != NULL){
    HASH_DEL(*table, entry);
    printf("Elminando %s\n", entry->name);
    free(entry);
  } else {
    printf("No existe el objeto: %d \n", id);
  }
}

struct VariableEntry *findVariable(struct VariableEntry **table, char *id){
  struct VariableEntry *entry;
  if (id == NULL) {
    printf("El id es NULL\n");
  }
  HASH_FIND_STR(*table, id, entry);
  if(entry != NULL){
    return entry;
  } else {
    printf("No existe el objeto: %s\n", id);
    return NULL;
  }
}

void printVariableTable(VariableEntry* table) {
    VariableEntry* current;
    VariableEntry * tmp;
    HASH_ITER(hh, table, current, tmp) {
        printf("Nombre: %s, Tipo: ", current->name);

        // Opcional: imprime tipo como string
        switch (current->type) {
            case TYPE_INT: printf("int\n"); break;
            case TYPE_FLOAT: printf("float\n"); break;
            case TYPE_STRING: printf("string\n"); break;
            default: printf("desconocido\n"); break;
        }
    }
}

void addFunction(struct FunctionEntry **table, const char *id, const char* type){
  struct FunctionEntry *entry;
  
  HASH_FIND_STR(*table, id, entry);
  if(entry == NULL){
    entry = (FunctionEntry*)malloc(sizeof *entry);
    entry->name = strdup(id);
    entry->type = stringToType(type);
    entry->variableTable = NULL;
    HASH_ADD_STR(*table, name, entry);
  } else {
    printf("Error: Función '%s' ya declarada\n", id);
  }
}

void deleteFunction(struct FunctionEntry **table, char * id){
  struct FunctionEntry *entry;

  HASH_FIND_STR(*table, id, entry);
  if(entry != NULL){
    HASH_DEL(*table, entry);
    printf("Elminando %s\n", entry->name);
    free(entry);
  } else {
    printf("No existe el objeto: %d \n", id);
  }
}

struct FunctionEntry *findFunction(struct FunctionEntry **table, const char *id){
  struct FunctionEntry *entry;
  HASH_FIND_STR(*table, id, entry);
  if(entry != NULL){
    return entry;
  } else {
    printf("No existe el objeto: %d\n", id);
    return NULL;
  }
}

void printFunctionTable(FunctionEntry* table) {
    FunctionEntry* current;
    FunctionEntry * tmp;
    printf("=== Tabla de Funciones ===\n");
    HASH_ITER(hh, table, current, tmp) {
        printf("=== Variables de Funcion %s ===\n", current->name);
        printVariableTable(current->variableTable);
        // // Opcional: imprime tipo como string
        // switch (current->type) {
        //     case TYPE_INT: printf("int\n"); break;
        //     case TYPE_FLOAT: printf("float\n"); break;
        //     case TYPE_STRING: printf("string\n"); break;
        //     default: printf("desconocido\n"); break;
        // }
    }
}