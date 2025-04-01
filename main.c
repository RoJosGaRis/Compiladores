#include <stdio.h>
#include <stdbool.h>

#define MAX_SIZE 100

typedef struct {
  int arr[MAX_SIZE];
  int top;
} Stack;

// Initialize
void initialize(Stack *stack){
  stack -> top = -1;
  return;
}
// IsEmpty
bool isEmpty(Stack *stack){
  return stack->top == -1;
}
// IsFull
bool isFull(Stack *stack){
  return stack->top == MAX_SIZE - 1;
}
// Peek
int peek(Stack *stack){
  if(isEmpty(stack)){
    printf("Stack está vacío");
    return -1;
  }
  return stack->arr[stack->top];
}
// Push
void push(Stack *stack, int value){
  if(isFull(stack)){
    printf("Stack lleno. No se puede agregar mas.\n");
    return;
  } 
  stack->arr[++stack->top] = value;
  return;
}
// Pop
int pop(Stack *stack){
  if(isEmpty(stack)){
    printf("Stack vacio. No se puede sacar nada.\n");
    return -1;
  }
  int val = stack->arr[stack->top];
  stack->top--;
  return val;
}


int main(){
  Stack stack;
  initialize(&stack);
  
  push(&stack, 1);
  printf("Top: %d\n", peek(&stack));
  push(&stack, 2);
  printf("Top: %d\n", peek(&stack));
  printf("Sacando: %d\n", pop(&stack));
  printf("%s\n", isEmpty(&stack) ? "Vacio" : "No vacio");
  printf("Top: %d\n", peek(&stack));
  printf("Sacando: %d\n", pop(&stack));
  printf("%s\n", isEmpty(&stack) ? "Vacio" : "No vacio");
  printf("Sacando: %d\n", pop(&stack));

  for(int i = 1; i <= 101; i++){
    push(&stack, i);
  }
  printf("Top: %d\n", peek(&stack));

  return 0;
}