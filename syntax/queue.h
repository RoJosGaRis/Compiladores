// queue.h
#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include "tokenNode.h"
#include "../consts.h"

// #define MAX_SIZE 10000

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
void qInitialize(struct Queue *queue);
bool qIsEmpty(struct Queue *q);
bool qIsFull(struct Queue *q);
TokenNode* qPeek(struct Queue *q);
void enqueue(struct Queue *q, TokenNode *val);
TokenNode* dequeue(struct Queue *q);

#endif // QUEUE_H