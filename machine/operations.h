#include "../consts.h"
#include "../utils/quads/quads.h"
#include "machineContext.h"
#include "../utils/stringUtils.h"
#include "../utils/stack.h"

void * getMemoryLocation(int searchAddress, MachineContext * ctx);
void initLocalContext(MachineContext *ctx, LocalContext * localCtx, int functionId);
bool handleGotoF(Quadruple * quad, MachineContext * ctx);
void handlePrint(Quadruple * quad, MachineContext * ctx);
void handleEquals(Quadruple * quad, MachineContext * ctx);
void handleLessThan(Quadruple * quad, MachineContext * ctx);
void handleGreaterThan(Quadruple * quad, MachineContext * ctx);
float getNumericValue(int address, MachineContext* ctx);
void handleEra(Quadruple *  quad, MachineContext * ctx);
void handleParam(Quadruple *  quad, MachineContext * ctx);
int handleGosub(int index, Quadruple * quad, MachineContext * ctx);
int handleEndfunc(Quadruple *  quad, MachineContext * ctx);
void handleBinaryOperation(Quadruple * quad, MachineContext * ctx);
float getResult(float a, float b, OPERATORS op);
