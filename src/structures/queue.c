#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

/* Initialize an empty queue */
Queue* init_queue() {
    Queue *q = malloc(sizeof(Queue));
    if (q) {
        q->front = NULL;
        q->rear = NULL;
    }
    return q;
}

/* Add a message to the end of the queue */
void enqueue(Queue *q, const char *msg) {
    QueueNode *newNode = malloc(sizeof(QueueNode));
    if (!newNode) return;
    
    strncpy(newNode->message, msg, 254);
    newNode->message[254] = '\0';
    newNode->next = NULL;
    
    if (q->rear == NULL) {
        q->front = q->rear = newNode;
        return;
    }
    
    q->rear->next = newNode;
    q->rear = newNode;
}

/* Remove and return the oldest message from the queue */
char* dequeue(Queue *q) {
    if (q->front == NULL) return NULL;
    
    QueueNode *temp = q->front;
    char *msg = strdup(temp->message);
    
    q->front = q->front->next;
    if (q->front == NULL) q->rear = NULL;
    
    free(temp);
    return msg;
}

int is_queue_empty(Queue *q) {
    return (q->front == NULL);
}