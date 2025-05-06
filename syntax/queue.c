#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

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
TokenNode* peek(struct Queue *q){
  if(isEmpty(q))
  {
    printf("Queue vacía\n");
    return NULL;
  }
  return q->front->value;
}
// enqueue
void enqueue(struct Queue *q, TokenNode* val){
  // printf("ENQUEUE");
  if(isFull(q)) {
    printf("QUEUE: Queue llena\n");
    return;
  }
  
  struct QueueNode *newNode = (struct QueueNode*) malloc(sizeof(struct QueueNode));
  if(!newNode)
  {
    printf("QUEUE: Error al asignar memoria\n");
  }

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
TokenNode* dequeue(struct Queue *q){
  
  if(isEmpty(q)){
    printf("Queue vacia\n");
    return NULL;
  }
  
  struct QueueNode *temp = q->front;
  TokenNode* value = temp->value;

  q->front = q->front->next;
  q->count--;
  free(temp);


  return value;
}

// QUEUE END