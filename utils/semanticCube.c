#include "semanticCube.h"


void initSemanticCube() {
    // Inicializar todo como error
    for (int i = 0; i < TYPE_COUNT; i++) {
        for (int j = 0; j < OP_COUNT; j++) {
            for (int k = 0; k < TYPE_COUNT; k++) {
                semanticCube[i][j][k] = TYPE_ERROR;
            }
        }
    }

    // int + int = int
    semanticCube[TYPE_INT][OP_ADD][TYPE_INT] = TYPE_INT;
    semanticCube[TYPE_INT][OP_SUB][TYPE_INT] = TYPE_INT;
    semanticCube[TYPE_INT][OP_MUL][TYPE_INT] = TYPE_INT;
    semanticCube[TYPE_INT][OP_DIV][TYPE_INT] = TYPE_INT;
    semanticCube[TYPE_INT][OP_EQ][TYPE_INT] = TYPE_INT;

    // int + float = float
    semanticCube[TYPE_INT][OP_ADD][TYPE_FLOAT] = TYPE_FLOAT;
    semanticCube[TYPE_FLOAT][OP_ADD][TYPE_INT] = TYPE_FLOAT;
    semanticCube[TYPE_INT][OP_SUB][TYPE_FLOAT] = TYPE_FLOAT;
    semanticCube[TYPE_FLOAT][OP_SUB][TYPE_INT] = TYPE_FLOAT;
    semanticCube[TYPE_INT][OP_MUL][TYPE_FLOAT] = TYPE_FLOAT;
    semanticCube[TYPE_FLOAT][OP_MUL][TYPE_INT] = TYPE_FLOAT;
    semanticCube[TYPE_INT][OP_DIV][TYPE_FLOAT] = TYPE_FLOAT;
    semanticCube[TYPE_FLOAT][OP_DIV][TYPE_INT] = TYPE_FLOAT;

    // float + float = float
    semanticCube[TYPE_FLOAT][OP_ADD][TYPE_FLOAT] = TYPE_FLOAT;
    semanticCube[TYPE_FLOAT][OP_SUB][TYPE_FLOAT] = TYPE_FLOAT;
    semanticCube[TYPE_FLOAT][OP_MUL][TYPE_FLOAT] = TYPE_FLOAT;
    semanticCube[TYPE_FLOAT][OP_DIV][TYPE_FLOAT] = TYPE_FLOAT;
    semanticCube[TYPE_FLOAT][OP_EQ][TYPE_FLOAT] = TYPE_FLOAT;

    // string + string = string
    semanticCube[TYPE_STRING][OP_ADD][TYPE_STRING] = TYPE_STRING;

    // Comparaciones
    semanticCube[TYPE_INT][OP_GT][TYPE_INT] = TYPE_BOOL;
    semanticCube[TYPE_INT][OP_LT][TYPE_INT] = TYPE_BOOL;
    semanticCube[TYPE_INT][OP_DEQ][TYPE_INT] = TYPE_BOOL;

    semanticCube[TYPE_INT][OP_GT][TYPE_FLOAT] = TYPE_BOOL;
    semanticCube[TYPE_INT][OP_LT][TYPE_FLOAT] = TYPE_BOOL;
    semanticCube[TYPE_INT][OP_DEQ][TYPE_FLOAT] = TYPE_BOOL;

    semanticCube[TYPE_FLOAT][OP_GT][TYPE_INT] = TYPE_BOOL;
    semanticCube[TYPE_FLOAT][OP_LT][TYPE_INT] = TYPE_BOOL;
    semanticCube[TYPE_FLOAT][OP_DEQ][TYPE_INT] = TYPE_BOOL;
    
    semanticCube[TYPE_FLOAT][OP_GT][TYPE_FLOAT] = TYPE_BOOL;
    semanticCube[TYPE_FLOAT][OP_LT][TYPE_FLOAT] = TYPE_BOOL;
    semanticCube[TYPE_FLOAT][OP_DEQ][TYPE_FLOAT] = TYPE_BOOL;
}

TYPES getResultingType(TYPES left, OPERATORS op, TYPES right) {
    if (left >= TYPE_COUNT || right >= TYPE_COUNT || op >= OP_COUNT) return TYPE_ERROR;
    return semanticCube[left][op][right];
}