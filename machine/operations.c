#include "operations.h"


void * getMemoryLocation(int searchAddress, MachineContext * ctx){

  if(searchAddress >= INT_VARIABLES_SEGMENT && searchAddress < INT_CONSTANTS_SEGMENT) return &ctx->INT_VARIABLES[searchAddress - INT_VARIABLES_SEGMENT];  
  if(searchAddress >= INT_CONSTANTS_SEGMENT && searchAddress < INT_TEMPS_SEGMENT) return &ctx->INT_CONSTANTS[searchAddress - INT_CONSTANTS_SEGMENT];
  if(searchAddress >= INT_TEMPS_SEGMENT && searchAddress < FLOAT_VARIABLES_SEGMENT) return &ctx->INT_TEMPS[searchAddress-INT_TEMPS_SEGMENT];
  
  if(searchAddress >= FLOAT_VARIABLES_SEGMENT && searchAddress < FLOAT_CONSTANTS_SEGMENT) return &ctx->FLOAT_VARIABLES[searchAddress - FLOAT_VARIABLES_SEGMENT];  
  if(searchAddress >= FLOAT_CONSTANTS_SEGMENT && searchAddress < FLOAT_TEMPS_SEGMENT) return &ctx->FLOAT_CONSTANTS[searchAddress - FLOAT_CONSTANTS_SEGMENT];
  if(searchAddress >= FLOAT_TEMPS_SEGMENT && searchAddress < STRING_CONSTANTS_SEGMENT) return &ctx->FLOAT_TEMPS[searchAddress-FLOAT_TEMPS_SEGMENT];
  
  perror("Failed to fetch int location in memory. Address is not integer.");
  exit(1);
}

void handleEquals(Quadruple * quad, MachineContext * ctx){
  TYPES rightType = vAddressToType(quad->addRight);
  TYPES resType = vAddressToType(quad->addRes);
  
  if(rightType == resType){
    switch(rightType){
      case TYPE_INT: {
        int *source = getMemoryLocation(quad->addRight, ctx);
        int *target = getMemoryLocation(quad->addRes, ctx);
        int value = *source;
        *target = value;
        break;
      }
      case TYPE_FLOAT: {

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

void handlePrint(Quadruple * quad, MachineContext * ctx){
  TYPES leftType = vAddressToType(quad->addLeft);
  switch(leftType){
    case TYPE_STRING:
      printf(ctx->STRING_CONSTANTS[quad->addLeft - STRING_CONSTANTS_SEGMENT]);
      fflush(stdout);
    break;
    case TYPE_INT: {
      int * address = (int*)getMemoryLocation(quad->addLeft, ctx);
      printf("%d\n",*address);
      fflush(stdout);
      break;
    }
    case TYPE_FLOAT: {
      float * address = (float*)getMemoryLocation(quad->addLeft, ctx);
      printf("%2f\n", *address);
      fflush(stdout);
    }
    break;
  }
}