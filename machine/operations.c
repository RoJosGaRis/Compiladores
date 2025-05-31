#include "operations.h"


void * getMemoryLocation(int searchAddress, MachineContext * ctx){
  // printf("GetMemoryLocation\n");
  if(searchAddress >= INT_VARIABLES_SEGMENT && searchAddress < INT_CONSTANTS_SEGMENT){
    int tempAddress = searchAddress - INT_VARIABLES_SEGMENT;
    if(tempAddress >= ctx->INT_VARIABLES_COUNT ){
      return &ctx->currentContext->INT_VARIABLES[tempAddress - ctx->INT_VARIABLES_COUNT];
    }
    return &ctx->INT_VARIABLES[searchAddress - INT_VARIABLES_SEGMENT];  
  } 
  if(searchAddress >= INT_CONSTANTS_SEGMENT && searchAddress < INT_TEMPS_SEGMENT){
    return &ctx->INT_CONSTANTS[searchAddress - INT_CONSTANTS_SEGMENT];
  }
  if(searchAddress >= INT_TEMPS_SEGMENT && searchAddress < FLOAT_VARIABLES_SEGMENT){
    if(ctx->currentContext){
      if(ctx->currentContext->INT_TEMPS[searchAddress - INT_TEMPS_SEGMENT]){
        return &ctx->currentContext->INT_TEMPS[searchAddress - INT_TEMPS_SEGMENT];
      }
    }
    return &ctx->INT_TEMPS[searchAddress-INT_TEMPS_SEGMENT];
  }
  
  if(searchAddress >= FLOAT_VARIABLES_SEGMENT && searchAddress < FLOAT_CONSTANTS_SEGMENT){
    int tempAddress = searchAddress - FLOAT_VARIABLES_SEGMENT;
    if(tempAddress >= ctx->FLOAT_VARIABLES_COUNT ){
      return &ctx->currentContext->FLOAT_VARIABLES[tempAddress - ctx->FLOAT_VARIABLES_COUNT];
    }
    return &ctx->FLOAT_VARIABLES[searchAddress - FLOAT_VARIABLES_SEGMENT];  
  }
  if(searchAddress >= FLOAT_CONSTANTS_SEGMENT && searchAddress < FLOAT_TEMPS_SEGMENT){
    return &ctx->FLOAT_CONSTANTS[searchAddress - FLOAT_CONSTANTS_SEGMENT];
    
  }
  if(searchAddress >= FLOAT_TEMPS_SEGMENT && searchAddress < STRING_CONSTANTS_SEGMENT){
    if(ctx->currentContext){
      if(ctx->currentContext->FLOAT_TEMPS[searchAddress - FLOAT_TEMPS_SEGMENT]){
        return &ctx->currentContext->FLOAT_TEMPS[searchAddress - FLOAT_TEMPS_SEGMENT];
      }
    }
    
    return &ctx->FLOAT_TEMPS[searchAddress-FLOAT_TEMPS_SEGMENT];
  }
  
  // #define STRING_CONSTANTS_SEGMENT    13000
  if(searchAddress >= STRING_CONSTANTS_SEGMENT && searchAddress < STRING_TEMPS_SEGMENT){
    return &ctx->FLOAT_VARIABLES[searchAddress - STRING_CONSTANTS_SEGMENT];  
    
  }
  // #define BOOL_TEMPS_SEGMENT          17000
  if(searchAddress >= BOOL_TEMPS_SEGMENT){
    if(ctx->currentContext){
      return &ctx->currentContext->BOOL_TEMPS[searchAddress - BOOL_TEMPS_SEGMENT];
    }
    
    // printf("GetMemoryLocation:Return\n");
    return &ctx->BOOL_TEMPS[searchAddress-BOOL_TEMPS_SEGMENT];
    
  }
  
  perror("Failed to fetch int location in memory. Address is not integer.");
  exit(1);
}

void initLocalContext(MachineContext *ctx, LocalContext * localCtx, int functionId){
  if (functionId < 0 || functionId >= ctx->FUNCTION_COUNT) {
    printf("Error: función %d fuera de rango (max: %d)\n", functionId, ctx->FUNCTION_COUNT - 1);
    return; // o abortar el programa, o manejar de forma segura
}
  FunctionData * functionData = &ctx->functions[functionId];
  localCtx->BOOL_TEMPS_COUNT = functionData->BoolTemps;
  // printf("Allocating %d BOOL temps\n", localCtx->BOOL_TEMPS_COUNT);
  localCtx->BOOL_TEMPS = (bool*)malloc(sizeof(bool)*localCtx->BOOL_TEMPS_COUNT);
  localCtx->INT_TEMPS_COUNT = functionData->IntTemps;
  localCtx->INT_TEMPS = (int*)malloc(sizeof(int)*localCtx->INT_TEMPS_COUNT);
  localCtx->INT_VARIABLES_COUNT = functionData->IntVariable;
  localCtx->INT_VARIABLES = (int*)malloc(sizeof(int)*localCtx->INT_VARIABLES_COUNT);
  localCtx->FLOAT_TEMPS_COUNT = functionData->FloatTemps;
  localCtx->FLOAT_TEMPS = (float*)malloc(sizeof(float)*localCtx->FLOAT_TEMPS_COUNT);
  localCtx->FLOAT_VARIABLES_COUNT = functionData->FloatVariables;
  localCtx->FLOAT_VARIABLES = (float*)malloc(sizeof(float)*localCtx->FLOAT_VARIABLES_COUNT); 
  localCtx->prev = (LocalContext*)malloc(sizeof(LocalContext));
}

void handleEquals(Quadruple * quad, MachineContext * ctx){
  // printf("handleEquals\n");
  TYPES rightType = vAddressToType(quad->addRight);
  TYPES resType = vAddressToType(quad->addRes);
  
  if(rightType == resType){
    switch(rightType){
      case TYPE_INT: {
        // printf("handleEquals:INT\n");
        // printf("handleEquals:INT:sourceAdd:%d\n", quad->addRight);
        int *source = getMemoryLocation(quad->addRight, ctx);
        // printf("handleEquals:INT:source:%d\n", *source);
        
        // printf("handleEquals:INT:targetAdd:%d\n", quad->addRes);
        int *target = getMemoryLocation(quad->addRes, ctx);
        // printf("handleEquals:INT:target:%d\n", *target);
        int value = *source;
        *target = value;
        // printf("handleEquals:INT:break\n");
        break;
      }
      case TYPE_FLOAT: {
        // printf("handleEquals:FLOAT\n");

        float *source = getMemoryLocation(quad->addRight, ctx);
        float *target = getMemoryLocation(quad->addRes, ctx);
        
        *target = *source;
        break;
      }
    }
  } else {
    fprintf(stderr, "Type mismatch in assignment: %d != %d\n", rightType, resType);
    exit(1);
  }
}

void handleLessThan(Quadruple * quad, MachineContext * ctx){
  float left = getNumericValue(quad->addLeft, ctx);
  float right = getNumericValue(quad->addRight, ctx);
  bool *target = (bool*)getMemoryLocation(quad->addRes, ctx);
  // printf("\nLess Than Left: %f Right: %f Saving %d in target: %d\n", left, right, left < right, quad->addRes);
  // printf("Target is: %p and has %d\n", left, right, (void*)target, *target);
  *target = left < right;
  // printf("Target is: %p and has %d\n", left, right, (void*)target, *target);
}
void handleGreaterThan(Quadruple * quad, MachineContext * ctx){
  float left = getNumericValue(quad->addLeft, ctx);
  float right = getNumericValue(quad->addRight, ctx);
  bool *target = (bool*)getMemoryLocation(quad->addRes, ctx);
  // printf("\nGreater Than Left: %f Right %f\n", left, right);
  *target = left > right;
}

float getNumericValue(int address, MachineContext* ctx) {
  TYPES type = vAddressToType(address);
  switch (type) {
      case TYPE_INT:
          return (float)(* (int*) getMemoryLocation(address, ctx));
      case TYPE_FLOAT:
          return * (float*) getMemoryLocation(address, ctx);
      default:
          fprintf(stderr, "Invalid type for arithmetic operation\n");
          exit(1);
  }
}

float getResult(float a, float b, OPERATORS op){
  switch(op){
    case OP_ADD:
      return a + b;
    break;
    case OP_SUB:
      return a - b;
    break;
    case OP_MUL:
      return a * b;
    break;
    case OP_DIV:
      return a / b;
    break;
  }
}


void handleEra(Quadruple *  quad, MachineContext * ctx){
  LocalContext * localCtx = (LocalContext*)malloc(sizeof(LocalContext));
  initLocalContext(ctx, localCtx, quad->addRight);
  localCtx->prev = ctx->currentContext;
  ctx->nextContext = localCtx;
}

void handleParam(Quadruple *  quad, MachineContext * ctx){
  TYPES paramType = vAddressToType(quad->addRight);
  switch(paramType){
    case TYPE_INT:
    {
      int* source = (int*)getMemoryLocation(quad->addRight, ctx);
      int index = quad->addRes;
      ctx->nextContext->INT_VARIABLES[index] = *source;
      break;
    }
    case TYPE_FLOAT:
    {
      float* source = (float*)getMemoryLocation(quad->addRight, ctx);
      int index = quad->addRes;
      ctx->nextContext->FLOAT_VARIABLES[index] = *source;
    }
    break;
  }
}

int handleGosub(int index, Quadruple * quad, MachineContext * ctx){
  sPush(&ctx->instructionLog, index);
  // printf("Instruction pointer _ PUSH %d\n", index);
  ctx->currentContext = ctx->nextContext;
  ctx->nextContext = NULL;
  // printf("%s\n", ctx->functions[quad->addRight].name);
  int newIndex = ctx->functions[quad->addRight].StartQuad;
  // printf("%d\n", newIndex);
  return newIndex;
}

int handleEndfunc(Quadruple *  quad, MachineContext * ctx){
  int index = sPop(&ctx->instructionLog);
  // printf("Instruction pointer _ POP %d\n", index);
  LocalContext * nextContext = ctx->currentContext->prev;
  free(ctx->currentContext);
  ctx->currentContext = nextContext;

  return index;
}

void handleBinaryOperation(Quadruple * quad, MachineContext * ctx){
  float left = getNumericValue(quad->addLeft, ctx);
  float right = getNumericValue(quad->addRight, ctx);
  float result = getResult(left, right, quad->op);
  
  TYPES resultType = vAddressToType(quad->addRes);
  void* resPtr = getMemoryLocation(quad->addRes, ctx);

  switch (resultType) {
      case TYPE_INT:
          *((int*)resPtr) = (int)result;  // truncado
          break;
      case TYPE_FLOAT:
          *((float*)resPtr) = result;
          break;
      default:
          fprintf(stderr, "Invalid result type in add\n");
          exit(1);
  }
}

bool handleGotoF(Quadruple * quad, MachineContext * ctx){
  bool res = *(bool*)getMemoryLocation(quad->addLeft, ctx);
  // printf("GotoF Checking addres %d and got %d\n", quad->addLeft, res);
  return res;
}

void handlePrint(Quadruple * quad, MachineContext * ctx){
  TYPES leftType = vAddressToType(quad->addLeft);
  switch(leftType){
      case TYPE_STRING: {
        char* raw = ctx->STRING_CONSTANTS[quad->addLeft - STRING_CONSTANTS_SEGMENT];
        char* unescaped = unescape_string(raw);
        printf("%s", unescaped);
        free(unescaped); // clean up the heap memory
      }
    break;
    case TYPE_INT: {
      int * address = (int*)getMemoryLocation(quad->addLeft, ctx);
      printf("%d",*address);
      break;
    }
    case TYPE_FLOAT: {
      float * address = (float*)getMemoryLocation(quad->addLeft, ctx);
      printf("%.2f", *address);
    }
    break;
  }
}