#include "types.h"

static TYPES semanticCube [TYPE_COUNT][OP_COUNT][TYPE_COUNT];

void initSemanticCube();
TYPES getResultingType(TYPES left, OPERATORS op, TYPES right);