#include "types.h"
#include "string.h"

int priorities[] = {
    1, 
    1,
    1,
    2,
    3,
    3,
    4,
    4,
    -1,
    -1,
};

TYPES stringToType(const char* typeStr) {
    if (strcmp(typeStr, "int") == 0) return TYPE_INT;
    if (strcmp(typeStr, "float") == 0) return TYPE_FLOAT;
    if (strcmp(typeStr, "string") == 0) return TYPE_STRING;
    if (strcmp(typeStr, "bool") == 0) return TYPE_BOOL;
    return TYPE_ERROR;
}

OPERATORS stringToOp(char* op) {
    //  OP_ADD,     // +
    if (strcmp(op, "+") == 0) return OP_ADD;
    //  OP_SUB,     // -
    if (strcmp(op, "-") == 0) return OP_SUB;
    //  OP_MUL,     // *
    if (strcmp(op, "*") == 0) return OP_MUL;
    //  OP_DIV,     // /
    if (strcmp(op, "/") == 0) return OP_DIV;
    //  OP_EQ,
    if (strcmp(op, "=") == 0) return OP_EQ;
    //  OP_DEQ,     // ==
    if (strcmp(op, "==") == 0) return OP_DEQ;
    //  OP_GT,      // >
    if (strcmp(op, ">") == 0) return OP_GT;
    //  OP_LT,      // <
    if (strcmp(op, "<") == 0) return OP_LT;
    //  OP_GOTO
    if (strcmp(op, "GOTO") == 0) return OP_GOTO;
    //  OP_GOTOF
    if (strcmp(op, "GOTOF") == 0) return OP_GOTOF;
    //  OP_GOTOT
    if (strcmp(op, "GOTOT") == 0) return OP_GOTOT;
    return OP_ERROR;
}

const char* typeToString(TYPES type) {
    switch (type) {
        case TYPE_INT: return "int";
        case TYPE_FLOAT: return "float";
        case TYPE_STRING: return "string";
        case TYPE_BOOL: return "bool";
        default: return "unknown";
    }
}
const char* opToString(OPERATORS op)
{
    switch(op){
        //  OP_ADD,     // +
        case OP_ADD:
            return "+";
        break;
        //  OP_SUB,     // -
        case OP_SUB:
            return "-";
        break;
        //  OP_MUL,     // *
        case OP_MUL:
            return "*";
        break;
        //  OP_DIV,     // /
        case OP_DIV:
            return "/";
        break;
        //  OP_EQ,
        case OP_EQ:
            return "=";
        break;
        //  OP_DEQ,     // ==
        case OP_DEQ:
            return "==";
        break;
        //  OP_GT,      // >
        case OP_GT:
            return ">";
        break;
        //  OP_LT,      // <
        case OP_LT:
            return "<";
        break;
    }
    return NULL;
}
bool hasPriority(TYPES op1, TYPES op2)
{
    return priorities[op1] > priorities[op2];
}