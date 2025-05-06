// queue.h
#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include "tokenNode.h"

#define MAX_SIZE 10000

// Definición de nodo de la cola
struct QueueNode {
    TokenNode* value;
    struct QueueNode *next;
};

// Definición de la cola
struct Queue {
    struct QueueNode *front, *rear;
    int count;
};

// Declaraciones de funciones
void initialize(struct Queue *queue);
bool isEmpty(struct Queue *q);
bool isFull(struct Queue *q);
TokenNode* peek(struct Queue *q);
void enqueue(struct Queue *q, TokenNode *val);
TokenNode* dequeue(struct Queue *q);

#endif // QUEUE_H