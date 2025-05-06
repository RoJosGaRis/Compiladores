#include "types.h"
#include "string.h"

TYPES stringToType(const char* typeStr) {
    if (strcmp(typeStr, "int") == 0) return TYPE_INT;
    if (strcmp(typeStr, "float") == 0) return TYPE_FLOAT;
    if (strcmp(typeStr, "string") == 0) return TYPE_STRING;
    if (strcmp(typeStr, "bool") == 0) return TYPE_BOOL;
    return TYPE_UNKNOWN;
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