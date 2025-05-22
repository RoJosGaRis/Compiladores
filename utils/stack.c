#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stack.h"

// Initialize
void sInitialize(Stack *stack){
  stack -> top = -1;
  return;
}
// IsEmpty
bool sIsEmpty(Stack *stack){
  return stack->top == -1;
}
// IsFull
bool sIsFull(Stack *stack){
  return stack->top == MAX_STACK_SIZE - 1;
}
// Peek
int sPeek(Stack *stack){
  if(sIsEmpty(stack)){
    // printf("Stack está vacío");
    return -1;
  }
  return stack->arr[stack->top];
}
// Push
void sPush(Stack *stack, int value){
  if(sIsFull(stack)){
    printf("Stack lleno. No se puede agregar mas.\n");
    return;
  } 
  stack->arr[++stack->top] = value;
  // printf("Pushed into stack %d\n", value);
  return;
}
// Pop
int sPop(Stack *stack){
  if(sIsEmpty(stack)){
    printf("Stack vacio. No se puede sacar nada.\n");
    return -1;
  }
  int val = stack->arr[stack->top];
  stack->top--;
  // printf("Returning %d\n", val);
  // printf("New top: %d Index: %d\n", stack->arr[stack->top], stack->top);
  return val;
}
// STACK END