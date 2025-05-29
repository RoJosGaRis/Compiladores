#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "operations.h"

void initMachineContext(MachineContext * ctx);
void machine_run(char * filename);
MachineContext * loadMachine(char* filename);
void freeMachineContext(MachineContext* ctx);