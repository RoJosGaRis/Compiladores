#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "uthash.h"

#define MAX_SIZE 100

// STACK START
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
// STACK END

// QUEUE START
struct QueueNode{
  int value;
  struct QueueNode *next;
};
struct Queue{
  struct QueueNode *front, *rear;
  int count;
};

void initialize(struct Queue *queue){
  queue -> front = NULL;
  queue -> rear = NULL;
  queue -> count = 0;
}

// isEmpty
bool isEmpty(struct Queue *q){
  return q->count == 0;
}
// isFull
bool isFull(struct Queue *q){
  return q->count == MAX_SIZE;
}
// peek
int peek(struct Queue *q){
  return q->front->value;
}
// enqueue
void enqueue(struct Queue *q, int val){
  // printf("ENQUEUE");
  if(isFull(q)) {
    printf("Queue llena\n");
    return;
  }
  // printf("ENQUEUE2");
  
  struct QueueNode *newNode;
  newNode = (struct QueueNode*) malloc(sizeof(struct QueueNode));
  newNode->value = val;
  newNode->next = NULL;
  
  // printf("ENQUEUE3");
  if(isEmpty(q)){
    q->front = newNode;
    q->rear = newNode;
  } else {
    q->rear->next = newNode;
    q->rear = newNode;
  }
    
  q->count++;
}
// dequeue
int dequeue(struct Queue *q){
  struct QueueNode *res;
  res = (struct QueueNode*) malloc(sizeof(struct QueueNode));
  res = q->front;

  if(isEmpty(q)){
    printf("Queue vacia\n");
    return -1;
  } else {
    q->front = q->front->next;
    q->count--;
  }

  return res->value;
}

// QUEUE END

// HASH START
struct hashEntry{
  int id;
  char name[10];
  UT_hash_handle hh;
} ;

void addHashEntry(struct hashEntry **table, int id, char *value){
  struct hashEntry *entry;

  HASH_FIND_INT(*table, &id, entry);
  if(entry == NULL){
    entry = (struct hashEntry*)malloc(sizeof *entry);
    entry->id = id;
    HASH_ADD_INT(*table, id, entry);
  } 
  strcpy(entry->name, value);
}

char *findEntry(struct hashEntry **table, int id){
  struct hashEntry *entry;
  HASH_FIND_INT(*table, &id, entry);
  return entry->name;
}
// HASH END

int main(){

  // Stack Test Cases Start
  // Stack stack;
  // initialize(&stack);
  
  // push(&stack, 1);
  // printf("Top: %d\n", peek(&stack));
  // push(&stack, 2);
  // printf("Top: %d\n", peek(&stack));
  // printf("Sacando: %d\n", pop(&stack));
  // printf("%s\n", isEmpty(&stack) ? "Vacio" : "No vacio");
  // printf("Top: %d\n", peek(&stack));
  // printf("Sacando: %d\n", pop(&stack));
  // printf("%s\n", isEmpty(&stack) ? "Vacio" : "No vacio");
  // printf("Sacando: %d\n", pop(&stack));

  // for(int i = 1; i <= 101; i++){
  //   push(&stack, i);
  // }
  // printf("Top: %d\n", peek(&stack));
  
  // Stack Test Cases End
  
  // // QUEUE Test Cases Start
  // struct Queue q;
  // initialize(&q);  
  // printf("Dequeue: %d\n", dequeue(&q));
  // enqueue(&q, 1);
  // printf("Top: %d\n", peek(&q));
  // enqueue(&q, 2);
  // printf("Top: %d\n", peek(&q));
  // enqueue(&q, 3);
  // printf("Top: %d\n", peek(&q));
  // printf("Dequeue: %d\n", dequeue(&q));
  // printf("Top: %d\n", peek(&q));
  
  // for(int i = 1; i <= 99; i++){
  //   enqueue(&q, i);
  // }
  // // QUEUE Test Cases End

  // HASH Test Cases Start
  struct hashEntry * identifiersTable = NULL;
  addHashEntry(&identifiersTable, 0, (char*)"Hello");
  addHashEntry(&identifiersTable, 1, (char*)",");
  addHashEntry(&identifiersTable, 2, (char*)"World");
  printf("%s", findEntry(&identifiersTable, 2));
  printf("%s", findEntry(&identifiersTable, 1));
  printf("%s", findEntry(&identifiersTable, 0));
  // HASH Test Cases End

  return 0;
}