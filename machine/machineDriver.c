#include "machineDriver.h"

void initMachineContext(MachineContext * ctx){
  // Globals
  ctx->INT_VARIABLES = (int*)malloc(sizeof(int) * ctx->INT_VARIABLES_COUNT);
  ctx->FLOAT_VARIABLES = (float*)malloc(sizeof(float) * ctx->FLOAT_VARIABLES_COUNT);
  // Temps
  ctx->INT_TEMPS = (int*)malloc(sizeof(int) * ctx->INT_TEMPS_COUNT);
  ctx->FLOAT_TEMPS = (float*)malloc(sizeof(float) * ctx->FLOAT_TEMPS_COUNT);
  ctx->BOOL_TEMPS = (bool*)malloc(sizeof(bool) * ctx->BOOL_TEMPS_COUNT);
  // Constants
  ctx->INT_CONSTANTS = (int*)malloc(sizeof(int) * ctx->INT_CONSTANTS_COUNT);
  ctx->FLOAT_CONSTANTS = (float*)malloc(sizeof(float) * ctx->FLOAT_CONSTANTS_COUNT);
  ctx->STRING_CONSTANTS = (char**)malloc(sizeof(char*) * ctx->STRING_CONSTANTS_COUNT);

  ctx->quadList = (QuadList*)malloc(sizeof(QuadList));
  ctx->quadList->quadruples = (Quadruple*)malloc(sizeof(Quadruple)); // TODO: Cargar el tamaño del arreglo desde un inicio
  ctx->quadList->count = 0;
}

MachineContext * loadMachine(char* filename){
  FILE * fileptr = fopen(filename, "r");
  MachineContext * ctx = (MachineContext *)malloc(sizeof(MachineContext));

  if (fileptr == NULL) {
    perror("No se pudo abrir el archivo");
    exit(EXIT_FAILURE);
  }

  char line[256];
  ctx->INT_VARIABLES_COUNT = strtol(fgets(line, sizeof(line), fileptr), NULL, 10);
  ctx->INT_TEMPS_COUNT = strtol(fgets(line, sizeof(line), fileptr), NULL, 10);
  ctx->FLOAT_VARIABLES_COUNT = strtol(fgets(line, sizeof(line), fileptr), NULL, 10);
  ctx->FLOAT_TEMPS_COUNT = strtol(fgets(line, sizeof(line), fileptr), NULL, 10);
  ctx->BOOL_TEMPS_COUNT = strtol(fgets(line, sizeof(line), fileptr), NULL, 10);
  ctx->INT_CONSTANTS_COUNT= strtol(fgets(line, sizeof(line), fileptr), NULL, 10);
  ctx->FLOAT_CONSTANTS_COUNT= strtol(fgets(line, sizeof(line), fileptr), NULL, 10);
  ctx->STRING_CONSTANTS_COUNT= strtol(fgets(line, sizeof(line), fileptr), NULL, 10);

  initMachineContext(ctx);
  
  for (int i = 0; i < ctx->INT_CONSTANTS_COUNT; i++) {
    int newVal = strtol(fgets(line, sizeof(line), fileptr), NULL, 10);
    ctx->INT_CONSTANTS[i] = newVal;
  }
  for (int i = 0; i < ctx->FLOAT_CONSTANTS_COUNT; i++) {
    float newVal = strtof(fgets(line, sizeof(line), fileptr), NULL);
    ctx->FLOAT_CONSTANTS[i] = newVal;
  }
  for (int i = 0; i < ctx->STRING_CONSTANTS_COUNT; i++) {
    fgets(line, sizeof(line), fileptr);
    ctx->STRING_CONSTANTS[i] = strdup(line);;
  }

  while(fgets(line, sizeof(line), fileptr)){
    Quadruple quad;
    if (sscanf(line, "%d\t%d\t%d\t%d", &quad.op, &quad.addLeft, &quad.addRight, &quad.addRes) == 4) {
        // Ya tienes los valores enteros
        printf("%d:: OP: %d, LEFT: %d, RIGHT: %d, RESULT: %d\n",
          ctx->quadList->count, quad.op, quad.addLeft, quad.addRight, quad.addRes);
        ctx->quadList->quadruples[ctx->quadList->count] = quad;
        ctx->quadList->count++;
    } else {
        fprintf(stderr, "Error: línea malformada -> %s", line);
    }
  }

  return ctx;
}

void machine_run(char* filename){
  MachineContext * ctx = (MachineContext*)malloc(sizeof(MachineContext));
  ctx = loadMachine(filename);

  int instructionPtr = 0;
  while(instructionPtr < ctx->quadList->count){
    Quadruple * quad = &ctx->quadList->quadruples[instructionPtr];
    switch(quad->op){
      case OP_EQ:
        handleEquals(quad, ctx);
      break;
      case OP_ADD:
        handleBinaryOperation(quad, ctx);
      break;
      case OP_SUB:
        handleBinaryOperation(quad, ctx);
      break;
      case OP_MUL:
        handleBinaryOperation(quad, ctx);
      break;
      case OP_DIV:
        handleBinaryOperation(quad, ctx);
      break;
      case OP_PRINT:
        handlePrint(quad, ctx);
      break;
      default:
        printf("%d\n", quad->op);
      break;
    }
    instructionPtr++;
  }
  // freeMachineContext(ctx);
}

void freeMachineContext(MachineContext* ctx) {
  free(ctx->INT_VARIABLES);
  free(ctx->FLOAT_VARIABLES);

  for (int i = 0; i < ctx->STRING_CONSTANTS_COUNT; i++) {
      free(ctx->STRING_CONSTANTS[i]);
  }
  free(ctx->STRING_CONSTANTS);
  free(ctx->quadList->quadruples);
  free(ctx->quadList);
  free(ctx);
}