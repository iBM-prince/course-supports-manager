#ifndef QUEUE_H
#define QUEUE_H

/* Node structure for the queue */
typedef struct QueueNode {
    char message[255];
    struct QueueNode *next;
} QueueNode;

/* Queue structure */
typedef struct {
    QueueNode *front;
    QueueNode *rear;
} Queue;

/* Function prototypes */
Queue* init_queue();
void enqueue(Queue *q, const char *msg);
char* dequeue(Queue *q);
int is_queue_empty(Queue *q);

#endif