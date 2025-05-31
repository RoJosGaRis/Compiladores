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
  ctx->quadList->quadruples = (Quadruple*)malloc(sizeof(Quadruple) * ctx->QUAD_COUNT); // TODO: Cargar el tamaño del arreglo desde un inicio
  ctx->quadList->count = 0;
  
  ctx->currentContext = NULL;
  ctx->nextContext = NULL;

  sInitialize(&ctx->instructionLog);
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
  ctx->QUAD_COUNT= strtol(fgets(line, sizeof(line), fileptr), NULL, 10);
  ctx->INT_CONSTANTS_COUNT= strtol(fgets(line, sizeof(line), fileptr), NULL, 10);
  ctx->FLOAT_CONSTANTS_COUNT= strtol(fgets(line, sizeof(line), fileptr), NULL, 10);
  ctx->STRING_CONSTANTS_COUNT= strtol(fgets(line, sizeof(line), fileptr), NULL, 10);
  ctx->FUNCTION_COUNT = strtol(fgets(line, sizeof(line), fileptr), NULL, 10);
  
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
    // Remove trailing newline if present
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
      line[len - 1] = '\0';
    }
    ctx->STRING_CONSTANTS[i] = strdup(line);
    
  }

  ctx->functions = (FunctionData*)malloc(sizeof(FunctionData) * ctx->FUNCTION_COUNT);

  for(int i = 0; i < ctx->FUNCTION_COUNT; i++){
    // printf("Funcion count: %d\n", ctx->FUNCTION_COUNT);
    fgets(line, sizeof(line), fileptr);
    ctx->functions[i].name = strdup(line);
    // printf("Index: %d Name: %s\n", i, ctx->functions[i].name);
    ctx->functions[i].Index = strtol(fgets(line, sizeof(line), fileptr), NULL, 10);
    fgets(line, sizeof(line), fileptr);
    ctx->functions[i].Firma = strdup(line);
    ctx->functions[i].IntVariable = strtol(fgets(line, sizeof(line), fileptr), NULL, 10);
    ctx->functions[i].IntTemps = strtol(fgets(line, sizeof(line), fileptr), NULL, 10);
    ctx->functions[i].FloatVariables = strtol(fgets(line, sizeof(line), fileptr), NULL, 10);
    ctx->functions[i].FloatTemps = strtol(fgets(line, sizeof(line), fileptr), NULL, 10);
    ctx->functions[i].BoolTemps = strtol(fgets(line, sizeof(line), fileptr), NULL, 10);
    ctx->functions[i].StartQuad = strtol(fgets(line, sizeof(line), fileptr), NULL, 10);
  }
  // exit(1);
  while(fgets(line, sizeof(line), fileptr)){
    Quadruple quad;
    if (sscanf(line, "%d\t%d\t%d\t%d", &quad.op, &quad.addLeft, &quad.addRight, &quad.addRes) == 4) {
        // Ya tienes los valores enteros
        // printf("%d:: OP: %d, LEFT: %d, RIGHT: %d, RESULT: %d\n", ctx->quadList->count, quad.op, quad.addLeft, quad.addRight, quad.addRes);
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
    // printf("\n%d:: OP: %d, LEFT: %d, RIGHT: %d, RESULT: %d\n", instructionPtr, quad->op, quad->addLeft, quad->addRight, quad->addRes);
    switch(quad->op){
      case OP_EQ:
        handleEquals(quad, ctx);
      break;
      case OP_LT:
        handleLessThan(quad, ctx);
      break;
      case OP_GT:
        handleGreaterThan(quad, ctx);
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
      case OP_GOTO:
      // printf("Old ptr: %d\n", instructionPtr);
        instructionPtr = quad->addRight;
        // printf("New ptr: %d\n", instructionPtr);
        continue;
      break;
      case OP_GOTOF:

        if(!handleGotoF(quad, ctx)){
          instructionPtr = quad->addRight;
          continue;
        }
      break;
      case OP_ERA:
        handleEra(quad, ctx);
        break;
      case OP_PARAM:
        handleParam(quad, ctx);
        break;
      case OP_GOSUB:
        // printf("Old ptr: %d\n", instructionPtr);
        instructionPtr = handleGosub(instructionPtr, quad, ctx);
        // printf("New ptr: %d\n", instructionPtr);
        continue;
        break;
        case OP_ENDFUNC:
        instructionPtr = handleEndfunc(quad, ctx) +1;
        // printf("New ptr: %d\n", instructionPtr);
        continue;
      break;
      case OP_PRINT:
        handlePrint(quad, ctx);
      break;
      default:
        printf("Instrucción no reconocida: ");
        printf("%d", quad->op);
        exit(1);
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