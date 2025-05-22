#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../consts.h"
// STACK START
typedef struct {
  int arr[MAX_STACK_SIZE];
  int top;
} Stack;

// Initialize
void sInitialize(Stack *stack);
// IsEmpty
bool sIsEmpty(Stack *stack);
// IsFull
bool sIsFull(Stack *stack);
// Peek
int sPeek(Stack *stack);
// Push
void sPush(Stack *stack, int value);
// Pop
int sPop(Stack *stack);
// STACK END
#endif