#include "../consts.h"
#include "../utils/quads/quads.h"
#include "machineContext.h"

void * getMemoryLocation(int searchAddress, MachineContext * ctx);
void handlePrint(Quadruple * quad, MachineContext * ctx);
void handleEquals(Quadruple * quad, MachineContext * ctx);
float getNumericValue(int address, MachineContext* ctx);
void handleBinaryOperation(Quadruple * quad, MachineContext * ctx);
float getResult(float a, float b, OPERATORS op);
