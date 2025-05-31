#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashDriver.h"

void handleVariableList(StringList* vars, ParserContext * ctx, const char* type){
  StringList* node = vars;
  while (node != NULL) {
    TYPES typeObj = stringToType(type);
    int vAddress = -1;
    switch(typeObj)
    {
      case TYPE_INT:
        vAddress = (int)ctx->INT_VARIABLES_COUNT + INT_VARIABLES_SEGMENT;
        if(ctx->currentFunction){
          vAddress = vAddress + ctx->currentFunction->INT_VARIABLES_COUNT;
          ctx->currentFunction->INT_VARIABLES_COUNT++;
        } else {
          ctx->INT_VARIABLES_COUNT++;
        }
        break;
      case TYPE_FLOAT:
        vAddress = (int)ctx->FLOAT_VARIABLES_COUNT + FLOAT_VARIABLES_SEGMENT;
        if(ctx->currentFunction){
          vAddress = vAddress + ctx->currentFunction->FLOAT_VARIABLES_COUNT;
          ctx->currentFunction->FLOAT_VARIABLES_COUNT++;
        } else {
          ctx->FLOAT_VARIABLES_COUNT++;
        }
      break;
      
    }
    // printf("Añadiendo variable %s con direccion %d para la funcion %s\n", node->id, vAddress, &ctx->currentFunction->name);
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
    HASH_ADD_STR(*table, name, entry);
    // VariableEntry *tmp;
    // for (tmp = *table; tmp != NULL; tmp = tmp->hh.next) {
    //   printf("Variable: %s, Address: %d, Type: %d\n", tmp->name, tmp->VAddress, tmp->type);
    // }
  } else {
    printf("Error: Variable '%s' ya declarada\n", id);
  }
}

void addToFunctionSignature(const char* type, ParserContext * ctx){
  FunctionEntry * function = ctx->currentFunction;
  // Calculate required space for new signature
  size_t oldLen = function->signature ? strlen(function->signature) : 0;
  size_t typeLen = strlen(type);
  size_t totalLen = oldLen + typeLen; // +1 for comma or null terminator

  char* newSignature = malloc(totalLen);
  if (!newSignature) {
    printf("Memory allocation failed\n");
    exit(EXIT_FAILURE);
  }

  newSignature[0] = '\0'; // Initialize to empty string

  if (function->signature) {
    // snprintf(newSignature, totalLen, "%s%s", type, function->signature);
    strcat(newSignature, type);
    strcat(newSignature, ctx->currentFunction->signature);
  } else {
    strcat(newSignature, type);
    // snprintf(newSignature, totalLen, "%s", type);
  }

  function->signature = newSignature;
  function->PARAM_COUNT++;
}

void addConstantInt(char* val, ParserContext * ctx){
  int newVal = strtol(val, NULL, 10);
  // printf("Pushing constant %d\n", newVal);
  
  ctx->INT_CONSTANTS = realloc(ctx->INT_CONSTANTS, sizeof(int) * (ctx->INT_CONSTANTS_COUNT + 1));
  if (ctx->INT_CONSTANTS == NULL) {
    fprintf(stderr, "Error reallocating memory for INT_CONSTANTS\n");
    exit(EXIT_FAILURE);
  }
  ctx->INT_CONSTANTS[ctx->INT_CONSTANTS_COUNT] = newVal;
  // printf("Pre Check: %d", sPeek(&ctx->operators));
  sPush(&ctx->operators, INT_CONSTANTS_SEGMENT + ctx->INT_CONSTANTS_COUNT);
  // printf("Pre Check: %d", sPeek(&ctx->operators));
  ctx->INT_CONSTANTS_COUNT++;
}
void addConstantFloat(char* val, ParserContext * ctx){
  // printf("\nNEW VAL: %s\n", val);
  float newVal = strtof(val, NULL);
  // printf("\nNEW VAL: %f\n", newVal);
  sPush(&ctx->operators, FLOAT_CONSTANTS_SEGMENT + ctx->FLOAT_CONSTANTS_COUNT);
  ctx->FLOAT_CONSTANTS[ctx->FLOAT_CONSTANTS_COUNT] = newVal;
  ctx->FLOAT_CONSTANTS_COUNT++;
}

void addConstantString(char* val, ParserContext * ctx){
  ctx->STRING_CONSTANTS = realloc(ctx->STRING_CONSTANTS, sizeof(char*) * (ctx->STRING_CONSTANTS_COUNT + 1));
  ctx->STRING_CONSTANTS[ctx->STRING_CONSTANTS_COUNT++] = strdup(val); // <- COPIA
}

void deleteVariable(struct VariableEntry **table, char * id){
  struct VariableEntry *entry;

  HASH_FIND_STR(*table, id, entry);
  if(entry != NULL){
    HASH_DEL(*table, entry);
    //printf("Elminando %s\n", entry->name);
    free(entry);
  } else {
    printf("Delete: No existe el objeto: %d \n", id);
  }
}

struct VariableEntry *findVariable(char *id, ParserContext * ctx){
  struct VariableEntry *entry;
  if (id == NULL) {
    printf("El id es NULL\n");
  }
  // printf(id);
  if(ctx->currentFunction){
    HASH_FIND_STR(ctx->currentFunction->variableTable, id, entry);
    if(!entry){
      HASH_FIND_STR(ctx->programFunction->variableTable, id, entry);
    }

  } else HASH_FIND_STR(ctx->programFunction->variableTable, id, entry);
  if(entry != NULL){
    return entry;
  } else {
    printf("No existe la variable: %s\n", id);
    exit(1);
    return NULL;

  }
}

void printVariableTable(VariableEntry* table) {
    VariableEntry* current;
    VariableEntry * tmp;
    HASH_ITER(hh, table, current, tmp) {
        // printf("Nombre: %s, Tipo: ", current->name);

        // Opcional: imprime tipo como string
        switch (current->type) {
            case TYPE_INT: printf("int, "); break;
            case TYPE_FLOAT: printf("float, "); break;
            case TYPE_STRING: printf("string, "); break;
            default: printf("desconocido, "); break;
        }

        // printf("Dir. Virtual: %d\n", current->VAddress);
    }
    printf("\n");
}

void addFunction(struct FunctionEntry **table, const char *id, const char* type){
  struct FunctionEntry *entry;
  
  HASH_FIND_STR(*table, id, entry);
  if(entry == NULL){
    entry = (FunctionEntry*)malloc(sizeof *entry);
    entry->name = strdup(id);
    entry->type = stringToType(type);
    entry->signature = "";
    entry->variableTable = NULL;
    entry->INT_VARIABLES_COUNT = 0;
    entry->FLOAT_VARIABLES_COUNT = 0;
    entry->INT_TEMPS_COUNT = 0;
    entry->FLOAT_TEMPS_COUNT = 0;
    entry->BOOL_TEMPS_COUNT = 0;
    entry->PARAM_COUNT = 0;
    entry->index = 0;
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
    //printf("Elminando %s\n", entry->name);
    free(entry);
  } else {
    printf("Delete Func: No existe el objeto: %d \n", id);
  }
}

struct FunctionEntry *findFunction(struct FunctionEntry **table, const char *id){
  struct FunctionEntry *entry;
  HASH_FIND_STR(*table, id, entry);
  if(entry != NULL){
    return entry;
  } else {
    printf("Find Func: No existe el objeto: %d\n", id);
    return NULL;
  }
}

void printFunctionTable(FunctionEntry* table) {
    FunctionEntry* current;
    FunctionEntry * tmp;
    
    printf("=== Tabla de Funciones ===\n");
    HASH_ITER(hh, table, current, tmp) {
      printf("=== MetaDatos de Funcion %s ===\n", current->name);
      printf("Index: %d\n", current->index);
      printf("Firma: %s\n", current->signature);
      printf("Int Variables: %d\n", current->INT_VARIABLES_COUNT);
      printf("Int Temps: %d\n", current->INT_TEMPS_COUNT);
      printf("Float Variables: %d\n", current->FLOAT_VARIABLES_COUNT);
      printf("Float Temps: %d\n", current->FLOAT_TEMPS_COUNT);
      printf("Bool Temps: %d\n", current->BOOL_TEMPS_COUNT);
      printf("Start Quad: %d\n", current->startQuad);
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